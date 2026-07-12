# Lvalues, Rvalues & References

- lvalue -> Named object with a persistent location in memory.
- rvalue -> Temporary object (e.g. function return value).

Non-const lvalue reference (`T&`):

- Can bind only to lvalues.
- Cannot bind to temporaries (rvalues).

Example:

```cpp
foo(getVector()); // if foo(T&) -> this is not okay AT ALL
```

However:

```cpp
auto v = getVector();
foo(v);             // this is okay
```

Reason:
A non-const reference can modify its argument, so C++ prevents it from referring to a temporary object that will soon be destroyed.
