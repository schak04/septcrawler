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
BIN_CORE = bin/septcrawler
BIN_API = bin/api

all: $(BIN_CORE) $(BIN_API)

$(BIN_CORE): core/src/main.cpp $(CORE_SRCS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) core/src/main.cpp $(CORE_SRCS) -o $(BIN_CORE)

bin/%.o: core/src/%.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_CORE): $(CORE_OBJS) $(BRIDGE_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(BIN_API): $(LIB_CORE) cmd/api/main.go internal/api/server.go internal/core/search.go
	go build -o $(BIN_API) ./cmd/api

lib: $(LIB_CORE)

api: $(BIN_API)

core: $(BIN_CORE)

clean:
	rm -rf bin
