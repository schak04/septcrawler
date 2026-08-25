package api

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
)

const port = ":8080"

type Server struct{}

type Response struct {
	Query string `json:"query"`
}

func searchHandler(w http.ResponseWriter, r *http.Request) {
	query := r.URL.Query().Get("q")

	if query == "" {
		http.Error(w, "missing query", http.StatusBadRequest)
		return
	}

	res := Response{Query: query}
	encoder := json.NewEncoder(w) // future ref: https://pkg.go.dev/encoding/json#Encoder.Encode

	w.Header().Set("Content-Type", "application/json") // future ref: https://pkg.go.dev/net/http#Header.Set
	encoder.Encode(res)

	// TODO: connect api to core
	// slice of Candidate Documents
	// SendSearchQueryAndReceiveResultsFromSearchEngine()
}

func (server *Server) RegisterRoutes() {
	http.HandleFunc("/search", searchHandler)
}

func (server *Server) Start() {
	server.RegisterRoutes()

	fmt.Println("Server listening on port", port)
	log.Fatal(http.ListenAndServe(port, nil))
}
