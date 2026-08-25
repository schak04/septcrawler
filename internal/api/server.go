package api

import (
	"fmt"
	"log"
	"net/http"
)

const port = ":8080"

type Server struct{}

func searchHandler(w http.ResponseWriter, r *http.Request) {
	query := r.URL.Query().Get("q")
	if query == "" {
		http.Error(w, "missing query", http.StatusBadRequest)
		return
	}

	fmt.Println("Query:", query)

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
