# C ABI & C++ Wrapper

**ABI (Application Binary Interface)** defines how compiled code interacts at the binary level: function calls, arguments, return values, symbol names, data layout, etc.

C++ has **name mangling**: the compiler encodes information such as function names and parameter types into binary symbol names.

```cpp
int search(int);
int search(double);
```

These need distinct symbols in the compiled program.

`extern "C"` tells the C++ compiler to use **C linkage** instead of C++ name mangling:

```cpp
extern "C" {
    void* core_create();
    void core_destroy(void* core);
}
```

### C-compatible API wrapper

A small interface implemented in C++ that exposes simple C-compatible functions to Go.

```
Go -> C-compatible API -> C++
```

The C++ implementation remains C++; only the exposed interface is C-compatible.

The wrapper is an **insulation layer**: Go depends on the wrapper's interface, not on the internal C++ implementation.
