# cgo Build Configuration

- **`${SRCDIR}`** = directory containing the current Go source file.

- **`CFLAGS`** = compiler flags used when cgo compiles C/C++ code.
  - `-I<dir>` adds `<dir>` to the header search path.

- **`LDFLAGS`** = linker flags used when linking.
  - `-L<dir>` adds `<dir>` to the library search path.
  - `-lfoo` links against `libfoo`.
  - `-lstdc++` links the C++ standard library.
  - `-lm` links the C math library.

```c
#cgo CFLAGS: -I${SRCDIR}/path/to/include
#cgo LDFLAGS: -L${SRCDIR}/path/to/lib -lfoo
#include "foo.h"
```
