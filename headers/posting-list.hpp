#pragma once

#include <vector>

#include "./posting.hpp"

class PostingList {
   public:
    std::vector<Posting> entries;
    int totalFrequency;
};
