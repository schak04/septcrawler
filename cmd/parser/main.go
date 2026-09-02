package main

import (
	"fmt"

	"septcrawler/internal/crawler"
	"septcrawler/internal/parser"
)

var targetURL = "https://en.cppreference.com"

func main() {
	var httpFetcher crawler.HTTPFetcher

	crawler.StartCrawling(targetURL, 5, httpFetcher)

	parsedDocuments := parser.GetParsedDocuments()

	for _, document := range parsedDocuments {
		fmt.Println("URL:", document.URL)
		fmt.Println("Content:")
		fmt.Println(document.Content)
		fmt.Println("------------------")
	}
}
