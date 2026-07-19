# STL Memory Management

### Core Rule

- **Container object:** Lives wherever the container object itself is created (e.g., stack, heap, or static storage). Stores metadata such as pointers and size.
- **Contained elements:** May use dynamic memory depending on the container.

### Container Memory Behaviour

- **`std::vector`:** Contiguous dynamic array. Reallocates as needed.
- **`std::deque`:** Multiple dynamically allocated blocks. Not contiguous.
- **`std::list` / `std::map` / `std::set`:** Node-based. Each element is allocated separately.
- **`std::array`:** No dynamic allocation. Storage is part of the array object itself.
- **`std::string`:** Usually uses SSO (Small String Optimization). Short strings are stored inside the string object; longer strings use dynamic allocation.

### Cleanup Lifecycle (RAII)

1. Container goes out of scope.
2. Container destructor runs automatically.
3. Destructors run for all stored elements.
4. Dynamically allocated memory is released via the allocator.

### The Pointer Trap

- `std::vector<MyClass>` -> Safe. Elements are destroyed automatically.
- `std::vector<MyClass*>` -> Raw pointers are destroyed, **not** the objects they point to.
- **Fix:** Use smart pointers, e.g. `std::vector<std::unique_ptr<MyClass>>`.
