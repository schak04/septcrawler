#pragma once

#include <string>
#include <vector>

#include "../data-structures/candidate-doc.hpp"
#include "../data-structures/inverted-index.hpp"
#include "../data-structures/posting-list.hpp"

void setInvertedIndex(const InvertedIndex& index);
const InvertedIndex& getInvertedIndex();

std::vector<PostingList> lookupPostingLists(const std::vector<std::string>& queryTokens);
std::vector<CandidateDocument> generateCandidateDocuments(
    const std::vector<std::string>& queryTokens);
