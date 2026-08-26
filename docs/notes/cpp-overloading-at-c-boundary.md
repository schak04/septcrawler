### C-Compatible Boundary and C++ Overloading

- C++ function overloading works normally inside the C++ implementation.
- Only functions exposed through the `extern "C"` API need C-compatible linkage.
- Therefore, C++ overloaded functions do not conflict with the C API.
