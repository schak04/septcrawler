/*
 * NOTE:
 * scoring and ranking implemented using TF-IDF (draft, not completely tested yet)
 *
 * TODO:
 * replace dummy data with real data once service and storage layers are implemented
 * */

#include "../headers/core-components/ranker.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "../headers/core-components/index-builder.hpp"
#include "../headers/core-components/query-processor.hpp"
#include "../headers/core-components/retrieval-engine.hpp"
#include "../headers/data-structures/candidate-doc.hpp"
#include "../headers/data-structures/inverted-index.hpp"
#include "../headers/data-structures/posting-list.hpp"
#include "../headers/data-structures/posting.hpp"

// NOTE: scoring algorithm for v1

double calcTFIDFScore(const CandidateDocument& cd) {
    const std::vector<std::string> query = getProcessedQuery();
    std::vector<std::string> rawContent = readFromDocs("dummy-data");
    std::vector<std::string> normalizedContent = normalizeDocs(rawContent);
    std::vector<std::vector<std::string>> tokenizedContent = tokenizeDocs(normalizedContent);
    InvertedIndex invidx = buildInvertedIndex(tokenizedContent);

    double TFIDFScoreForCandidateDoc = 0;

    for (const std::string& term : query) {
        // TF
        Posting posting;
        for (const Posting& p : invidx.index[term].entries) {
            if (p.docId == cd.docId) {
                posting = p;
                break;
            }
        }

        int countOfTermInDoc = posting.termFrequency;
        int totalTermsInDoc = 0;
        for (const Posting& p : invidx.index[term].entries) {
            if (p.docId == cd.docId) {
                totalTermsInDoc += p.termFrequency;
            }
        }

        double TF = countOfTermInDoc / totalTermsInDoc;

        // IDF
        int numberOfDocumentsInTheCorpus = tokenizedContent.size();
        int numberOfDocumentsContainingTerm = invidx.index[term].entries.size();

        // for future ref: https://en.cppreference.com/cpp/numeric/math/log
        double IDF = std::log(numberOfDocumentsInTheCorpus / numberOfDocumentsContainingTerm);

        // TF-IDF
        double TFIDFScoreForQueryTerm = TF * IDF;
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

    for (const auto& rcd : ranked) {
        std::cout << "docId: " << rcd.docId << " | matched terms count: " << rcd.matchedTermsCount
                  << '\n';
    }

    return 0;
}
