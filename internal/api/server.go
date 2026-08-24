package api

import (
	"fmt"
	"log"
	"net/http"
)

var PORT = ":8080"

type Server struct{}

func (server *Server) Start() {
	fmt.Println("Server listening on port", PORT)
	log.Fatal(http.ListenAndServe(PORT, nil))
}
