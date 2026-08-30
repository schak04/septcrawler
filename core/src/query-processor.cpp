#include "core-components/query-processor.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

std::string normalizeQuery(const std::string& rawQuery) {
    std::string normalizedQuery = rawQuery;

    std::transform(normalizedQuery.begin(), normalizedQuery.end(), normalizedQuery.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    normalizedQuery.erase(std::remove_if(normalizedQuery.begin(), normalizedQuery.end(),
                                         [](unsigned char c) { return std::ispunct(c); }),
                          normalizedQuery.end());

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

std::vector<std::string> processQuery(const std::string& rawQuery) {
    std::string normalizedQuery = normalizeQuery(rawQuery);
    return tokenizeQuery(normalizedQuery);
}
