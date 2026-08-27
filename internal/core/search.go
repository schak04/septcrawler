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

func Search(query string) []CandidateDocument {
	// Allocate C-string in C heap; must be freed manually
	cQuery := C.CString(query)
	defer C.free(unsafe.Pointer(cQuery))

	cResponse := C.search_query(cQuery)
	// Free C-side allocated array after converting results to Go structs
	defer C.free_search_response(cResponse)

	if cResponse.count <= 0 || cResponse.documents == nil {
		return []CandidateDocument{}
	}

	// unsafe.Slice constructs a Go slice backed by C memory without extra allocation
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
