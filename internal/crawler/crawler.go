package crawler

import (
	"errors"
	"io"
	"net/http"
)

func Crawl(url string) (string, error) {
	res, err := http.Get(url)
	if err != nil {
		return "", err
	}
	defer res.Body.Close()

	if res.StatusCode >= 400 && res.StatusCode < 600 {
		return "", errors.New("Some error occurred")
	}

	body, err := io.ReadAll(res.Body)

	if err != nil {
		return "", err
	}

	return string(body), nil
}
