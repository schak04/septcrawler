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

double calcTFIDFScore(const CandidateDocument& cd, const std::vector<std::string>& query) {
    const InvertedIndex& invidx = getInvertedIndex();

    std::unordered_map<int, double> docLengths;  // docId -> total number of terms in that document
    std::unordered_set<int> distinctDocs;

    for (const auto& pair : invidx.index) {
        for (const auto& p : pair.second.entries) {
            docLengths[p.docId] += p.termFrequency;
            distinctDocs.insert(p.docId);
        }
    }

    double TFIDFScoreForCandidateDoc = 0;

    for (const std::string& term : query) {
        // TF
        // NOTE:
        // Safe because candidate documents are generated using AND retrieval,
        // so every candidate contains every query term.
        Posting posting;  // for (term, cd.docId)
        auto it = invidx.index.find(term);
        if (it != invidx.index.end()) {
            for (const Posting& p : it->second.entries) {
                if (p.docId == cd.docId) {
                    posting = p;
                    break;
                }
            }
        }

        const double countOfTermInDoc = posting.termFrequency;
        const double totalTermsInDoc = docLengths[cd.docId];

        const double TF = totalTermsInDoc > 0 ? (countOfTermInDoc / totalTermsInDoc) : 0.0;

        // IDF
        const double numberOfDocumentsInTheCorpus = static_cast<double>(distinctDocs.size());
        const double numberOfDocumentsContainingTerm =
            (it != invidx.index.end()) ? static_cast<double>(it->second.entries.size()) : 0.0;

        // for future ref: https://en.cppreference.com/cpp/numeric/math/log
        if (numberOfDocumentsContainingTerm > 0 && numberOfDocumentsInTheCorpus > 0) {
            // +1 on denominator to be safe from division by zero
            const double IDF =
                std::log(numberOfDocumentsInTheCorpus / (1 + numberOfDocumentsContainingTerm));

            // TF-IDF
            const double TFIDFScoreForQueryTerm = TF * IDF;
            TFIDFScoreForCandidateDoc += TFIDFScoreForQueryTerm;
        }
    }

    return TFIDFScoreForCandidateDoc;
}

std::vector<CandidateDocument> rankCandidateDocuments(
    const std::vector<CandidateDocument>& unrankedCandidateDocs,
    const std::vector<std::string>& query) {
    std::vector<CandidateDocument> rankedCandidateDocs = unrankedCandidateDocs;

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

    std::sort(rankedCandidateDocs.begin(), rankedCandidateDocs.end(),
              [&query](const CandidateDocument& a, const CandidateDocument& b) {
                  return calcTFIDFScore(a, query) > calcTFIDFScore(b, query);
              });

    return rankedCandidateDocs;
}
