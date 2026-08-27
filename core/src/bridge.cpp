#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "bridge/c-api.h"
#include "core-components/query-processor.hpp"
#include "core-components/ranker.hpp"
#include "core-components/retrieval-engine.hpp"
#include "data-structures/candidate-doc.hpp"

extern "C" {

SearchResponseC search_query(const char* query) {
    SearchResponseC response;
    response.documents = nullptr;
    response.count = 0;

    if (query == nullptr || std::strlen(query) == 0) {
        return response;
    }

    std::string rawQuery(query);
    std::vector<std::string> queryTokens = processQuery(rawQuery);

    if (queryTokens.empty()) {
        return response;
    }

    std::vector<CandidateDocument> candidateDocs = generateCandidateDocuments(queryTokens);
    if (candidateDocs.empty()) {
        return response;
    }

    std::vector<CandidateDocument> rankedDocs = rankCandidateDocuments(candidateDocs, queryTokens);
    if (rankedDocs.empty()) {
        return response;
    }

    int count = static_cast<int>(rankedDocs.size());
    auto* docs = static_cast<CandidateDocumentC*>(std::malloc(sizeof(CandidateDocumentC) * count));
    if (docs == nullptr) {
        return response;
    }

    for (int i = 0; i < count; ++i) {
        docs[i].doc_id = rankedDocs[i].docId;
        docs[i].matched_terms_count = rankedDocs[i].matchedTermsCount;
    }

    response.documents = docs;
    response.count = count;
    return response;
}

void free_search_response(SearchResponseC response) {
    if (response.documents != nullptr) {
        std::free(response.documents);
    }
}
}
