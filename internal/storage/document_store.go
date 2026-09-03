package storage

import (
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"septcrawler/internal/parser"
	"strconv"
)

const destination string = "data/documents/"

type Document struct {
	URL     string
	Content string
}

func StoreDocuments(source []parser.ParsedDocument) error {
	if err := os.MkdirAll(destination, 0755); err != nil { // https://pkg.go.dev/os#MkdirAll
		return err
	}

	for idx, parsedDoc := range source {
		filename := "doc" + strconv.Itoa(idx+1)

		currDoc := Document{URL: parsedDoc.URL, Content: parsedDoc.Content}
		jsonDoc, err := json.Marshal(currDoc)
		if err != nil {
			fmt.Println("Could not convert to json:", filename)
			return err
		}

		err = os.WriteFile(filepath.Join(destination, filename+".json"), jsonDoc, 0600)
		if err != nil {
			return err
		}
	}

	return nil
}
