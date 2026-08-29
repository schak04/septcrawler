package storage

import (
	"encoding/json"
	"os"
	"path/filepath"

	"septcrawler/internal/core"
)

const indexDestination string = "data/index/"
const indexFilename string = "inverted_index.json"

func StoreInvertedIndex() error {
	idx := core.BuildInvertedIndex()

	if err := os.MkdirAll(indexDestination, 0755); err != nil {
		return err
	}

	jsonIndex, err := json.Marshal(idx)
	if err != nil {
		return err
	}

	return os.WriteFile(filepath.Join(indexDestination, indexFilename), jsonIndex, 0600)
}

func ReadInvertedIndex() (core.InvertedIndex, error) {
	data, err := os.ReadFile(filepath.Join(indexDestination, indexFilename))
	if err != nil {
		return nil, err
	}

	var idx core.InvertedIndex
	if err := json.Unmarshal(data, &idx); err != nil {
		return nil, err
	}

	return idx, nil
}
