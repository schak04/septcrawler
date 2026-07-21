#pragma once

#include <string>
#include <unordered_map>

#include "./posting-list.hpp"

class InvertedIndex {
   public:
    std::unordered_map<std::string, PostingList> index;
};
