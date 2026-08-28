## Bear & `compile_commands.json`

- **Bear** = Build EAR; observes compiler commands during a build and generates `compile_commands.json`.
- **`compile_commands.json`** = database describing how each C/C++ source file is compiled (compiler, flags, include paths, etc.).
- **clangd** is a **C/C++ language server**.
- **clangd uses `compile_commands.json`** to know how each source file should be parsed.
- Bear only records **compilation commands that actually execute**, so only those source files appear in the database.
- `compile_commands.json` is generally **generated tooling metadata**, not part of the source code.
