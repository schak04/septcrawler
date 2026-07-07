#pragma once

#include <vector>

class Posting {
    int docId;
    int termFrequency;
    std::vector<int> positions;
};
