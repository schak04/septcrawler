package main

import (
	"fmt"
	"log"

	"septcrawler/internal/parser"
	"septcrawler/internal/storage"
)

func main() {
	parsedDocuments := parser.GetParsedDocuments()

	if err := storage.StoreDocuments(parsedDocuments); err != nil {
		log.Fatal(err)
	}
	fmt.Println("Documents stored successfully.")

	if err := storage.StoreInvertedIndex(); err != nil {
		log.Fatal(err)
	}
	fmt.Println("Inverted index stored successfully.")
}
