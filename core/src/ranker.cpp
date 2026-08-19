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
#include <iostream>
#include <string>
#include <vector>

#include "core-components/index-builder.hpp"
#include "core-components/query-processor.hpp"
#include "core-components/retrieval-engine.hpp"
#include "data-structures/candidate-doc.hpp"
#include "data-structures/inverted-index.hpp"
#include "data-structures/posting-list.hpp"
#include "data-structures/posting.hpp"

double calcTFIDFScore(const CandidateDocument& cd) {
    // NOTE:
    // Temporary approach for testing.
    // The index and processed query are rebuilt locally,
    // and this is obviously not efficient.
    // Once the storage and service layers are integrated,
    // these will not be required anymore.
    const std::vector<std::string> query = getProcessedQuery();
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
        for (const Posting& p : invidx.index[term].entries) {
            if (p.docId == cd.docId) {
                posting = p;
                break;
            }
        }

        const double countOfTermInDoc = posting.termFrequency;
        const double totalTermsInDoc = tokenizedContent[posting.docId - 1].size();

        const double TF = countOfTermInDoc / totalTermsInDoc;

        // IDF
        const double numberOfDocumentsInTheCorpus = static_cast<double>(tokenizedContent.size());
        const double numberOfDocumentsContainingTerm =
            static_cast<double>(invidx.index[term].entries.size());

        // for future ref: https://en.cppreference.com/cpp/numeric/math/log
        const double IDF = std::log(numberOfDocumentsInTheCorpus / numberOfDocumentsContainingTerm);

        // TF-IDF
        const double TFIDFScoreForQueryTerm = TF * IDF;
        TFIDFScoreForCandidateDoc += TFIDFScoreForQueryTerm;
    }

    return TFIDFScoreForCandidateDoc;
}

std::vector<CandidateDocument> rankCandidateDocuments(
    const std::vector<CandidateDocument>& unrankedCandidateDocs) {
    std::vector<CandidateDocument> rankedCandidateDocs = unrankedCandidateDocs;

    std::sort(rankedCandidateDocs.begin(), rankedCandidateDocs.end(),
              [](const CandidateDocument& a, const CandidateDocument& b) {
                  return calcTFIDFScore(a) > calcTFIDFScore(b);
              });

    return rankedCandidateDocs;
}

int runRanker() {
    std::cout << "\nRUNNING RANKER...\n";

    std::vector<CandidateDocument> unranked = generateCandidateDocuments();
    std::vector<CandidateDocument> ranked = rankCandidateDocuments(unranked);

    // NOTE:
    // calcTFIDFScore() rebuilds the index and processed query on each call.
    // This is temporary for testing. Once the storage layer is integrated,
    // scores will be computed from the shared index instead.
    for (const auto& rcd : ranked) {
        std::cout << "docId: " << rcd.docId << " | matched terms count: " << rcd.matchedTermsCount
                  << " | TF-IDF score: " << calcTFIDFScore(rcd) << '\n';
    }

    return 0;
}
