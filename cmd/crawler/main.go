package main

import (
	"fmt"
	"septcrawler/internal/crawler"
)

// target URLs slice
// only one target for now until I get the crawler working
var targetURLs []string = []string{"https://en.cppreference.com"}

func main() {
	body, err := crawler.Crawl(targetURLs[0])

	if err != nil {
		fmt.Println("Error occurred:", err)
	}

	fmt.Println("Crawled URL!")
	fmt.Println("Body:")
	fmt.Println(body)
}
