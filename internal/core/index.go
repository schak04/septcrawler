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

func BuildInvertedIndex() InvertedIndex {
	cIndex := C.build_inverted_index()
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
