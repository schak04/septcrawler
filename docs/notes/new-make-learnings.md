# Make Revision + New Stuff I Learned

Adding to [Make and CMake notes in Priorify's repo](https://github.com/schak04/priorify/blob/main/docs/notes/make-and-cmake-notes.md).

## Rule Structure

```make
target: prerequisites
	recipe
```

- **target** -> what Make should build
- **prerequisites** -> files/targets required to build it
- **recipe** -> command(s) used to build the target

## Automatic Variables

| Variable | Meaning            |
| -------- | ------------------ |
| `$@`     | Current target     |
| `$<`     | First prerequisite |
| `$^`     | All prerequisites  |

Example:

```make
bin/%.o: core/src/%.cpp
	g++ $(CXXFLAGS) -c $< -o $@
```

`core/src/foo.cpp` -> `bin/foo.o`

## Pattern Rules

```make
bin/%.o: core/src/%.cpp
```

`%` is a **pattern placeholder**. The same matched text is substituted on both sides.

```text
core/src/foo.cpp -> bin/foo.o
core/src/bar.cpp -> bin/bar.o
```

## Static Libraries

```make
LIB_CORE = bin/libseptcrawler_core.a
```

`.a` is a **static library** containing compiled object files.

```text
.cpp -> .o -> .a
```

`ar rcs` creates/updates the static library:

```make
$(LIB_CORE): $(CORE_OBJS) $(BRIDGE_OBJS)
	ar rcs $@ $^
```

## Special `@`

```make
@mkdir -p bin
```

`@` means **don't print this command before executing it**.

## Convenience Targets

```make
all: $(BIN_CORE) $(BIN_API)
lib: $(LIB_CORE)
api: $(BIN_API)
core: $(BIN_CORE)
```

These are **named entry points** for Make, not necessarily files.

```bash
make
make lib
make api
make clean
```

`all` is conventionally the default target when it appears first.

## Key Mental Model

```text
target -> needs prerequisites -> recipe builds target
```
