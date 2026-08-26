#pragma once

#include <string>
#include <vector>

#include "../data-structures/candidate-doc.hpp"
#include "../data-structures/posting-list.hpp"

std::vector<PostingList> lookupPostingLists();
std::vector<PostingList> lookupPostingLists(const std::vector<std::string>& queryTokens);

std::vector<CandidateDocument> generateCandidateDocuments();
std::vector<CandidateDocument> generateCandidateDocuments(
    const std::vector<std::string>& queryTokens);

int runRetrievalEngine();
