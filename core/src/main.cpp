/*
SeptCrawler: A search engine being made for learning-focused retrieval.
Author: Saptaparno Chakraborty
*/

#include "core-components/index-builder.hpp"
#include "core-components/query-processor.hpp"
#include "core-components/ranker.hpp"
#include "core-components/retrieval-engine.hpp"

int main() {
    runIndexBuilder();
    runQueryProcessor();
    runRetrievalEngine();
    runRanker();

    return 0;
}
