package storage

import (
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

// TODO: replace with what the crawler crawls and the parser parses once those are implemented
const source string = "dummy-data/"
const destination string = "data/documents/"

type Document struct {
	DocId   int
	Content string
}

func ReadData(path string) ([]byte, error) {
	docContent, err := os.ReadFile(path)
	if err != nil {
		return []byte(""), err
	}
	return docContent, nil
}

// https://pkg.go.dev/os#MkdirAll
func StoreData() error {
	sourceDirEntries, err := os.ReadDir(source)
	if err != nil {
		fmt.Println("No data found")
		return err
	}

	if err := os.MkdirAll(destination, 0755); err != nil {
		return err
	}

	for _, entry := range sourceDirEntries {
		if entry.IsDir() {
			continue
		}

		filename := entry.Name()
		ext := filepath.Ext(filename)                           // https://pkg.go.dev/path/filepath
		filenameWithoutExt := strings.TrimSuffix(filename, ext) // https://pkg.go.dev/strings

		docId, err := strconv.Atoi(filenameWithoutExt[len("doc"):])
		if err != nil {
			fmt.Println("Could not convert string to int:", docId)
			return err
		}

		docContent, err := ReadData(filepath.Join(source, filename))
		if err != nil {
			fmt.Println("Could not read document:", filename)
			return err
		}

		currDoc := Document{DocId: docId, Content: string(docContent)}
		jsonDoc, err := json.Marshal(currDoc)
		if err != nil {
			fmt.Println("Could not convert to json:", filename)
			return err
		}

		err = os.WriteFile(filepath.Join(destination, filenameWithoutExt+".json"), jsonDoc, 0600)
		if err != nil {
			return err
		}
	}

	return nil
}
