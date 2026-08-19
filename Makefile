CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Icore/include

septcrawler: core/src/main.cpp core/src/index-builder.cpp core/src/query-processor.cpp core/src/retrieval-engine.cpp core/src/ranker.cpp
	$(CXX) $(CXXFLAGS) \
		core/src/main.cpp \
		core/src/index-builder.cpp \
		core/src/query-processor.cpp \
		core/src/retrieval-engine.cpp \
		core/src/ranker.cpp \
		-o bin/septcrawler

clean:
	rm -f bin/septcrawler
