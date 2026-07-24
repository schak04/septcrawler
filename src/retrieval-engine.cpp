/*
 * TODO: receive normalized and tokenized queries from the query processor
 * look up postings from the storage layer (will use dummy postings until I finally implement the storage layer)
 * generate candidate docs with some lightweight stats for the ranker
 * send it to the ranker
 * */

#include <iostream>
#include <string>
#include <vector>

#include "../headers/query-processor.hpp"

int main() {
    std::vector<std::string> processedQuery = getProcessedQuery();

    return 0;
}
