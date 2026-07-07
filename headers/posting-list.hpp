#pragma once

#include <vector>

#include "./posting.hpp"

class PostingList {
    std::vector<Posting> entries;
    int totalFrequency;
};
