# Go Struct Tags

Struct tags attach metadata to struct fields.

```go
type Response struct {
	Query string `json:"query"`
}
```

- `` `json:"query"` `` → tells `encoding/json` to use `"query"` as the JSON field name.
- Backticks contain the **struct tag string**.
- Without the tag, JSON normally uses the Go field name (`"Query"`).
