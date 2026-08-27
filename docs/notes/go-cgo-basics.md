# Go cgo Basics

## cgo Preamble

```go
/*
#cgo CFLAGS: -I...
#cgo LDFLAGS: -L... -l... -lstdc++ -lm
#include "bridge/c-api.h"
#include <stdlib.h>
*/
import "C"
```

- **cgo preamble** → comment immediately before `import "C"` that cgo reads specially.
- `CFLAGS` → compiler flags; `-I` specifies header search paths.
- `LDFLAGS` → **linker flags**; `-L` specifies library search paths, `-l` specifies libraries to link.
- `#include "bridge/c-api.h"` → makes the C declarations from the bridge available through `C`.
- `#include <stdlib.h>` → provides declarations such as `free()`, used here as `C.free()`.
- `import "C"` → enables access to the C declarations/functions/types through the special `C` namespace.

## `unsafe`

`unsafe` is a standard Go package for low-level operations that bypass Go's normal type/memory-safety guarantees.

Used here because cgo gives us **C pointers/memory**:

```go
unsafe.Pointer(...)
unsafe.Slice(...)
```

**Mental model:** `cgo` = communicate with C; `unsafe` = manipulate C pointers/memory from Go.

## `unsafe.Slice`

```go
unsafe.Slice(ptr, len)
```

Creates a Go slice **view over existing memory** starting at `ptr`; it does **not copy the elements**.

Here, the view points to C-allocated memory, which is later freed with `C.free_search_response()`.

## `make`

```go
make([]CandidateDocument, n)
```

Creates a Go slice containing `n` elements.

Do **not** assume this means "allocate on the heap"; Go's compiler decides stack vs heap through escape analysis.
