### `__cplusplus`

`__cplusplus` is automatically defined when a file is being compiled as C++.

```cpp
#ifdef __cplusplus
extern "C" {
#endif
```

This means:

> If this header is being used from C++, apply `extern "C"`. Otherwise, skip it.
