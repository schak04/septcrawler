/*
SeptCrawler: A search engine for learning-focused retrieval.
Author: Saptaparno Chakraborty
*/

#include <iostream>
#include <vector>

#include "core-components/query-processor.hpp"
#include "core-components/ranker.hpp"
#include "core-components/retrieval-engine.hpp"
#include "data-structures/candidate-doc.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./bin/septcrawler-cli <query>\n";
        std::cout << "Enclose the query within \"\" if it has whitespaces in between.\n";
        return 1;
    }

    std::string rawQuery = argv[1];
    std::vector<std::string> queryTokens = processQuery(rawQuery);

    if (queryTokens.empty()) {
        std::cout << "What are you tryna look up? That does NOT look like a valid search query.\n";
        std::cout << "Try again, nerd. (in a friendly tone btw)\n";
        return 0;
    }

    std::vector<CandidateDocument> candidateDocs = generateCandidateDocuments(queryTokens);
    std::vector<CandidateDocument> rankedDocs = rankCandidateDocuments(candidateDocs, queryTokens);

    if (rankedDocs.empty()) {
        std::cout << "No results found for query: " << rawQuery << '\n';
        std::cout << "Sorry, pal." << '\n';
        return 0;
    }

    std::cout << "Search results for: " << rawQuery << '\n';
    for (const auto& doc : rankedDocs) {
        std::cout << "docId: " << doc.docId << " | matched terms: " << doc.matchedTermsCount
                  << " | TF-IDF score: " << calcTFIDFScore(doc, queryTokens) << '\n';
    }

    return 0;
}
