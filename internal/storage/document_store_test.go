package storage

import (
	"encoding/json"
	"os"
	"path/filepath"
	"testing"
)

func init() {
	if _, err := os.Stat("dummy-data"); os.IsNotExist(err) {
		_ = os.Chdir("../../")
	}
}

func TestStoreData(t *testing.T) {
	if err := StoreData(); err != nil {
		t.Fatalf("StoreData failed: %v", err)
	}

	doc1Path := filepath.Join(destination, "doc1.json")
	content, err := os.ReadFile(doc1Path)
	if err != nil {
		t.Fatalf("failed to read stored doc1.json: %v", err)
	}

	var doc Document
	if err := json.Unmarshal(content, &doc); err != nil {
		t.Fatalf("failed to unmarshal doc1.json: %v", err)
	}

	if doc.DocId != 1 {
		t.Errorf("expected docId 1, got %d", doc.DocId)
	}

	if len(doc.Content) == 0 {
		t.Errorf("expected non-empty content for doc1")
	}
}

func TestReadData(t *testing.T) {
	data, err := ReadData(filepath.Join(source, "doc1.txt"))
	if err != nil {
		t.Fatalf("ReadData failed: %v", err)
	}

	if len(data) == 0 {
		t.Errorf("expected non-empty data from ReadData")
	}
}
