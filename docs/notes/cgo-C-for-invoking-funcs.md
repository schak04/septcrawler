# cgo: How `C.function()` Works

- `import "C"` is a special cgo import that lets Go interact with C.
- The comment immediately before it is the **C preamble**; `#include` brings in C declarations (functions, structs, etc.).
- `C.someFunction()` means: **call the C function `someFunction`**.
- The header (`.h`) provides the declaration, what exists and its signature.
- `#cgo LDFLAGS` tells the linker where to find the compiled implementation (library).
- cgo generates the glue needed to call C from Go.

```
C header -> declares the function
    |
    v
cgo -> exposes it as C.function()
    |
    v
C/C++ library -> contains the implementation
    |
    v
Go -> calls it
```
