package crawler

import (
	"errors"
	"fmt"
	"io"
	"net/http"
	"strings"
)

type Fetcher interface {
	Fetch(url string) (body string, urlsFound []string, err error)
}

type HTTPFetcher struct{}

func ExtractLinks(page io.Reader) ([]string, error) {
	// this is just so my LSP doesn't throw errors at my face while I'm yet to write the function
	return []string{}, nil
}

// URL -> page body + a slice of URLs found on that page
func (httpFetcher HTTPFetcher) Fetch(url string) (string, []string, error) {
	res, err := http.Get(url)
	if err != nil {
		return "", []string{}, err
	}
	defer res.Body.Close()

	if res.StatusCode >= 400 && res.StatusCode < 600 {
		errorMsg := fmt.Sprintf("Error occurred: %v", res.StatusCode)
		return "", []string{}, errors.New(errorMsg)
	}

	byteSliceBody, err := io.ReadAll(res.Body)
	if err != nil {
		return "", []string{}, err
	}
	body := string(byteSliceBody)

	urlsFound, err := ExtractLinks(strings.NewReader(body)) // https://pkg.go.dev/strings#NewReader
	if err != nil {
		return body, []string{}, err
	}

	return body, urlsFound, nil
}

func Crawl(url string, depth int, fetcher Fetcher) {
	// TODO: just fetch urls and recursively crawl first
	// no need to worry about doing so in parallel yet
	// TODO: once the basic functionality is done,
	// implement parallel crawling + don't crawl already crawled urls
	if depth <= 0 {
		return
	}

	var body string
	var urls []string
	var err error

	body, urls, err = fetcher.Fetch(url)
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println("Found:")
	fmt.Println("URL:", url)
	fmt.Println("Body:", body)

	for _, u := range urls {
		Crawl(u, depth-1, fetcher)
	}
}
