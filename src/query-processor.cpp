/*
 * TODO: actual user queries to be used once the API layer is integrated
 * */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// TODO: dummy query/queries to be replaced on the integration for the API layer
// #define QUERY "GNU Debugger"
#define QUERY "Some query related to something educational with %#* () special characters"

std::string normalizeQuery(const std::string& rawQuery) {
    std::string normalizedQuery = rawQuery;

    std::transform(normalizedQuery.begin(), normalizedQuery.end(), normalizedQuery.begin(), [](unsigned char c) { return std::tolower(c); });

    normalizedQuery.erase(std::remove_if(normalizedQuery.begin(), normalizedQuery.end(), [](unsigned char c) { return std::ispunct(c); }), normalizedQuery.end());

    return normalizedQuery;
}

std::vector<std::string> tokenizeQuery(const std::string& normalizedQuery) {
    std::vector<std::string> tokenizedQuery;

    std::stringstream ss(normalizedQuery);
    std::string token;
    while (ss >> token) {
        tokenizedQuery.push_back(token);
    }

    return tokenizedQuery;
}

// for the retrieval engine
std::vector<std::string> getProcessedQuery() {
    std::string normalizedQuery = normalizeQuery(QUERY);
    std::vector<std::string> tokenizedQuery = tokenizeQuery(normalizedQuery);

    return tokenizedQuery;
}

int main() {
    std::string normalizedQuery = normalizeQuery(QUERY);
    std::vector<std::string> tokenizedQuery = tokenizeQuery(normalizedQuery);

    std::cout << QUERY << " -> " << normalizedQuery << '\n';
    std::cout << "Query tokens: " << '\n';
    for (const std::string& token : tokenizedQuery) {
        std::cout << token << '\n';
    }

    return 0;
}
