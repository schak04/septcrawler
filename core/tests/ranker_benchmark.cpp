#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core-components/ranker.hpp"
#include "core-components/retrieval-engine.hpp"
#include "data-structures/candidate-doc.hpp"
#include "data-structures/inverted-index.hpp"
#include "data-structures/posting-list.hpp"

namespace {

constexpr int documentCount = 4000;
constexpr int candidateCount = 3000;
constexpr int queryTermCount = 3;
constexpr int runCount = 5;

const std::vector<std::string> query = {"term0", "term1", "term2"};

InvertedIndex buildIndex() {
    InvertedIndex index;
    for (int term = 0; term < queryTermCount; ++term) {
        PostingList postings{};
        for (int docId = 0; docId < documentCount; ++docId) {
            postings.entries.push_back({docId, 1 + (docId + term) % 5, {}});
            postings.totalFrequency += postings.entries.back().termFrequency;
        }
        index.index.emplace(query[term], std::move(postings));
    }
    return index;
}

std::vector<CandidateDocument> buildCandidates() {
    std::vector<CandidateDocument> candidates;
    candidates.reserve(candidateCount);
    for (int docId = 0; docId < candidateCount; ++docId) {
        candidates.push_back({docId, queryTermCount});
    }
    return candidates;
}

std::vector<CandidateDocument> rankWithLinearLookup(
    const std::vector<CandidateDocument>& candidates, const InvertedIndex& index) {
    std::unordered_map<int, double> documentLengths;
    std::unordered_set<int> documentIds;
    for (const auto& [term, postings] : index.index) {
        for (const Posting& posting : postings.entries) {
            documentLengths[posting.docId] += posting.termFrequency;
            documentIds.insert(posting.docId);
        }
    }

    std::vector<std::pair<CandidateDocument, double>> scored;
    scored.reserve(candidates.size());
    for (const CandidateDocument& candidate : candidates) {
        double score = 0.0;
        for (const std::string& term : query) {
            const PostingList& postings = index.index.at(term);
            int termFrequency = 0;
            for (const Posting& posting : postings.entries) {
                if (posting.docId == candidate.docId) {
                    termFrequency = posting.termFrequency;
                    break;
                }
            }
            const double tf = termFrequency / documentLengths.at(candidate.docId);
            const double idf =
                std::log(static_cast<double>(documentIds.size()) / (1 + postings.entries.size()));
            score += tf * idf;
        }
        scored.emplace_back(candidate, score);
    }

    std::sort(scored.begin(), scored.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    std::vector<CandidateDocument> ranked;
    ranked.reserve(scored.size());
    for (const auto& [candidate, score] : scored) {
        ranked.push_back(candidate);
    }
    return ranked;
}

template <typename RankFunction>
double medianMilliseconds(RankFunction rank) {
    std::vector<double> times;
    for (int run = 0; run < runCount; ++run) {
        const auto start = std::chrono::steady_clock::now();
        const auto ranked = rank();
        const auto end = std::chrono::steady_clock::now();
        if (ranked.size() != candidateCount) {
            return 0.0;
        }
        times.push_back(std::chrono::duration<double, std::milli>(end - start).count());
    }
    std::sort(times.begin(), times.end());
    return times[times.size() / 2];
}

}  // namespace

int main() {
    const InvertedIndex index = buildIndex();
    setInvertedIndex(index);
    const std::vector<CandidateDocument> candidates = buildCandidates();

    const double linearMs =
        medianMilliseconds([&] { return rankWithLinearLookup(candidates, index); });
    const double lookupMs =
        medianMilliseconds([&] { return rankCandidateDocuments(candidates, query); });
    const auto linearRanked = rankWithLinearLookup(candidates, index);
    const auto lookupRanked = rankCandidateDocuments(candidates, query);
    if (linearRanked.size() != lookupRanked.size()) {
        return 1;
    }
    for (std::size_t i = 0; i < linearRanked.size(); ++i) {
        if (linearRanked[i].docId != lookupRanked[i].docId) {
            std::cerr << "Ranking differs at position " << i << '\n';
            return 1;
        }
    }

    std::cout << "Documents: " << documentCount << "\n"
              << "Candidates: " << candidateCount << "\n"
              << "Query terms: " << queryTermCount << "\n"
              << "Runs: " << runCount << "\n"
              << "Linear lookup median: " << linearMs << " ms\n"
              << "Hash lookup median: " << lookupMs << " ms\n"
              << "Ranking order: identical\n";
}
