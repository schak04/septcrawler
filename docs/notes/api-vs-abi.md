# API vs ABI

## 1. API (Application Programming Interface)

- **What it is:** The contract at the **source code** level (what is written before compiling).
- **What it defines:** Function names, parameter types, return types, struct definitions, and abstractions exposed in header files.
- **In SeptCrawler:** The header file `core/include/bridge/c-api.h`. It defines that calling `search_query(const char*)` returns a `SearchResponseC` struct.
- **What breaks it:** Renaming a function or modifying a parameter in the header. The compiler catches this immediately with a compile-time error.

---

## 2. ABI (Application Binary Interface)

- **What it is:** The contract at the **machine code / binary** level (how compiled code interacts in memory and CPU registers at runtime).
- **What it defines:**
  1. **Symbol names:** How function names are encoded in compiled `.o` object files (C++ mangles names like `_Z12search_queryPKc` to support overloading; C keeps them plain like `search_query`).
  2. **Calling conventions:** Which CPU registers (e.g. `rdi`, `rsi`) or stack locations hold arguments and return values.
  3. **Memory layout:** Exact byte offsets, struct padding, and alignment in RAM.
- **In SeptCrawler:** `extern "C"` forces the C++ compiler to generate object code adhering to standard C binary conventions so Go's `cgo` can link and call the compiled C++ code directly in memory.
- **What breaks it:** Discrepancies in struct memory alignment or calling conventions between Go and C++. The program may compile cleanly, but crash or read garbage memory at runtime.

---

## Why both terms appear in the project

- **"C-compatible API wrapper":** Refers to the code written (`c-api.h` and `bridge.cpp`) that defines the callable search interface.
- **"In-process C-ABI boundary":** Refers to the **architectural decision**: instead of communicating over a network (HTTP/gRPC) or across OS processes (IPC), Go and C++ run in the same process and communicate directly via standard C machine-level calling conventions.

---

## Quick Summary

- **API:** *"How is the code written to call this?"* (Source code / Header / Compile-time)
- **ABI:** *"How do the CPU and linker execute this in memory?"* (Binary code / Registers / Runtime)
