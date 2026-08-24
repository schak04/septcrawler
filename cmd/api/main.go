package main

import "septcrawler/internal/api"

func main() {
	server := api.Server{}
	server.Start()
}
