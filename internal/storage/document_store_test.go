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

func TestReadDocuments(t *testing.T) {
	parsedDocs := []parser.ParsedDocument{
		{URL: "https://example.com/1", Content: "Doc 1 content"},
		{URL: "https://example.com/2", Content: "Doc 2 content"},
	}

	if err := StoreDocuments(parsedDocs); err != nil {
		t.Fatalf("StoreDocuments failed: %v", err)
	}

	docs, err := ReadDocuments()
	if err != nil {
		t.Fatalf("ReadDocuments failed: %v", err)
	}

	if len(docs) < 2 {
		t.Fatalf("expected at least 2 documents read, got %d", len(docs))
	}

	if docs[0].URL != "https://example.com/1" || docs[1].URL != "https://example.com/2" {
		t.Errorf("unexpected read documents URLs: %v, %v", docs[0].URL, docs[1].URL)
	}
}
