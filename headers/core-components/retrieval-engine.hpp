#pragma once

#include <vector>

#include "../data-structures/candidate-doc.hpp"
#include "../data-structures/posting-list.hpp"

std::vector<PostingList> lookupPostingLists();
std::vector<CandidateDocument> generateCandidateDocuments();
int runRetrievalEngine();
