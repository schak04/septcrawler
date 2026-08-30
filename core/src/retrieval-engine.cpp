#include "core-components/retrieval-engine.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "data-structures/candidate-doc.hpp"
#include "data-structures/inverted-index.hpp"
#include "data-structures/posting-list.hpp"

// g_ = global
// static gives this file-level state internal linkage
static InvertedIndex g_invertedIndex;

// public interface for the private file-level state (g_invertedIndex)
void setInvertedIndex(const InvertedIndex& index) { g_invertedIndex = index; }
const InvertedIndex& getInvertedIndex() { return g_invertedIndex; }

std::vector<PostingList> lookupPostingLists(const std::vector<std::string>& queryTokens) {
    std::vector<PostingList> postingListsBasedOnQueryTokens;
    for (const std::string& queryToken : queryTokens) {
        auto it = g_invertedIndex.index.find(queryToken);
        if (it != g_invertedIndex.index.end()) {
            postingListsBasedOnQueryTokens.push_back(it->second);
        }
    }

    return postingListsBasedOnQueryTokens;
}

std::vector<CandidateDocument> generateCandidateDocuments(
    const std::vector<std::string>& queryTokens) {
    std::vector<CandidateDocument> candidateDocs;
    std::vector<PostingList> postingListsToBeIntersected = lookupPostingLists(queryTokens);

    if (postingListsToBeIntersected.empty() ||
        postingListsToBeIntersected.size() < queryTokens.size()) {
        return candidateDocs;
    }

    // counts how many posting lists each doc appears in
    std::unordered_map<int, int> matchedTermsCountMap;  // docId -> matchedTermsCount

    for (const PostingList& postingList : postingListsToBeIntersected) {
        for (const Posting& posting : postingList.entries) {
            matchedTermsCountMap[posting.docId]++;
        }
    }

    for (const auto& entry : matchedTermsCountMap) {
        if (entry.second == static_cast<int>(postingListsToBeIntersected.size())) {
            CandidateDocument candidateDoc;
            candidateDoc.docId = entry.first;
            candidateDoc.matchedTermsCount = entry.second;
            candidateDocs.push_back(candidateDoc);
        }
    }

    return candidateDocs;
}
