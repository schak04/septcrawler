# Go Testing Failures

```go
t.Errorf(...)
```

Reports a test failure and **continues** the current test.

```go
t.Fatalf(...)
```

Reports a test failure and **stops** the current test.

**Rule:** `Fatalf` when continuing could make the test invalid or unsafe; `Errorf` when remaining checks can still run.

```
fmt.Println(...)  -> ordinary program output
t.Errorf(...)     -> message to Go's testing framework
```
