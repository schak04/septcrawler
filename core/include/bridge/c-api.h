// C-compatible API exposed to Go via cgo
// Declarations only; implementation is in bridge.cpp

#ifndef SEPTCRAWLER_BRIDGE_C_API_H
#define SEPTCRAWLER_BRIDGE_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct {
    int doc_id;
    int matched_terms_count;
} CandidateDocumentC;

typedef struct {
    CandidateDocumentC* documents;
    int count;
} SearchResponseC;

typedef struct {
    int docId;
    int termFrequency;
    int* positions;
    size_t positionCount;
} PostingC;

typedef struct {
    PostingC* entries;
    size_t entryCount;
    int totalFrequency;
} PostingListC;

typedef struct {
    char* term;
    PostingListC postingList;
} InvertedIndexEntryC;

typedef struct {
    InvertedIndexEntryC* entries;
    size_t entryCount;
} InvertedIndexC;

SearchResponseC search_query(const char* query);
void free_search_response(SearchResponseC response);

InvertedIndexC build_inverted_index(void);
void free_inverted_index(InvertedIndexC index);
void load_inverted_index(InvertedIndexC index);

#ifdef __cplusplus
}
#endif

#endif
