#include "core-components/ranker.hpp"

#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_map>
#include <unordered_set>
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

double calcTFIDFScore(const CandidateDocument& cd, const std::vector<std::string>& query,
                      const CorpusStatistics& stats) {
    const InvertedIndex& invidx = getInvertedIndex();

    double TFIDFScoreForCandidateDoc = 0.0;

    for (const std::string& term : query) {
        // NOTE:
        // since candidate documents are generated through AND retrieval
        // every candidate is guaranteed to contain every query term
        // so a corresponding posting should exist for each candidate-term pair

        auto it = invidx.index.find(term);

        if (it == invidx.index.end()) {
            continue;
        }

        // find this doc's posting for the current query term
        int termFrequency = 0;

        for (const Posting& posting : it->second.entries) {
            if (posting.docId == cd.docId) {
                termFrequency = posting.termFrequency;
                break;
            }
        }

        // .at() is a safety net to catch invalid document IDs
        const double documentLength = stats.docLengths.at(cd.docId);

        // Term Frequency
        const double TF =
            documentLength > 0 ? static_cast<double>(termFrequency) / documentLength : 0.0;

        // Inverse Document Frequency
        const double numberOfDocumentsInTheCorpus = static_cast<double>(stats.distinctDocs.size());

        const double numberOfDocumentsContainingTerm =
            static_cast<double>(it->second.entries.size());

        // for future ref: https://en.cppreference.com/cpp/numeric/math/log
        // +1 on denominator to be safe from division by zero
        const double IDF =
            std::log(numberOfDocumentsInTheCorpus / (1 + numberOfDocumentsContainingTerm));

        // TF-IDF
        TFIDFScoreForCandidateDoc += TF * IDF;
    }

    return TFIDFScoreForCandidateDoc;
}

double calcTFIDFScore(const CandidateDocument& cd, const std::vector<std::string>& query) {
    const CorpusStatistics stats = buildCorpusStatistics(getInvertedIndex());
    return calcTFIDFScore(cd, query, stats);
}

std::vector<CandidateDocument> rankCandidateDocuments(
    const std::vector<CandidateDocument>& unrankedCandidateDocs,
    const std::vector<std::string>& query) {
    const InvertedIndex& invidx = getInvertedIndex();

    // corpus statistics calculated once per ranking operation
    const CorpusStatistics stats = buildCorpusStatistics(invidx);

    // each candidate alongside its calculated score
    std::vector<std::pair<CandidateDocument, double>> scoredDocuments;
    scoredDocuments.reserve(unrankedCandidateDocs.size());

    for (const CandidateDocument& doc : unrankedCandidateDocs) {
        const double score = calcTFIDFScore(doc, query, stats);

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
