package storage

import (
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"septcrawler/internal/parser"
	"strconv"
	// "strings"
)

const destination string = "data/documents/"

type Document struct {
	URL     string
	Content string
}

// https://pkg.go.dev/os#MkdirAll
func StoreDocuments(source []parser.ParsedDocument) error {
	// sourceDirEntries, err := os.ReadDir(source)
	// if err != nil {
	// 	fmt.Println("No data found")
	// 	return err
	// }

	if err := os.MkdirAll(destination, 0755); err != nil {
		return err
	}

	for idx, parsedDoc := range source {
		// if entry.IsDir() {
		// 	continue
		// }

		// filename := entry.Name()
		// ext := filepath.Ext(filename)                           // https://pkg.go.dev/path/filepath
		// filenameWithoutExt := strings.TrimSuffix(filename, ext) // https://pkg.go.dev/strings

		// docId, err := strconv.Atoi(filenameWithoutExt[len("doc"):])
		// if err != nil {
		// 	fmt.Println("Could not convert string to int:", docId)
		// 	return err
		// }

		// docContent, err := ReadData(filepath.Join(source, filename))
		// if err != nil {
		// 	fmt.Println("Could not read document:", filename)
		// 	return err
		// }

		filename := "doc" + strconv.Itoa(idx)

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
