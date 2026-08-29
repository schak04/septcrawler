CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Icore/include
AR = ar
ARFLAGS = rcs

CORE_SRCS = core/src/index-builder.cpp \
            core/src/query-processor.cpp \
            core/src/retrieval-engine.cpp \
            core/src/ranker.cpp

BRIDGE_SRCS = core/src/bridge.cpp

CORE_OBJS = bin/index-builder.o \
            bin/query-processor.o \
            bin/retrieval-engine.o \
            bin/ranker.o

BRIDGE_OBJS = bin/bridge.o

LIB_CORE = bin/libseptcrawler_core.a
BIN_APP = bin/septcrawler
BIN_CLI = bin/septcrawler-cli
BIN_STORAGE = bin/storage

all: $(BIN_APP) $(BIN_CLI) $(BIN_STORAGE)

bin/%.o: core/src/%.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_CORE): $(CORE_OBJS) $(BRIDGE_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(BIN_APP): $(LIB_CORE) cmd/api/main.go internal/api/server.go internal/core/search.go internal/core/index.go
	go build -o $(BIN_APP) ./cmd/api

$(BIN_CLI): core/src/main.cpp $(LIB_CORE)
	$(CXX) $(CXXFLAGS) core/src/main.cpp $(LIB_CORE) -o $(BIN_CLI)

$(BIN_STORAGE): $(LIB_CORE) cmd/storage/main.go internal/storage/document_store.go internal/storage/index_store.go internal/core/index.go
	go build -o $(BIN_STORAGE) ./cmd/storage

lib: $(LIB_CORE)

cli: $(BIN_CLI)

storage: $(BIN_STORAGE)

clean:
	rm -rf bin
