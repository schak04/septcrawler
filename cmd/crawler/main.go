package main

import (
	"septcrawler/internal/crawler"
)

// target URLs slice
// only one target for now until I get the crawler working
var targetURLs []string = []string{"https://en.cppreference.com"}

func main() {
	var httpFetcher crawler.HTTPFetcher
	crawler.Crawl(targetURLs[0], 5, httpFetcher)
}
