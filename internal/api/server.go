package api

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"

	"septcrawler/internal/core"
	"septcrawler/internal/storage"
)

const port = ":8080"

type Server struct{}

type Response struct {
	Query   string                   `json:"query"`
	Results []core.CandidateDocument `json:"results"`
}

func searchHandler(w http.ResponseWriter, r *http.Request) {
	query := r.URL.Query().Get("q")

	if query == "" {
		http.Error(w, "missing query", http.StatusBadRequest)
		return
	}

	results := core.Search(query)

	res := Response{
		Query:   query,
		Results: results,
	}

	w.Header().Set("Content-Type", "application/json") // future ref: https://pkg.go.dev/net/http#Header.Set

	// future ref: https://pkg.go.dev/encoding/json#Encoder.Encode
	if err := json.NewEncoder(w).Encode(res); err != nil {
		log.Printf("failed to encode search response: %v", err)
	}
}

func (server *Server) RegisterRoutes() *http.ServeMux {
	mux := http.NewServeMux()
	mux.HandleFunc("/search", searchHandler)
	return mux
}

func (server *Server) Start() {
	mux := server.RegisterRoutes()

	httpServer := &http.Server{
		Addr:    port,
		Handler: mux,
	}

	idx, err := storage.ReadInvertedIndex()
	if err != nil {
		fmt.Println("Warning: Could not read persistent inverted index:", err)
	} else {
		core.LoadInvertedIndex(idx)
		fmt.Println("Loaded persistent inverted index into search core.")
	}

	fmt.Println("Server listening on port", port)
	log.Fatal(httpServer.ListenAndServe())
}
