## Package Init

- Package-level variables initialize before `main()` runs.
- Runtime/data-producing functions should not run at package level.
- Let the caller control execution order.
