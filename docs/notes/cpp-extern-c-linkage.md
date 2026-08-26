# extern "C" Linkage

Forces the C++ compiler to use **C linkage conventions** (unmangled symbol names) for FFI/linker compatibility, without changing internal C++ code execution[cite: 1].

## Key Mechanics

- **C++ Linkage:** Mangles symbols (`search_query` -> `_Z12search_queryPKc`) to support function overloading[cite: 1].
- **C Linkage:** Preserves exact symbol names (`search_query`), which `cgo` and C linkers require[cite: 1].

## Minimal Usage Pattern

```cpp
#ifdef __cplusplus
extern "C" {
#endif

SearchResponseC search_query(const char* query);

#ifdef __cplusplus
}
#endif
```

## Critical Caveats

1. **No Overloading:** Cannot overload functions inside `extern "C"`[cite: 1].
2. **Exceptions:** C++ exceptions must **never** cross FFI boundaries (`std::terminate`)[cite: 1].
3. **Memory Ownership:** Go GC cannot free C++ allocations; use `std::malloc`/`std::free` across boundaries[cite: 1].

## Reference

- [cppreference: Language linkage](https://en.cppreference.com/w/cpp/language/language_linkage)
