#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "bridge/c-api.h"
#include "core-components/index-builder.hpp"
#include "core-components/query-processor.hpp"
#include "core-components/ranker.hpp"
#include "core-components/retrieval-engine.hpp"
#include "data-structures/candidate-doc.hpp"
#include "data-structures/inverted-index.hpp"
#include "data-structures/posting-list.hpp"
#include "data-structures/posting.hpp"

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

InvertedIndexC build_inverted_index(void) {
    InvertedIndexC result;
    result.entries = nullptr;
    result.entryCount = 0;

    std::vector<std::string> rawContent = readFromDocs("dummy-data");
    if (rawContent.empty()) {
        return result;
    }

    std::vector<std::string> normalizedContent = normalizeDocs(rawContent);
    std::vector<std::vector<std::string>> tokenizedContent = tokenizeDocs(normalizedContent);
    InvertedIndex invidx = buildInvertedIndex(tokenizedContent);

    if (invidx.index.empty()) {
        return result;
    }

    size_t entryCount = invidx.index.size();
    auto* entries =
        static_cast<InvertedIndexEntryC*>(std::malloc(sizeof(InvertedIndexEntryC) * entryCount));
    if (entries == nullptr) {
        return result;
    }

    size_t entryIdx = 0;
    for (const auto& pair : invidx.index) {
        const std::string& term = pair.first;
        const PostingList& pList = pair.second;

        char* termStr = static_cast<char*>(std::malloc(term.length() + 1));
        if (termStr != nullptr) {
            std::memcpy(termStr, term.c_str(), term.length() + 1);
        }
        entries[entryIdx].term = termStr;

        PostingListC& pListC = entries[entryIdx].postingList;
        pListC.totalFrequency = pList.totalFrequency;
        pListC.entryCount = pList.entries.size();

        if (pListC.entryCount > 0) {
            pListC.entries =
                static_cast<PostingC*>(std::malloc(sizeof(PostingC) * pListC.entryCount));
            for (size_t pIdx = 0; pIdx < pListC.entryCount; ++pIdx) {
                const Posting& p = pList.entries[pIdx];
                PostingC& pC = pListC.entries[pIdx];

                pC.docId = p.docId;
                pC.termFrequency = p.termFrequency;
                pC.positionCount = p.positions.size();

                if (pC.positionCount > 0) {
                    pC.positions = static_cast<int*>(std::malloc(sizeof(int) * pC.positionCount));
                    for (size_t posIdx = 0; posIdx < pC.positionCount; ++posIdx) {
                        pC.positions[posIdx] = p.positions[posIdx];
                    }
                } else {
                    pC.positions = nullptr;
                }
            }
        } else {
            pListC.entries = nullptr;
        }

        entryIdx++;
    }

    result.entries = entries;
    result.entryCount = entryCount;
    return result;
}

void free_inverted_index(InvertedIndexC index) {
    if (index.entries == nullptr) {
        return;
    }

    for (size_t i = 0; i < index.entryCount; ++i) {
        InvertedIndexEntryC& entry = index.entries[i];
        if (entry.term != nullptr) {
            std::free(entry.term);
        }

        if (entry.postingList.entries != nullptr) {
            for (size_t j = 0; j < entry.postingList.entryCount; ++j) {
                PostingC& pC = entry.postingList.entries[j];
                if (pC.positions != nullptr) {
                    std::free(pC.positions);
                }
            }
            std::free(entry.postingList.entries);
        }
    }

    std::free(index.entries);
}
}
