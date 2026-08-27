/*
 * DONE: reading from docs, normalization, tokenization, inverted index construction
 *
 * TODO: read from actual docs passed by the HTML parser to process them and build inverted index
 * write inverted index to disk
 * */

#include "core-components/index-builder.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "data-structures/inverted-index.hpp"
#include "data-structures/posting-list.hpp"
#include "data-structures/posting.hpp"

// for future ref: https://cppreference.com/cpp/filesystem
// one string in the vector corresponds to one doc's content
std::vector<std::string> readFromDocs(const std::string& path) {
    std::string targetPath = path;
    if (!std::filesystem::exists(targetPath)) {
        if (std::filesystem::exists("../" + path)) {
            targetPath = "../" + path;
        } else if (std::filesystem::exists("../../" + path)) {
            targetPath = "../../" + path;
        } else {
            return {};
        }
    }

    std::vector<std::string> docsContent;
    for (const auto& doc : std::filesystem::directory_iterator{targetPath}) {
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
    std::transform(doc.begin(), doc.end(), doc.begin(),
                   [](unsigned char c) { return std::tolower(c); });
}

std::vector<std::string> normalizeDocs(const std::vector<std::string>& rawDocs) {
    std::vector<std::string> normalizedDocs;

    for (const std::string& doc : rawDocs) {
        std::string normalizedDoc = doc;

        normalizedDoc.erase(std::remove_if(normalizedDoc.begin(), normalizedDoc.end(),
                                           [](unsigned char c) { return std::ispunct(c); }),
                            normalizedDoc.end());

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
    for (int docIdx = 0; docIdx < static_cast<int>(processedDocs.size()); docIdx++) {
        docId = docIdx + 1;

        std::unordered_map<std::string, TermFreqAndPos>
            termFreqAndPosMap;  // term -> {termFreq, positions}
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

// TODO: inverted index to be written to disk (storage layer)
