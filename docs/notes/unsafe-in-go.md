# `unsafe` Package

The `unsafe` package provides operations that bypass Go's normal type and memory safety.

```go
import "unsafe"
```

### Common uses

- `unsafe.Pointer` -> convert between pointers of different types.
- `unsafe.Sizeof(x)` -> get the size of a value in bytes.
- `unsafe.Alignof(x)` -> get the memory alignment of a value.
- `unsafe.Offsetof(x)` -> get a struct field's byte offset.

### Example

```go
var x int
p := unsafe.Pointer(&x)

fmt.Println(unsafe.Sizeof(x))
```

### Mental model

Normally, Go controls how memory is accessed.

`unsafe` lets us work directly with the underlying memory.

It should be used sparingly because incorrect use can break Go's usual memory safety guarantees.
