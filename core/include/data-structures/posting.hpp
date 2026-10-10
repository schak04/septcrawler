#pragma once

#include <vector>

struct Posting {
    int docId;
    int termFrequency;
    std::vector<int> positions;
};
