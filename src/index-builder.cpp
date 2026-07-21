#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../headers/inverted-index.hpp"
#include "../headers/posting-list.hpp"
#include "../headers/posting.hpp"

// for future ref: https://cppreference.com/cpp/filesystem
// one string in the vector corresponds to one doc's content
std::vector<std::string> readFromDocs(const std::string& path) {
    std::vector<std::string> docsContent;
    for (const auto& doc : std::filesystem::directory_iterator{path}) {
        std::string fileToRead = doc.path();
        std::ifstream f(fileToRead);

        if (!f.is_open()) {
            std::cerr << "Error reading " + fileToRead + "\n";
            return {};
        }

        std::string docText = "";
        std::string line;
        while (std::getline(f, line)) {
            docText += line + '\n';
        }

        docsContent.push_back(docText);

        f.close();
    }

    return docsContent;
}

void normalizeDoc(std::string& doc) {
    std::transform(doc.begin(), doc.end(), doc.begin(), [](unsigned char c) { return std::tolower(c); });
}
std::vector<std::string> normalizeDocs(const std::vector<std::string>& rawDocs) {
    std::vector<std::string> normalizedDocs;

    for (const std::string& doc : rawDocs) {
        std::string normalizedDoc = doc;

        normalizedDoc.erase(std::remove_if(normalizedDoc.begin(), normalizedDoc.end(), [](unsigned char c) { return std::ispunct(c); }), normalizedDoc.end());

        normalizeDoc(normalizedDoc);
        normalizedDocs.push_back(normalizedDoc);
    }

    return normalizedDocs;
}

// a string vector for each doc where the strings are the tokens of each doc
// all of those vectors stored in a single vector and returned -> tokenizedDocs
std::vector<std::vector<std::string>> tokenizeDocs(const std::vector<std::string>& normalizedDocs) {
    std::vector<std::vector<std::string>> tokenizedDocs;

    for (const std::string& doc : normalizedDocs) {
        std::vector<std::string> tokenizedDoc;

        std::stringstream ss(doc);
        std::string word;
        while (ss >> word) {
            tokenizedDoc.push_back(word);
        }

        tokenizedDocs.push_back(tokenizedDoc);
    }

    return tokenizedDocs;
}

// finally the actual indexing work

// temp per-doc stats for generating postings
struct TermFreqAndPos {
    int termFreq;
    std::vector<int> positions;
};

InvertedIndex buildInvertedIndex(const std::vector<std::vector<std::string>>& processedDocs) {
    InvertedIndex invertedIndex;

    int docId;
    for (int docIdx = 0; docIdx < processedDocs.size(); docIdx++) {
        docId = docIdx + 1;

        std::unordered_map<std::string, TermFreqAndPos> termFreqAndPosMap;  // term -> {termFreq, positions}
        int currIdx = 0;
        for (const std::string& term : processedDocs[docIdx]) {
            termFreqAndPosMap[term].termFreq++;
            termFreqAndPosMap[term].positions.push_back(currIdx++);
        }

        for (const auto& termFreqAndPosMapEntry : termFreqAndPosMap) {
            const std::string term = termFreqAndPosMapEntry.first;
            const TermFreqAndPos freqAndPos = termFreqAndPosMapEntry.second;

            Posting posting;
            posting.docId = docId;
            posting.termFrequency = freqAndPos.termFreq;
            posting.positions = freqAndPos.positions;

            invertedIndex.index[term].entries.push_back(posting);
            invertedIndex.index[term].totalFrequency += posting.termFrequency;
        }
    }

    return invertedIndex;
}

int main() {
    std::vector<std::string> rawContent = readFromDocs("dummy-data");
    std::vector<std::string> normalizedContent = normalizeDocs(rawContent);
    std::vector<std::vector<std::string>> tokenizedContent = tokenizeDocs(normalizedContent);
    InvertedIndex invidx = buildInvertedIndex(tokenizedContent);

    // for (const auto& doc : tokenizedContent) {
    //     for (const auto& token : doc) {
    //         std::cout << token << ' ';
    //     }
    //     std::cout << '\n';
    // }

    std::cout << "inverted index testing:\n";

    for (const auto& invidxentry : invidx.index) {
        std::cout << "for term: " << invidxentry.first << '\n';
        int i = 0;
        for (const auto& posting : invidxentry.second.entries) {
            std::cout << "for posting #" << i++ << ": " << '\n';
            std::cout << "docId: " << posting.docId << '\n';
            std::cout << "termFrequency: " << posting.termFrequency << '\n';
            std::cout << "positions:\n";
            for (const int& pos : posting.positions) std::cout << pos << ", ";  // leaving trailing commas be since I'm just testing
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    return 0;
}
