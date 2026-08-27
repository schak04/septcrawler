#pragma once

#include <string>
#include <vector>

std::string normalizeQuery(const std::string& rawQuery);
std::vector<std::string> tokenizeQuery(const std::string& normalizedQuery);
std::vector<std::string> processQuery(const std::string& rawQuery);
