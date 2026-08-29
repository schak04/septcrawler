### cgo `import "C"`

- `C` is a special **cgo pseudo-package**, so it must be imported separately:
  ```go
  import "C"
  ```
- Normal Go packages can use grouped imports:
  ```go
  import (
      "unsafe"
  )
  ```
