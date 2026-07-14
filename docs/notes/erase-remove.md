# erase-remove idiom

Removes elements matching a condition from a container.

Syntax:

```cpp
container.erase(
    std::remove_if(container.begin(), container.end(), predicate),
    container.end()
);
```

How it works:

- `std::remove_if` shifts kept elements to the front.
- Returns the new logical end.
- `erase()` removes the leftover elements.

Example:

```cpp
text.erase(
    std::remove_if(
        text.begin(),
        text.end(),
        [](unsigned char c) { return std::ispunct(c); }
    ),
    text.end()
);
```

Note:

- `remove_if` doesn't erase; `erase` does.
- Prefer a lambda with `std::ispunct(unsigned char)`.
