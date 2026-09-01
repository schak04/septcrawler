## Goroutines and program termination

- `go f()` starts `f` in a new goroutine; the caller **does not wait**.
- A goroutine can continue running after the function that created it returns.
- The **entire program terminates when the `main` function returns**, even if other goroutines are still running.
- If the program must wait for goroutines to finish, use a `sync.WaitGroup`.

```
go Crawl(...)
    |
    v
Crawl runs concurrently
    |
    v
parent Crawl may return
    |
    v
child goroutine can continue
    |
    v
main() returns
    |
    v
entire program terminates
```

**Sooooooo:**  
`go f() -> independent execution -> need synchronization to wait`

## Refs:

- https://go.dev/ref/spec#Go_statements
- https://pkg.go.dev/sync#WaitGroup
