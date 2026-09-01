## `sync.WaitGroup` in recursive concurrency

- A `WaitGroup` tracks a set of concurrent work using a counter.
- In a recursive function, each call should not create its own independent `WaitGroup`.
- All goroutines belonging to the same set of work should share the same `WaitGroup`.
- `Add()` increments the counter before starting work.
- `Done()` decrements the counter when the work finishes.
- `Wait()` blocks until the counter reaches `0`.

```
SomeFunction(A)
  └── go SomeFunction(B)
          └── go SomeFunction(C)
```

All use the same WaitGroup.

## Refs

- https://pkg.go.dev/sync#WaitGroup
- https://go.dev/doc/effective_go#concurrency
