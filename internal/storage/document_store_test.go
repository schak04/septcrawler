package storage

import (
	"encoding/json"
	"os"
	"path/filepath"
	"testing"

	"septcrawler/internal/parser"
)

func TestStoreDocuments(t *testing.T) {
	documents := []parser.ParsedDocument{
		{
			URL:     "https://example.com",
			Content: "Example content",
		},
	}

	if err := StoreDocuments(documents); err != nil {
		t.Fatalf("StoreDocuments failed: %v", err)
	}

	docPath := filepath.Join(destination, "doc1.json")

	content, err := os.ReadFile(docPath)
	if err != nil {
		t.Fatalf("failed to read stored document: %v", err)
	}

	var doc Document
	if err := json.Unmarshal(content, &doc); err != nil {
		t.Fatalf("failed to unmarshal document: %v", err)
	}

	if doc.URL != "https://example.com" {
		t.Errorf("expected URL https://example.com, got %s", doc.URL)
	}

	if doc.Content != "Example content" {
		t.Errorf("expected content %q, got %q", "Example content", doc.Content)
	}
}
