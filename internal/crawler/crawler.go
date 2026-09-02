package crawler

import (
	"errors"
	"fmt"
	"io"
	"net/http"
	"net/url"
	"strings"
	"sync"

	"golang.org/x/net/html"
)

type Fetcher interface {
	Fetch(url string) (body string, urlsFound []string, err error)
}

type HTTPFetcher struct{}

// extracts URLs from <a href="..."> elements
// baseURL needed since links extracted from a page can be relative
func ExtractLinks(page io.Reader, baseURL string) ([]string, error) {
	doc, err := html.Parse(page) // https://pkg.go.dev/golang.org/x/net/html
	if err != nil {
		return nil, err
	}

	parsedBaseURL, err := url.Parse(baseURL) // https://pkg.go.dev/net/url
	if err != nil {
		return nil, err
	}

	var urlsFound []string

	// recursively traverse the HTML's node (since HTML is represented as a tree)
	// https://pkg.go.dev/golang.org/x/net/html#Node
	var traverseHTML func(*html.Node)

	traverseHTML = func(node *html.Node) {
		if node.Type == html.ElementNode && node.Data == "a" {
			for _, attr := range node.Attr {
				if attr.Key != "href" {
					continue
				}

				refURLfromHrefAttribute, err := url.Parse(attr.Val)
				if err != nil {
					continue
				}

				// since href can be relative, resolve it against the page url
				resolvedURL := parsedBaseURL.ResolveReference(refURLfromHrefAttribute)

				if resolvedURL.Scheme == "http" || resolvedURL.Scheme == "https" {
					urlsFound = append(urlsFound, resolvedURL.String())
				}
			}
		}

		for child := node.FirstChild; child != nil; child = child.NextSibling {
			traverseHTML(child)
		}
	}

	traverseHTML(doc)

	return urlsFound, nil
}

// URL -> page body + a slice of URLs found on that page
func (httpFetcher HTTPFetcher) Fetch(url string) (string, []string, error) {
	res, err := http.Get(url) // https://pkg.go.dev/net/http
	if err != nil {
		return "", nil, err
	}
	defer res.Body.Close()

	if res.StatusCode < 200 || res.StatusCode >= 300 {
		errorMsg := fmt.Sprintf("HTTP request failed with status: %s", res.Status)
		return "", nil, errors.New(errorMsg)
	}

	// only process HTML for now
	contentType := res.Header.Get("Content-Type")
	if !strings.HasPrefix(contentType, "text/html") {
		return "", nil, errors.New("Response is not HTML")
	}

	byteSliceBody, err := io.ReadAll(res.Body) // https://pkg.go.dev/io
	if err != nil {
		return "", nil, err
	}
	body := string(byteSliceBody)

	urlsFound, err := ExtractLinks(strings.NewReader(body), url) // https://pkg.go.dev/strings#NewReader
	if err != nil {
		return body, nil, err
	}

	return body, urlsFound, nil
}

var urlVisitedMap = make(map[string]bool) // url -> fetched already or not
var wg sync.WaitGroup
var mu sync.Mutex

type CrawledPage struct {
	URL  string
	HTML string
}

var CrawledPages = make([]CrawledPage, 0)

func Crawl(url string, depth int, fetcher Fetcher) {
	if depth <= 0 {
		return
	}

	var body string
	var urls []string
	var err error

	mu.Lock()
	if _, present := urlVisitedMap[url]; present {
		mu.Unlock()
		return
	}

	urlVisitedMap[url] = true
	mu.Unlock()

	body, urls, err = fetcher.Fetch(url)
	if err != nil {
		fmt.Println(err)
		return
	}

	mu.Lock()
	CrawledPages = append(CrawledPages, CrawledPage{URL: url, HTML: body})
	mu.Unlock()

	for _, u := range urls {
		wg.Add(1)

		go func() {
			defer wg.Done()
			Crawl(u, depth-1, fetcher)
		}()
	}
}

func StartCrawling(url string, depth int, fetcher Fetcher) {
	wg.Add(1)

	go func() {
		defer wg.Done()
		Crawl(url, depth, fetcher)
	}()

	wg.Wait()
}
