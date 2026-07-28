CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

septcrawler: src/main.cpp src/index-builder.cpp src/query-processor.cpp src/retrieval-engine.cpp src/ranker.cpp
	$(CXX) $(CXXFLAGS) \
		src/main.cpp \
		src/index-builder.cpp \
		src/query-processor.cpp \
		src/retrieval-engine.cpp \
		src/ranker.cpp \
		-o bin/septcrawler

clean:
	rm -f bin/septcrawler
