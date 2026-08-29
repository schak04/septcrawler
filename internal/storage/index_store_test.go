package storage

import (
	"os"
	"path/filepath"
	"testing"
)

func TestStoreAndReadInvertedIndex(t *testing.T) {
	if err := StoreInvertedIndex(); err != nil {
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
