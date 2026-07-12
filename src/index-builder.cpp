// using dummy docs for now, so:
// dummy docs -> index builder
// later when the crawler and parser are implemented, those will replace this:
// crawler -> parser -> REAL parsed docs -> index builder

/*
 * TODO: read from the dummy data (docs) for now -> normalize and tokenize the text -> build
 * postinglists -> inverted index -> figure out a way to store/persist the inverted index
 * */

/*
 * TODO: for every doc$.txt where $ = 1 to n, where n = number of docs
 * the readFromDoc function must be able to read each of those and then
 * store the text somewhere so I can manipulate the text (normalize and tokenize them)
 * and then build postings + posting list -> inverted index
 * */

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

// helper for normalizeDocs func
void normalizeDoc(std::string& doc) {
    std::transform(doc.begin(), doc.end(), doc.begin(),
                   [](unsigned char c) { return std::tolower(c); });
}
// a vector of strings containing all the docs' text
// -> normalized (lowercasing only for now)
// -> returned in the form of a vector of strings
std::vector<std::string> normalizeDocs(std::vector<std::string>& rawDocs) {
    std::vector<std::string> normalizedDocs;

    for (std::string& doc : rawDocs) {
        normalizeDoc(doc);
        normalizedDocs.push_back(doc);
    }

    return normalizedDocs;
}

// return types and params incomplete, to be thought of and changed while writing these functions
void tokenizeDocs();
void generatePostings();

int main() {
    std::vector<std::string> rawContent = readFromDocs("dummy-data");
    std::vector<std::string> normalizedContent = normalizeDocs(rawContent);

    for (const std::string& doc : normalizedContent) {
        std::cout << doc << '\n';
    }

    return 0;
}
