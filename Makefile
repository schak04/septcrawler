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

all: $(BIN_APP) $(BIN_CLI)

bin/%.o: core/src/%.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_CORE): $(CORE_OBJS) $(BRIDGE_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(BIN_APP): $(LIB_CORE) cmd/api/main.go internal/api/server.go internal/core/search.go
	go build -o $(BIN_APP) ./cmd/api

$(BIN_CLI): core/src/main.cpp $(LIB_CORE)
	$(CXX) $(CXXFLAGS) core/src/main.cpp $(LIB_CORE) -o $(BIN_CLI)

lib: $(LIB_CORE)

cli: $(BIN_CLI)

clean:
	rm -rf bin
