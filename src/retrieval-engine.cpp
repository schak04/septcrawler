/*
 * TODO: receive normalized and tokenized queries from the query processor
 * look up postings from the storage layer (will use dummy postings until I finally implement the storage layer)
 * generate candidate docs with some lightweight stats for the ranker
 * send it to the ranker
 * */

#include <iostream>
#include <string>
#include <vector>

#include "../headers/data-structures/candidate-doc.hpp"
#include "../headers/core-components/index-builder.hpp"
#include "../headers/data-structures/inverted-index.hpp"
#include "../headers/data-structures/posting-list.hpp"
#include "../headers/core-components/query-processor.hpp"
#include "../headers/core-components/retrieval-engine.hpp"

// TODO: dummy query processor to retrieval engine connection
std::vector<std::string> processedQuery = getProcessedQuery();

// TODO: replace with real inverted index and real data once the storage layer and API layer are implemented
std::vector<std::string> rawContent = readFromDocs("dummy-data");
std::vector<std::string> normalizedContent = normalizeDocs(rawContent);
std::vector<std::vector<std::string>> tokenizedContent = tokenizeDocs(normalizedContent);
InvertedIndex invidx = buildInvertedIndex(tokenizedContent);

/*
 * TODO: need to look up each query token/term in the inverted index
 * AND query -> find docs present in all posting lists
 * OR query -> find docs present in any of the posting lists
 * DECISION (v1): I'll use AND query semantics.
 * Result -> Candidate Documents
 *
 * NOTE:
 * processed query -> look up posting list for each term
 * -> combine posting lists according to query semantics
 * -> candidate doc IDs + attached stats -> send to ranker
 */

// posting list(s) lookup
std::vector<PostingList> lookupPostingLists() {
    std::vector<PostingList> postingListsBasedOnQueryTokens;
    for (const std::string& queryToken : processedQuery) {
        postingListsBasedOnQueryTokens.push_back(invidx.index[queryToken]);
    }

    return postingListsBasedOnQueryTokens;
}

// candidate docs creation
std::vector<CandidateDocument> generateCandidateDocuments() {
    std::vector<PostingList> postingListsToBeIntersected = lookupPostingLists();

    // WIP: I want docs that appear in all posting lists to produce candidate docs
}

int runRetrievalEngine() {
    std::cout << "RUNNING RETRIEVAL ENGINE...\n";

    std::vector<CandidateDocument> cd = generateCandidateDocuments();

    for (const auto& c : cd) {
        std::cout << c.docId << " " << c.matchedTermsCount << '\n';
    }

    return 0;
}
