package parser

import (
	"fmt"
	"strings"

	"septcrawler/internal/crawler"

	"golang.org/x/net/html"
)

type ParsedDocument struct {
	URL     string
	Content string
}

func GetParsedDocuments() []ParsedDocument {
	var ParsedDocuments = make([]ParsedDocument, 0)

	for _, currPage := range crawler.CrawledPages {
		parsedCurrPage, err := html.Parse(strings.NewReader(currPage.HTML))
		if err != nil {
			fmt.Println("Error occurred while trying to parse a page's HTML:", err)
			continue
		}

		content := ""

		var traverseParsedCurrPage func(*html.Node)
		traverseParsedCurrPage = func(node *html.Node) {
			if node.Type == html.TextNode {
				content += node.Data + "\n"
			}

			for child := node.FirstChild; child != nil; child = child.NextSibling {
				traverseParsedCurrPage(child)
			}
		}

		traverseParsedCurrPage(parsedCurrPage)

		parsedDoc := ParsedDocument{URL: currPage.URL, Content: content}
		ParsedDocuments = append(ParsedDocuments, parsedDoc)
	}

	return ParsedDocuments
}
