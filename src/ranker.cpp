/*
 * TODO:
 * understand TF-IDF
 * design and plan the algo for ranking on paper (based off of TF-IDF)
 * implement it
 * */

#include "../headers/core-components/ranker.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

#include "../headers/core-components/retrieval-engine.hpp"
#include "../headers/data-structures/candidate-doc.hpp"

int calcScore(const CandidateDocument& cd) { return cd.matchedTermsCount; }

std::vector<CandidateDocument> rankCandidateDocuments(const std::vector<CandidateDocument>& unrankedCandidateDocs) {
    std::vector<CandidateDocument> rankedCandidateDocs = unrankedCandidateDocs;

    std::sort(rankedCandidateDocs.begin(), rankedCandidateDocs.end(), [](const CandidateDocument& a, const CandidateDocument& b) { return calcScore(a) > calcScore(b); });

    return rankedCandidateDocs;
}

int runRanker() {
    std::cout << "\nRUNNING RANKER...\n";

    std::vector<CandidateDocument> unranked = generateCandidateDocuments();
    std::vector<CandidateDocument> ranked = rankCandidateDocuments(unranked);

    for (const auto& rcd : ranked) {
        std::cout << "docId: " << rcd.docId << " | matched terms count: " << rcd.matchedTermsCount << '\n';
    }

    return 0;
}
