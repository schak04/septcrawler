# C++ <-> Go Integration

The Go service layer connects to the C++ core via **cgo + a C-compatible API wrapper**, within the same process.

```text
Go -> cgo -> C-compatible API -> C++ core
```

---

## Alternatives

| Option                               | Why I did not choose it                                                                                            |
| ------------------------------------ | ------------------------------------------------------------------------------------------------------------------ |
| Separate process (IPC)               | Requires a communication protocol, message framing, process lifecycle management, and additional failure handling. |
| Separate network service (HTTP/gRPC) | Adds serialisation, networking, operational complexity, and requires a network-facing server on the C++ side.      |

cgo was chosen because v1 runs as a **single-node system**: the Go service and C++ core run together on the same machine and in the same process. Calling the C-compatible API through cgo therefore requires minimal additional infrastructure.

---

## The Boundary

The C-compatible API is the only interface Go uses to access the C++ core. C++ implementation details do not cross this boundary.

- **C++ side:** `QueryProcessor`, `RetrievalEngine`, `Ranker`, `InvertedIndex`, `CandidateDocument`, STL types, and templates remain internal.
- **C-compatible API:** simple C-compatible functions and types such as integers, character pointers, and plain structs. The functions are implemented in C++ and exposed with `extern "C"` linkage.
- **Go side:** a thin adapter converts between Go types and the types exposed by the C-compatible API.

The wrapper provides an insulation layer between Go and the C++ implementation. As long as the C-compatible API remains unchanged, internal C++ implementation changes do not require changes to the Go code.

---

## Ownership Rules

- **C++-owned memory must have an explicit lifetime/release mechanism.** If the wrapper returns C++-allocated memory to Go, the API provides a corresponding function for releasing it. Go's garbage collector does not manage C/C++ memory.
- **Go pointers passed to C** are subject to cgo's pointer rules. Memory passed to C is implicitly pinned for the duration of the call. C++ must not retain a Go pointer beyond its permitted lifetime.
- **Strings are copied at the boundary** where C-compatible strings are required, rather than having C++ retain pointers into Go-managed string memory.
