/*
SeptCrawler: A search engine being made for learning-focused retrieval.
Author: Saptaparno Chakraborty
*/

#include "../headers/core-components/index-builder.hpp"
#include "../headers/core-components/query-processor.hpp"
#include "../headers/core-components/ranker.hpp"
#include "../headers/core-components/retrieval-engine.hpp"

int main() {
    runIndexBuilder();
    runQueryProcessor();
    runRetrievalEngine();
    runRanker();

    return 0;
}
