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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// for future ref: https://cppreference.com/cpp/filesystem
std::vector<std::vector<std::string>> readFromDocs(const std::string& path) {
    std::vector<std::vector<std::string>> docsMatrix;
    for (const auto& doc : std::filesystem::directory_iterator{path}) {
        std::string fileToRead = doc.path();
        std::ifstream f(fileToRead);

        if (!f.is_open()) {
            std::cerr << "Error reading " + fileToRead + "\n";
            return {};
        }

        std::vector<std::string> linesRead;
        std::string s;
        while (std::getline(f, s)) {
            linesRead.push_back(s);
        }

        docsMatrix.push_back(linesRead);

        f.close();
    }

    return docsMatrix;
}

// return types and params incomplete, to be thought of and changed while writing these functions
void normalizeText();
void tokenizeText();
void generatePostings();

int main() {
    std::vector<std::vector<std::string>> ansMat = readFromDocs("dummy-data");

    for (const std::vector<std::string>& doc : ansMat) {
        for (const std::string& line : doc) {
            std::cout << line << '\n';
        }
        std::cout << '\n';
    }

    return 0;
}
