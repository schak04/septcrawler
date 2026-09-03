package api

import (
	"encoding/json"
	"net/http"
	"net/http/httptest" // provides tools for testing HTTP handlers without starting a real HTTP server
	"os"
	"testing"

	"septcrawler/internal/core"
	"septcrawler/internal/storage"
)

func init() {
	if _, err := os.Stat("dummy-data"); os.IsNotExist(err) {
		_ = os.Chdir("../../")
	}
	sampleDocs := []string{
		"cmake is a build system.",
		"GNU Debugger gdb is used for debugging.",
		"GNU Debugger is powerful.",
	}
	idx, err := storage.ReadInvertedIndex()
	if err != nil || len(idx) == 0 {
		_ = storage.StoreInvertedIndex(sampleDocs)
		idx, _ = storage.ReadInvertedIndex()
	}
	core.LoadInvertedIndex(idx)
}

func TestSearchHandler(t *testing.T) {
	req := httptest.NewRequest("GET", "/search?q=GNU+Debugger", nil)
	w := httptest.NewRecorder()

	searchHandler(w, req)

	resp := w.Result()
	if resp.StatusCode != http.StatusOK {
		t.Fatalf("expected status 200 OK, got %d", resp.StatusCode)
	}

	var data Response
	if err := json.NewDecoder(resp.Body).Decode(&data); err != nil {
		t.Fatalf("failed to decode response: %v", err)
	}

	if data.Query != "GNU Debugger" {
		t.Errorf("expected query 'GNU Debugger', got %q", data.Query)
	}

	if len(data.Results) != 2 {
		t.Fatalf("expected 2 results, got %d", len(data.Results))
	}

	if data.Results[0].DocID != 3 || data.Results[1].DocID != 2 {
		t.Errorf("unexpected results order: %v", data.Results)
	}
}

func TestSearchHandlerMissingQuery(t *testing.T) {
	req := httptest.NewRequest("GET", "/search", nil)
	w := httptest.NewRecorder()

	searchHandler(w, req)

	resp := w.Result()
	if resp.StatusCode != http.StatusBadRequest {
		t.Errorf("expected status 400 Bad Request for missing query, got %d", resp.StatusCode)
	}
}
