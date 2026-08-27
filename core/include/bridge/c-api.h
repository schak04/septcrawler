// C-compatible API exposed to Go via cgo
// Declarations only; implementation is in bridge.cpp

#ifndef SEPTCRAWLER_BRIDGE_C_API_H
#define SEPTCRAWLER_BRIDGE_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int doc_id;
    int matched_terms_count;
} CandidateDocumentC;

typedef struct {
    CandidateDocumentC* documents;
    int count;
} SearchResponseC;

SearchResponseC search_query(const char* query);
void free_search_response(SearchResponseC response);

#ifdef __cplusplus
}
#endif

#endif
