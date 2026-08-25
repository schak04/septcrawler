package api

import (
	"fmt"
	"log"
	"net/http"
)

var port = ":8080"

type Server struct{}

func (server *Server) Start() {
	fmt.Println("Server listening on port", port)
	log.Fatal(http.ListenAndServe(port, nil))
}
