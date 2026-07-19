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

/*
 * for each doc in processedDocs
 *      assign docId
 *      build a map: term -> {termFreq, positions}
 *
 *      for each unique term in the map
 *          create a Posting
 *          append the Posting to the PostingList for that term in the InvertedIndex
 * */

struct TermFreqAndPos {
    int termFreq;
    std::vector<int> positions;
};

InvertedIndex buildInvertedIndex(const std::vector<std::vector<std::string>>& processedDocs) {
    int docId;
    for (int docIdx = 0; docIdx < processedDocs.size(); docIdx++) {
        docId = docIdx + 1;

        std::unordered_map<std::string, int> termFreq;  // TODO: term -> {termFreq, positions}
        for (const std::string& token : processedDocs[docIdx]) {
            termFreq[token]++;
        }

        std::vector<int> positions;
        for (int tokenIdx = 0; tokenIdx < processedDocs[docIdx].size(); tokenIdx++) {
            /*
             * WIP:
             * Problem: positions belong to each term, not the document itself
             * A single vector of ints won't do.
             * Need an intermediate per-doc structure that maps each term -> both its freq and its positions.
             * */
        }
    }
}

int main() {
    std::vector<std::string> rawContent = readFromDocs("dummy-data");
    std::vector<std::string> normalizedContent = normalizeDocs(rawContent);
    std::vector<std::vector<std::string>> tokenizedContent = tokenizeDocs(normalizedContent);

    for (const auto& doc : tokenizedContent) {
        for (const auto& token : doc) {
            std::cout << token << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
