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

	docContents := make([]string, len(parsedDocuments))
	for i, doc := range parsedDocuments {
		docContents[i] = doc.Content
	}

	if err := storage.StoreInvertedIndex(docContents); err != nil {
		log.Fatal(err)
	}
	fmt.Println("Inverted index stored successfully.")
}
