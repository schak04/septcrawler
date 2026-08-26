# cgo Memory & Ownership

Go's garbage collector manages Go memory, not memory allocated by C or C++.

### Ownership

Prefer:

```
C++ allocates -> Go uses -> C++ frees
```

Expose a corresponding free function when C++ returns allocated memory:

```cpp
char* core_search(...);
void core_free(char*);
```

Go must call `core_free()` when finished.

### Go pointers passed to C

Go memory passed to C is subject to cgo's pointer rules.

For a normal cgo call, the memory is only guaranteed to remain pinned for the duration of the call.

**Do not let C++ retain a Go pointer after the call returns.**

### Strings

Go and C/C++ have different string representations.

Typically:

```
Go string --copy--> C string (char*) -> C++
```

The C/C++ side works with its own copy rather than retaining a pointer into Go's string memory.

### Rule of thumb

Keep ownership explicit and avoid sharing memory across the Go/C++ boundary unless there is a specific reason to do so.
