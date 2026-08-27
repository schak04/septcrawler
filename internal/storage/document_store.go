package storage

import (
	"encoding/json"
	"os"
)

// TODO: replace with what the crawler crawls and the parser parses once those are implemented
const source string = "../../dummy-data/"
const destination string = "../../data/documents/"

type Document struct {
	DocId   int
	Content []byte
	// []byte because filesystem APIs such as os.ReadFile and os.WriteFile operate directly on byte slices (ref: https://pkg.go.dev/os)
	// also because raw document content is fundamentally raw bytes
}

func ReadData(path string) []byte {
	docContent := os.ReadFile(path)
	return docContent
}

// https://pkg.go.dev/github.com/go-lang/go/src/pkg/os#MkdirAll
func StoreData(pathToDataDir string) {
	// TODO:
	// for each doc in source, we create a Document and then write it to the destination
	// in data/documents/<docId>.json
	sourceDirEntries := os.ReadDir(source)

	for _, entry := range sourceDirEntries {
		if entry.IsDir() {
			continue
		}
		// TODO: pick up from here
	}
}
