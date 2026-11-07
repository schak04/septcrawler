#include "core-components/ranker.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core-components/retrieval-engine.hpp"
#include "data-structures/candidate-doc.hpp"
#include "data-structures/inverted-index.hpp"
#include "data-structures/posting-list.hpp"
#include "data-structures/posting.hpp"

// NOTE:
// TF = (count of term in document) / (total terms in document)
// IDF = log(total documents in corpus / documents containing term)
// TF-IDF = TF * IDF
// Measures the relative importance of a term to a specific document within a corpus.
// Higher values indicate a term is frequent in this document but rare across the corpus.

// to avoid unnecessary calculations of the corpus stats
struct CorpusStatistics {
    // total number of terms in each document (docId -> total number of terms in that document)
    std::unordered_map<int, double> docLengths;

    // number of unique documents in the corpus
    std::unordered_set<int> distinctDocs;
};

// stores query-term statistics and provides direct docId-to-termFrequency lookups
// to avoid repeated linear scans through the inverted index's posting lists
struct QueryTermStatistics {
    std::size_t documentFrequency;  // for a term: number of documents containing the term
    std::unordered_map<int, int> termFrequencies;  // docId -> termFrequency
};

// build corpus stats with a single traversal of the inverted index
CorpusStatistics buildCorpusStatistics(const InvertedIndex& invidx) {
    CorpusStatistics stats;

    for (const auto& [term, postingList] : invidx.index) {
        for (const Posting& posting : postingList.entries) {
            stats.docLengths[posting.docId] += posting.termFrequency;
            stats.distinctDocs.insert(posting.docId);
        }
    }

    return stats;
}

// build query-specific posting lookup tables once per ranking operation
// so each candidate can subsequently retrieve its termFrequency using an expected O(1)
// hash lookup instead of scanning the corresponding posting list
std::unordered_map<std::string, QueryTermStatistics> buildQueryTermStatistics(
    const InvertedIndex& invidx, const std::vector<std::string>& query) {
    // collection of (query term -> QueryTermStatistics{...}) mappings for a query
    std::unordered_map<std::string, QueryTermStatistics> queryStats;

    for (const std::string& term : query) {
        // only need to calculate and store stats for each unique query term
        if (queryStats.find(term) != queryStats.end()) {
            continue;
        }

        auto it = invidx.index.find(term);
        // skip terms that aren't in the index
        if (it == invidx.index.end()) {
            continue;
        }

        // init term stats
        // it->second.entries.size() = number of documents the term exists in (document frequency)
        // {} = empty termFrequencies hash map (supposed to map docId -> termFrequency)
        QueryTermStatistics termStats{it->second.entries.size(), {}};

        // as many (docId -> termFrequency) entries in the map as the document frequency

        // NOTE:
        // normally, as an unordered map grows it occasionally needs to:
        // allocate a larger bucket array, rehash existing elements,
        // and redistribute elements into the new buckets.
        // these operations introduce a bunch of overhead.
        // reserve() requests enough capacity to accommodate at least the
        // specified number of elements without requiring rehashing.
        termStats.termFrequencies.reserve(it->second.entries.size());

        for (const Posting& posting : it->second.entries) {
            // NOTE: emplace() -> build and insert a std::pair into the unordered_map
            // only inserted if the key is not already present in the unordered_map
            termStats.termFrequencies.emplace(posting.docId, posting.termFrequency);
        }

        // NOTE:
        // lvalue and rvalue refresher:
        // ----------------------------
        // lvalue: identifies a specific, persistent object
        // rvalue: represents a temporary value, generally not associated with a persistent object
        // example: int x = 10; (here, x -> lvalue, 10 -> rvalue, x + 5 -> rvalue)
        // lvalue reference: int x = 10; int& l = x;
        // const lvalue reference: const int& r = 10; -> can bind to an rvalue
        // rvalue reference: int&& rr = 10; -> can bind to an rvalue
        // C++ needs rvalue references to distinguish between
        // objects that might still be needed (lvalues),
        // and objects that can potentially give up their resources (rvalues).
        // rvalues are further divided into: prvalues (pure rvalues) and xvalues (expiring values).

        // NOTE: std::move() converts an lvalue to an rvalue reference
        // it essentially casts its argument into an xvalue
        // which makes move operations possible
        // example:
        // std::string a = "Hello";
        // std::string b = a; // copy (a separate copy of the string's contents is created)
        // std::string c = std::move(a); // move (the string's resources can be transferred instead
        // of copying its contents)

        // NOTE:
        // When initialising an object with std::move(source),
        // C++ can select the move constructor (T&&) instead of the
        // copy constructor (const T&). The move constructor implements
        // the actual move, potentially transferring resource ownership
        // rather than duplicating resources.

        queryStats.emplace(term, std::move(termStats));
    }

    return queryStats;
}

double calcTFIDFScore(const CandidateDocument& cd, const std::vector<std::string>& query,
                      const CorpusStatistics& stats,
                      const std::unordered_map<std::string, QueryTermStatistics>& queryStats) {
    double TFIDFScoreForCandidateDoc = 0.0;

    for (const std::string& term : query) {
        // NOTE:
        // since candidate documents are generated through AND retrieval
        // every candidate is guaranteed to contain every query term
        // so a corresponding posting should exist for each candidate-term pair

        // look up the query term in the precomputed query statistics
        // (instead of in the inverted index every time)
        auto it = queryStats.find(term);
        if (it == queryStats.end()) {
            continue;
        }

        // find this doc's posting for the current query term

        // directly look up the candidate's document ID in the term's precomputed hash map
        // (in place of the linear posting-list-search brute-force logic I wrote for v0.1.0)
        auto postingIt = it->second.termFrequencies.find(cd.docId);
        const int termFrequency =
            postingIt != it->second.termFrequencies.end() ? postingIt->second : 0;

        // .at() is a safety net to catch invalid document IDs
        const double documentLength = stats.docLengths.at(cd.docId);

        // Term Frequency
        const double TF =
            documentLength > 0 ? static_cast<double>(termFrequency) / documentLength : 0.0;

        // Inverse Document Frequency
        const double numberOfDocumentsInTheCorpus = static_cast<double>(stats.distinctDocs.size());

        const double numberOfDocumentsContainingTerm =
            static_cast<double>(it->second.documentFrequency);  // DF from precomputed stats

        // for future ref: https://en.cppreference.com/cpp/numeric/math/log
        // +1 on denominator to be safe from division by zero
        const double IDF =
            std::log(numberOfDocumentsInTheCorpus / (1 + numberOfDocumentsContainingTerm));

        // TF-IDF
        TFIDFScoreForCandidateDoc += TF * IDF;
    }

    return TFIDFScoreForCandidateDoc;
}

double calcTFIDFScore(const CandidateDocument& cd, const std::vector<std::string>& query,
                      const CorpusStatistics& stats) {
    const InvertedIndex& invidx = getInvertedIndex();
    const auto queryStats = buildQueryTermStatistics(invidx, query);
    return calcTFIDFScore(cd, query, stats, queryStats);
}

double calcTFIDFScore(const CandidateDocument& cd, const std::vector<std::string>& query) {
    const CorpusStatistics stats = buildCorpusStatistics(getInvertedIndex());
    return calcTFIDFScore(cd, query, stats);
}

std::vector<CandidateDocument> rankCandidateDocuments(
    const std::vector<CandidateDocument>& unrankedCandidateDocs,
    const std::vector<std::string>& query) {
    const InvertedIndex& invidx = getInvertedIndex();

    // both corpus stats & query stats calculated once per ranking operation
    // corpus stats -> corpus-wide info
    // query stats -> query-wide info
    const CorpusStatistics stats = buildCorpusStatistics(invidx);
    const auto queryStats = buildQueryTermStatistics(invidx, query);

    // each candidate alongside its calculated score
    std::vector<std::pair<CandidateDocument, double>> scoredDocuments;
    scoredDocuments.reserve(unrankedCandidateDocs.size());

    for (const CandidateDocument& doc : unrankedCandidateDocs) {
        const double score = calcTFIDFScore(doc, query, stats, queryStats);

        // NOTE:
        // Instead of creating a temporary object first and then copying or moving it
        // into the container (like push_back does), emplace_back forwards the constructor arguments
        // directly to the memory allocated inside the container.
        scoredDocuments.emplace_back(doc, score);
    }

    // NOTE: std::sort() internally uses a sorting algorithm called Introsort (Introspective Sort),
    // which is a hybrid sorting algorithm engineered to achieve optimal real-world performance.
    // It dynamically switches between 3 algorithms depending on the state of the data:
    // Quicksort (primary), Heapsort (fallback), and Insertion Sort (micro-optimisation).

    // NOTE:
    // Quicksort, the primary algorithm, triggers at the start of execution for large datasets.
    // When the recursion depth exceeds a threshold (typically 2logN base 2),
    // Heapsort acts as the fallback and guarantees a worst-case TC of O(NlogN),
    // preventing Quicksort from degrading it to O(N^2) on malicious / poorly partitioned data.
    // When a sub-partition size drops below a threshold (usually 16 elements),
    // Insertion sort gets triggered since it provides minimal overhead,
    // is highly efficient for small collections, and takes advantage of modern CPU memory caches.

    // compare precomputed scores instead of recalculating them
    // like I did at first, which made ranking slow as hell
    std::sort(scoredDocuments.begin(), scoredDocuments.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    std::vector<CandidateDocument> rankedCandidateDocs;
    rankedCandidateDocs.reserve(scoredDocuments.size());

    for (const auto& [doc, score] : scoredDocuments) {
        rankedCandidateDocs.push_back(doc);
    }

    return rankedCandidateDocs;
}
