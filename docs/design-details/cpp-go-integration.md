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
- **Persistent Index Transfer:** The Go storage layer owns filesystem I/O and reads `data/index/inverted_index.json`. It passes the deserialized index across the boundary via `load_inverted_index(InvertedIndexC)`, populating the C++ core in memory without C++ directly accessing disk JSON.

The wrapper provides an insulation layer between Go and the C++ implementation. As long as the C-compatible API remains unchanged, internal C++ implementation changes do not require changes to the Go code.

---

## Ownership Rules

- **C++-owned memory must have an explicit lifetime/release mechanism.** If the wrapper returns C++-allocated memory to Go, the API provides a corresponding function for releasing it. Go's garbage collector does not manage C/C++ memory.
- **Go pointers passed to C** are subject to cgo's pointer rules. Memory passed to C is implicitly pinned for the duration of the call. C++ must not retain a Go pointer beyond its permitted lifetime.
- **Strings are copied at the boundary** where C-compatible strings are required, rather than having C++ retain pointers into Go-managed string memory.

---

## Lifecycle of a Search Query

```
[Client]
   |
   | HTTP GET /search?q=GNU+Debugger
   v
[Go API Handler: internal/api/server.go]
   |
   | calls core.Search("GNU Debugger")
   v
[Go cgo Adapter: internal/core/search.go]
   |
   | 1. C.CString(query) allocates a null-terminated C string in C heap memory.
   | 2. Passes `const char*` across the boundary to C.search_query.
   v
[C-Compatible Bridge: core/src/bridge.cpp (extern "C")]
   |
   | 3. Converts `const char*` to `std::string`.
   | 4. QueryProcessor: `normalizeQuery` -> `tokenizeQuery`.
   | 5. RetrievalEngine: `generateCandidateDocuments(queryTokens)` intersects posting lists using loaded InvertedIndex.
   | 6. Ranker: `rankCandidateDocuments(candidateDocs, queryTokens)` computes TF-IDF scores from loaded InvertedIndex.
   | 7. Allocates C struct array with `std::malloc(sizeof(CandidateDocumentC) * count)`.
   | 8. Returns `SearchResponseC{documents, count}` back across the boundary.
   v
[Go cgo Adapter: internal/core/search.go]
   |
   | 9. Reads `cResponse.documents` using `unsafe.Slice`.
   | 10. Copies data into native Go slice `[]CandidateDocument`.
   | 11. `defer C.free_search_response(cResponse)` frees the C result array.
   | 12. `defer C.free(unsafe.Pointer(cQuery))` frees the C input string.
   v
[Go API Handler: internal/api/server.go]
   |
   | Encodes JSON response with results.
   v
[Client Receives JSON]
```

---
