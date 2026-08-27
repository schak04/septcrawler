# Go HTTP Testing

### `httptest`

Tests HTTP handlers without starting a real HTTP server.

```go
req := httptest.NewRequest("GET", "/search?q=GNU+Debugger", nil)
w := httptest.NewRecorder()

searchHandler(w, req)
resp := w.Result()
```

- `NewRequest()` -> creates an in-memory HTTP request.
- `NewRecorder()` -> creates a fake `http.ResponseWriter` that records the handler's response.
- `Result()` -> returns the recorded `*http.Response` (status, headers, body).

### `testing.T`

`*testing.T` -> test context supplied by Go's testing framework.

- `t.Errorf()` -> report failure; continue test.
- `t.Fatalf()` -> report failure; stop current test.

### JSON response testing

```go
var data Response
json.NewDecoder(resp.Body).Decode(&data)
```

`Decode(&data)` -> reads JSON from the response body and stores the decoded values in the `Response` struct.

**Flow:**

`fake request -> handler -> fake response writer -> HTTP response -> decode JSON -> inspect data`
