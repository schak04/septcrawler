package api

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"

	"septcrawler/internal/core"
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

	encoder := json.NewEncoder(w)                      // future ref: https://pkg.go.dev/encoding/json#Encoder.Encode
	w.Header().Set("Content-Type", "application/json") // future ref: https://pkg.go.dev/net/http#Header.Set
	encoder.Encode(res)
}

func (server *Server) RegisterRoutes() {
	http.HandleFunc("/search", searchHandler)
}

func (server *Server) Start() {
	server.RegisterRoutes()

	fmt.Println("Server listening on port", port)
	log.Fatal(http.ListenAndServe(port, nil))
}
