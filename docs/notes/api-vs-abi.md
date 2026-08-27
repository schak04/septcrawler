# API vs ABI

## API (Application Programming Interface)

- **Level:** Source code level.
- **Defines:** What functions, structs, types, and signatures are available to write code against.
- **Example in SeptCrawler:** `SearchResponseC search_query(const char* query);` declared in `c-api.h`.
- **Breaking change:** Changing a function signature breaks source compilation.

## ABI (Application Binary Interface)

- **Level:** Compiled binary / machine code level.
- **Defines:** How compiled machine code interacts at runtime: calling conventions (register/stack usage for arguments and return values), struct memory layout (field offsets, byte padding, alignment), and symbol name encoding.
- **Example in SeptCrawler:** `extern "C"` forcing the compiler to emit unmangled symbol names and standard C calling conventions so the compiled Go binary can call into the compiled C++ code.
- **Breaking change:** Changing struct field order or layout can cause memory corruption at runtime even if the code compiles cleanly.

## Summary

- **API:** Source code contract for the programmer (compile-time).
- **ABI:** Binary contract for the linker and CPU (runtime).
