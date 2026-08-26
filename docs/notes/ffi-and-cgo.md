# FFI & cgo

**FFI (Foreign Function Interface)** lets code written in one language call code written in another language.

Go's FFI mechanism for C is **cgo**.

```
Go -> cgo -> C
```

Since SeptCrawler's core is C++, a C-compatible interface around the C++ implementation is exposed:

```
Go -> cgo -> C-compatible API -> C++
```

### Alternatives

- **IPC:** C++ runs as a separate process.
- **RPC:** C++ runs as a separate service and Go communicates with it over a network/RPC protocol.
