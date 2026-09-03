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

type Posting struct {
	DocID         int   `json:"docId"`
	TermFrequency int   `json:"termFrequency"`
	Positions     []int `json:"positions"`
}

type PostingList struct {
	Entries        []Posting `json:"entries"`
	TotalFrequency int       `json:"totalFrequency"`
}

type InvertedIndex map[string]PostingList

func BuildInvertedIndex(docs []string) InvertedIndex {
	if len(docs) == 0 {
		return InvertedIndex{}
	}

	cDocs := make([]*C.char, len(docs))
	for i, doc := range docs {
		cDocs[i] = C.CString(doc)
		defer C.free(unsafe.Pointer(cDocs[i]))
	}

	cIndex := C.build_inverted_index((**C.char)(unsafe.Pointer(&cDocs[0])), C.size_t(len(docs)))
	defer C.free_inverted_index(cIndex)

	if cIndex.entryCount == 0 || cIndex.entries == nil {
		return InvertedIndex{}
	}

	result := make(InvertedIndex, int(cIndex.entryCount))
	cEntries := unsafe.Slice(cIndex.entries, int(cIndex.entryCount))

	for _, entry := range cEntries {
		term := C.GoString(entry.term)
		cPostingList := entry.postingList

		postingList := PostingList{
			TotalFrequency: int(cPostingList.totalFrequency),
			Entries:        make([]Posting, int(cPostingList.entryCount)),
		}

		if cPostingList.entryCount > 0 && cPostingList.entries != nil {
			cPostings := unsafe.Slice(cPostingList.entries, int(cPostingList.entryCount))
			for j, p := range cPostings {
				posting := Posting{
					DocID:         int(p.docId),
					TermFrequency: int(p.termFrequency),
				}
				if p.positionCount > 0 && p.positions != nil {
					cPositions := unsafe.Slice(p.positions, int(p.positionCount))
					positions := make([]int, int(p.positionCount))
					for k, pos := range cPositions {
						positions[k] = int(pos)
					}
					posting.Positions = positions
				} else {
					posting.Positions = []int{}
				}
				postingList.Entries[j] = posting
			}
		}

		result[term] = postingList
	}

	return result
}

func LoadInvertedIndex(index InvertedIndex) {
	// the index is already represented as Go data
	// construct a temporary C-compatible representation so the C++ search core
	// can consume the index through the C API
	if len(index) == 0 {
		return
	}

	// allocate C heap memory for an array of InvertedIndexEntryC objects
	// unsafe.Sizeof gives the size of one C.InvertedIndexEntryC object in bytes
	entryCount := len(index)
	cEntries := C.malloc(C.size_t(entryCount) * C.size_t(unsafe.Sizeof(C.InvertedIndexEntryC{})))

	// create a Go slice that provides convenient access to the C-allocated memory
	// unsafe.Slice does not allocate new memory, it creates a Go view over the existing C memory
	entriesSlice := unsafe.Slice((*C.InvertedIndexEntryC)(cEntries), entryCount)

	i := 0
	for term, plist := range index {
		cTerm := C.CString(term) // Go string -> null-terminated C string
		entriesSlice[i].term = cTerm

		entriesSlice[i].postingList.totalFrequency = C.int(plist.TotalFrequency)

		// since each PostingList contains an array of postings, one for each document containing the term
		pCount := len(plist.Entries)
		entriesSlice[i].postingList.entryCount = C.size_t(pCount)

		if pCount > 0 {
			// allocate C heap memory for the PostingC array belonging to this term's posting list
			cPostings := C.malloc(C.size_t(pCount) * C.size_t(unsafe.Sizeof(C.PostingC{})))
			postingsSlice := unsafe.Slice((*C.PostingC)(cPostings), pCount)

			for j, p := range plist.Entries {
				postingsSlice[j].docId = C.int(p.DocID)
				postingsSlice[j].termFrequency = C.int(p.TermFrequency)

				posCount := len(p.Positions) // each posting contains its own array of term positions
				postingsSlice[j].positionCount = C.size_t(posCount)

				if posCount > 0 {
					cPositions := C.malloc(C.size_t(posCount) * C.size_t(unsafe.Sizeof(C.int(0))))
					posSlice := unsafe.Slice((*C.int)(cPositions), posCount)
					for k, pos := range p.Positions {
						posSlice[k] = C.int(pos)
					}
					postingsSlice[j].positions = (*C.int)(cPositions)
				} else {
					postingsSlice[j].positions = nil
				}
			}
			entriesSlice[i].postingList.entries = (*C.PostingC)(cPostings)
		} else {
			entriesSlice[i].postingList.entries = nil
		}
		i++
	}

	// assemble the C-compatible inverted index from the C-allocated entries array
	// so it can be passed to the C API
	cIndex := C.InvertedIndexC{
		entries:    (*C.InvertedIndexEntryC)(cEntries),
		entryCount: C.size_t(entryCount),
	}

	// the bridge converts the C-compatible representation into
	// the native C++ InvertedIndex
	// and loads it into the search core's in-memory index
	C.load_inverted_index(cIndex)

	C.free_inverted_index(cIndex) // since the C++ core has already copied the data into its own InvertedIndex
}
