#pragma once

#include <string>
#include <vector>

#include "../data-structures/candidate-doc.hpp"

// to avoid unnecessary calculations of the corpus stats
struct CorpusStatistics;

double calcTFIDFScore(const CandidateDocument& cd, const std::vector<std::string>& query);

double calcTFIDFScore(const CandidateDocument& cd, const std::vector<std::string>& query,
                      const CorpusStatistics& stats);

std::vector<CandidateDocument> rankCandidateDocuments(
    const std::vector<CandidateDocument>& candidateDocs, const std::vector<std::string>& query);
