#pragma once

#include <string>
#include <unordered_map>

#include "./posting-list.hpp"

struct InvertedIndex {
    std::unordered_map<std::string, PostingList> index;
};
