#pragma once

#include <string>
#include <vector>

#include "../data-structures/candidate-doc.hpp"

double calcTFIDFScore(const CandidateDocument& cd);
double calcTFIDFScore(const CandidateDocument& cd, const std::vector<std::string>& query);

std::vector<CandidateDocument> rankCandidateDocuments(
    const std::vector<CandidateDocument>& candidateDocs);
std::vector<CandidateDocument> rankCandidateDocuments(
    const std::vector<CandidateDocument>& candidateDocs, const std::vector<std::string>& query);

int runRanker();
