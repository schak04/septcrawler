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
std::vector<std::string> readFromDoc(const std::string& path) {
    for (const auto& dir_entry : std::filesystem::directory_iterator{
             "dummy-data"}) {  // TODO: gotta replace with real docs storage location later
        // to be continued from here
    }
    std::string fileToRead = dirName + "/" + fileName;
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

    f.close();

    return linesRead;
}

// return types and params incomplete, to be thought of and changed while writing these functions
void normalizeText();
void tokenizeText();
void generatePostings();

int main() {
    std::string file = "doc" + std::to_string(1) + ".txt";
    std::vector<std::string> readLines = readFromDoc("dummy-data", file);

    for (int i = 0; i < readLines.size(); i++) {
        std::cout << readLines[i] << "\n";
    }
    readLines.clear();

    std::string file2 = "doc" + std::to_string(2) + ".txt";
    readLines = readFromDoc("dummy-data", file2);

    for (int i = 0; i < readLines.size(); i++) {
        std::cout << readLines[i] << "\n";
    }
    readLines.clear();

    return 0;
}
