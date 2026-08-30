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
            const double IDF =
                std::log(numberOfDocumentsInTheCorpus / numberOfDocumentsContainingTerm);

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

    std::sort(rankedCandidateDocs.begin(), rankedCandidateDocs.end(),
              [&query](const CandidateDocument& a, const CandidateDocument& b) {
                  return calcTFIDFScore(a, query) > calcTFIDFScore(b, query);
              });

    return rankedCandidateDocs;
}
