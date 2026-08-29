package main

import (
	"fmt"
	"os"
	"strings"

	"septcrawler/internal/core"
	"septcrawler/internal/storage"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: ./bin/septcrawler-cli <query>")
		fmt.Println(`No need to enclose the query within "" if it has whitespaces in between.`)
		return
	}

	rawQuery := strings.Join(os.Args[1:], " ")

	index, err := storage.ReadInvertedIndex()
	if err != nil {
		fmt.Println("Warning: Could not read persistent inverted index:", err)
		return
	}

	core.LoadInvertedIndex(index)

	results := core.Search(rawQuery)

	if len(results) == 0 {
		fmt.Println("No results found for query:", rawQuery)
		fmt.Println("Sorry, nerd.")
		return
	}

	fmt.Println("Search results for:", rawQuery)

	for _, doc := range results {
		fmt.Printf("docId: %d | matched terms: %d\n",
			doc.DocID,
			doc.MatchedTermsCount,
		)
	}
}
