package storage

import (
	"os"
	"path/filepath"
	"testing"
)

func TestStoreAndReadInvertedIndex(t *testing.T) {
	sampleDocs := []string{
		"cmake is a build system.",
		"GNU Debugger gdb is used for debugging.",
	}
	if err := StoreInvertedIndex(sampleDocs); err != nil {
		t.Fatalf("StoreInvertedIndex failed: %v", err)
	}

	indexPath := filepath.Join(indexDestination, indexFilename)
	if _, err := os.Stat(indexPath); os.IsNotExist(err) {
		t.Fatalf("expected index file at %s", indexPath)
	}

	idx, err := ReadInvertedIndex()
	if err != nil {
		t.Fatalf("ReadInvertedIndex failed: %v", err)
	}

	if len(idx) == 0 {
		t.Fatal("expected non-empty index from ReadInvertedIndex")
	}

	if _, ok := idx["cmake"]; !ok {
		t.Errorf("expected 'cmake' in read inverted index")
	}
}
