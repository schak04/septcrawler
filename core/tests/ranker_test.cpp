#include "core-components/ranker.hpp"

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

#include "core-components/retrieval-engine.hpp"
#include "data-structures/inverted-index.hpp"
#include "data-structures/posting-list.hpp"

int main() {
    InvertedIndex testInvidx;

    PostingList alpha{};
    alpha.entries.push_back({1, 1, {}});
    alpha.entries.push_back({2, 3, {}});
    alpha.totalFrequency = 4;
    testInvidx.index.emplace("alpha", alpha);

    PostingList beta{};
    beta.entries.push_back({1, 1, {}});
    beta.entries.push_back({2, 1, {}});
    beta.totalFrequency = 2;
    testInvidx.index.emplace("beta", beta);

    PostingList filler{};  // introduces docs 3 and 4 w/o making them candidates
    filler.entries.push_back({1, 8, {}});
    filler.entries.push_back({2, 1, {}});
    filler.entries.push_back({3, 2, {}});
    filler.entries.push_back({4, 2, {}});
    filler.totalFrequency = 13;
    testInvidx.index.emplace("filler", filler);

    setInvertedIndex(testInvidx);

    const std::vector<std::string> query = {"alpha", "beta"};
    const double idf = std::log(4.0 / 3.0);

    const double doc1Score = calcTFIDFScore({1, 2}, query);
    const double doc2Score = calcTFIDFScore({2, 2}, query);
    const double doc3Score = calcTFIDFScore({3, 0}, query);

    // NOTE: 1e-12 = 10^(-12) = 0.000000000001
    // why check ( < 1e-12 ) instead of ( == 0 )?
    // because floating-point numbers are not always represented exactly in binary
    // so it's safer to check whether the difference is sufficiently small instead of
    // checking whether the difference is exactly 0
    assert(std::abs(doc1Score - 0.2 * idf) < 1e-12);
    assert(std::abs(doc2Score - 0.8 * idf) < 1e-12);
    assert(doc3Score == 0.0);
    assert(std::abs(calcTFIDFScore({1, 2}, {"missing", "alpha", "beta"}) - doc1Score) < 1e-12);

    const double duplicateTermScore = calcTFIDFScore({1, 2}, {"alpha", "beta", "alpha"});
    assert(std::abs(duplicateTermScore - 0.3 * idf) < 1e-12);

    const std::vector<CandidateDocument> candidates = {{1, 2}, {2, 2}};
    const std::vector<double> scores = calcTFIDFScores(candidates, query);
    assert(scores.size() == candidates.size());
    assert(std::abs(scores[0] - doc1Score) < 1e-12);
    assert(std::abs(scores[1] - doc2Score) < 1e-12);
    assert(std::abs(calcTFIDFScore(candidates[0], query) - scores[0]) < 1e-12);
    assert(calcTFIDFScores({}, query).empty());

    const std::vector<CandidateDocument> ranked = rankCandidateDocuments(candidates, query);

    assert(ranked.size() == 2);
    assert(ranked[0].docId == 2);  // since doc 2 has a higher score
    assert(ranked[1].docId == 1);
}
