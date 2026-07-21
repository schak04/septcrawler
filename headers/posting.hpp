#pragma once

#include <vector>

class Posting {
   public:
    int docId;
    int termFrequency;
    std::vector<int> positions;
};
