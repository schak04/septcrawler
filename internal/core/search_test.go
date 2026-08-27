package core

import (
	"testing"
)

func TestSearch(t *testing.T) {
	results := Search("GNU Debugger")
	if len(results) != 2 {
		t.Fatalf("expected 2 results, got %d", len(results))
	}

	if results[0].DocID != 3 || results[1].DocID != 2 {
		t.Errorf("expected docId 3 followed by 2, got %v", results)
	}

	singleTerm := Search("cmake")
	if len(singleTerm) != 1 {
		t.Fatalf("expected 1 result for cmake, got %d", len(singleTerm))
	}
	if singleTerm[0].DocID != 1 {
		t.Errorf("expected docId 1 for cmake, got %d", singleTerm[0].DocID)
	}

	empty := Search("")
	if len(empty) != 0 {
		t.Errorf("expected 0 results for empty query, got %d", len(empty))
	}

	noMatch := Search("nonexistentterm123")
	if len(noMatch) != 0 {
		t.Errorf("expected 0 results for nonexistent term, got %d", len(noMatch))
	}
}
