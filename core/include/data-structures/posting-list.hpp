#pragma once

#include <vector>

#include "./posting.hpp"

struct PostingList {
    std::vector<Posting> entries;
    int totalFrequency;
};
