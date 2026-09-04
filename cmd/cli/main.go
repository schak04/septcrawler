package main

import (
	"fmt"
	"log"
	"os"
	"strings"

	"septcrawler/internal/core"
	"septcrawler/internal/storage"
)

func main() {
	if len(os.Args) < 2 {
		log.Fatal("Usage: ./bin/septcrawler-cli <query>")
	}

	rawQuery := strings.Join(os.Args[1:], " ")

	index, err := storage.ReadInvertedIndex()
	if err != nil {
		log.Fatal("Could not read persistent inverted index: ", err)
	}

	core.LoadInvertedIndex(index)

	results := core.Search(rawQuery)

	if len(results) == 0 {
		log.Fatal("No results found for query: ", rawQuery, "\nSorry, nerd.")
	}

	storedDocs, err := storage.ReadDocuments()
	if err != nil {
		log.Fatal("Error while reading documents: ", err)
	}

	docMap := make(map[int]string)
	for idx, doc := range storedDocs {
		docMap[idx+1] = doc.URL
	}

	fmt.Println("Search results for:", rawQuery)

	for _, doc := range results {
		urlString := ""
		if u, ok := docMap[doc.DocID]; ok {
			urlString = fmt.Sprintf(" | URL: %s", u)
		}
		fmt.Printf("docId: %d%s | matched terms: %d\n", doc.DocID, urlString, doc.MatchedTermsCount)
	}
}
