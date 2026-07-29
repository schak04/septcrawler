#pragma once
#include <vector>

#include "../data-structures/candidate-doc.hpp"

int calcScore(const CandidateDocument& cd);

std::vector<CandidateDocument> rankCandidateDocuments(const std::vector<CandidateDocument>& candidateDocs);

int runRanker();
