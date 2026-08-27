/*
 * NOTE:
 * Scoring and ranking implemented using TF-IDF.
 *
 * TODO:
 * Replace dummy data with real data once service and storage layers are implemented.
 * */

#include "core-components/ranker.hpp"

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "core-components/index-builder.hpp"
#include "core-components/query-processor.hpp"
#include "core-components/retrieval-engine.hpp"
#include "data-structures/candidate-doc.hpp"
#include "data-structures/inverted-index.hpp"
#include "data-structures/posting-list.hpp"
#include "data-structures/posting.hpp"

double calcTFIDFScore(const CandidateDocument& cd, const std::vector<std::string>& query) {
    // NOTE:
    // Temporary approach for testing.
    // The index and processed query are rebuilt locally,
    // and this is obviously not efficient.
    // Once the storage and service layers are integrated,
    // these will not be required anymore.
    std::vector<std::string> rawContent = readFromDocs("dummy-data");
    std::vector<std::string> normalizedContent = normalizeDocs(rawContent);
    std::vector<std::vector<std::string>> tokenizedContent = tokenizeDocs(normalizedContent);
    InvertedIndex invidx = buildInvertedIndex(tokenizedContent);
    // ---

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
        const double totalTermsInDoc = static_cast<double>(tokenizedContent[posting.docId - 1].size());

        const double TF = totalTermsInDoc > 0 ? (countOfTermInDoc / totalTermsInDoc) : 0.0;

        // IDF
        const double numberOfDocumentsInTheCorpus = static_cast<double>(tokenizedContent.size());
        const double numberOfDocumentsContainingTerm =
            (it != invidx.index.end()) ? static_cast<double>(it->second.entries.size()) : 0.0;

        // for future ref: https://en.cppreference.com/cpp/numeric/math/log
        if (numberOfDocumentsContainingTerm > 0) {
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
