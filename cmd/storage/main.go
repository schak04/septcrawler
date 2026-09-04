package main

import (
	"fmt"
	"log"
	"os"
	"strconv"

	"septcrawler/internal/crawler"
	"septcrawler/internal/parser"
	"septcrawler/internal/storage"
)

func main() {
	targetURL := "https://en.cppreference.com"
	depth := 2

	if len(os.Args) > 1 {
		targetURL = os.Args[1]
	}
	if len(os.Args) > 2 {
		if d, err := strconv.Atoi(os.Args[2]); err == nil && d > 0 {
			depth = d
		}
	}

	fmt.Printf("Starting crawler for %s (depth: %d)...\n", targetURL, depth)
	var httpFetcher crawler.HTTPFetcher
	crawler.StartCrawling(targetURL, depth, httpFetcher)
	fmt.Printf("Crawling complete. Crawled %d pages.\n", len(crawler.CrawledPages))

	parsedDocuments := parser.GetParsedDocuments()
	fmt.Printf("Parsed %d documents.\n", len(parsedDocuments))

	if len(parsedDocuments) == 0 {
		log.Fatal("No documents were crawled or parsed. Aborting the process because there's no point in continuing without any documents.")
	}

	if err := storage.StoreDocuments(parsedDocuments); err != nil {
		log.Fatal("Failed to store documents: ", err)
	}
	fmt.Println("Documents stored successfully.")

	docContents := make([]string, len(parsedDocuments))
	for i, doc := range parsedDocuments {
		docContents[i] = doc.Content
	}

	if err := storage.StoreInvertedIndex(docContents); err != nil {
		log.Fatal("Failed to store inverted index: ", err)
	}
	fmt.Println("Inverted index stored successfully.")
}
