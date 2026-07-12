# std::transform

Header: `<algorithm>`

Syntax:

```cpp
std::transform(first, last, dest, operation);
```

Parameters:

- `first` -> Iterator to the first element in the input range.
- `last` -> Iterator pointing one past the last element in the input range.
- `dest` -> Iterator to the beginning of the destination range. Can be the same as `first` for in-place transformation.
- `operation` -> Function, function object, or lambda applied to each element. The return value is written to the destination.

Example (in-place lowercase conversion):

```cpp
std::transform(str.begin(), str.end(), str.begin(),
               [](unsigned char c) { return std::tolower(c); });
```
