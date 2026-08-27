#pragma once

#include <string>
#include <vector>

#include "../data-structures/inverted-index.hpp"

std::vector<std::string> readFromDocs(const std::string& path);
void normalizeDoc(std::string& doc);
std::vector<std::string> normalizeDocs(const std::vector<std::string>& rawDocs);
std::vector<std::vector<std::string>> tokenizeDocs(const std::vector<std::string>& normalizedDocs);
InvertedIndex buildInvertedIndex(const std::vector<std::vector<std::string>>& processedDocs);
