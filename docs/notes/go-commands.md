## Go Commands

### Project / Module

- `go mod init <module-path>`: Initialise a new Go module.
- `go mod tidy`: Add missing dependencies and remove unused dependencies. Also updates `go.sum`.
- `go mod download`: Download module dependencies.
- `go mod graph`: Display the module dependency graph.
- `go mod why <module>`: Explain why a module is required.
- `go env`: Display Go environment variables.
- `go version`: Display the installed Go version.

### Build

- `go build`: Compile the current package.
- `go build ./...`: Compile all Go packages in the current module and its subdirectories recursively.
- `go build ./cmd/something`: Build a specific package/executable.
- `go install`: Compile and install a package or executable.

### Run

- `go run .`: Compile and run the current package.
- `go run ./cmd/something`: Compile and run the 'something' executable inside cmd/.

### Test

- `go test`: Run tests in the current package.
- `go test ./...`: Run tests for all packages in the module.
- `go test -v`: Run tests with verbose output.
- `go test -run <pattern>`: Run only tests whose names match the pattern.
- `go test -cover`: Run tests and display code coverage.
- `go test -race`: Run tests with the race detector enabled.

### Code Quality

- `gofmt -w <file.go>`: Format a Go source file.
- `gofmt -w .`: Format Go source files in the current directory.
- `go fmt ./...`: Format all packages in the module.
- `go vet ./...`: Analyse packages for suspicious or likely incorrect code.

### Dependencies

- `go get <module>`: Add or update a dependency.
- `go get <module>@<version>`: Add or update a dependency to a specific version.
- `go list -m all`: List all modules used by the current module.
- `go list ./...`: List all packages in the current module.

### Documentation

- `go doc <package>`: Display documentation for a package.
- `go doc <package>.<symbol>`: Display documentation for a specific symbol.
- `go doc -http=:PORT-NUMBER`: Serve package documentation over HTTP on port `PORT-NUMBER`.

### Tooling / Debugging

- `go env GOPATH`: Display the current Go workspace path.
- `go env GOMOD`: Display the path to the active `go.mod`.
- `go clean`: Remove cached build files and other generated files.

### Useful Patterns

- `go build ./... && go test ./...`: Build the entire module, then run all tests.
- `go test ./... -race`: Run the entire test suite with race detection.
- `go fmt ./... && go vet ./...`: Format the project and run static analysis.
