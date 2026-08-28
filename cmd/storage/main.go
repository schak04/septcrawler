package main

import (
	"fmt"
	"log"

	"septcrawler/internal/storage"
)

func main() {
	if err := storage.StoreData(); err != nil {
		log.Fatal(err)
	}

	fmt.Println("Documents stored successfully.")
}
