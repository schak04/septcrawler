package core

/*
#cgo CFLAGS: -I${SRCDIR}/../../core/include
#cgo LDFLAGS: -L${SRCDIR}/../../bin -lseptcrawler_core -lstdc++ -lm
#include "bridge/c-api.h"
#include <stdlib.h>
*/
import "C"
import (
	"unsafe"
)

type CandidateDocument struct {
	DocID             int `json:"docId"`
	MatchedTermsCount int `json:"matchedTermsCount"`
}

// NOTE: C allocates memory for the query and search response.
// The Go wrapper is responsible for freeing C-owned memory.
// (since Go's GC can't manage C-allocated memory)
func Search(query string) []CandidateDocument {
	cQuery := C.CString(query)
	defer C.free(unsafe.Pointer(cQuery))

	cResponse := C.search_query(cQuery)

	defer C.free_search_response(cResponse)

	if cResponse.count <= 0 || cResponse.documents == nil {
		return []CandidateDocument{}
	}

	cDocs := unsafe.Slice(cResponse.documents, int(cResponse.count))
	results := make([]CandidateDocument, int(cResponse.count))

	for i, doc := range cDocs {
		results[i] = CandidateDocument{
			DocID:             int(doc.doc_id),
			MatchedTermsCount: int(doc.matched_terms_count),
		}
	}

	return results
}
