## `url.Parse`

- Parses a URL string into a `*url.URL` struct.
- Does **not** make an HTTP request or fetch the URL.
- Lets us access individual URL components.

```go
u, err := url.Parse("https://example.com/docs?lang=en#top")

u.Scheme   // "https"
u.Host     // "example.com"
u.Path     // "/docs"
u.RawQuery // "lang=en"
u.Fragment // "top"
```

- Can parse both absolute and relative URLs.

```go
url.Parse("https://example.com/docs") // absolute
url.Parse("/docs")                    // relative
```

### Essentially:

`URL string -> url.Parse() -> *url.URL -> access individual components`

**Ref:** https://pkg.go.dev/net/url
