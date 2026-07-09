// using dummy docs for now, so:
// dummy docs -> index builder
// later when the crawler and parser are implemented, those will replace this:
// crawler -> parser -> REAL parsed docs -> index builder

/*
 * TODO: read from the dummy data (docs) for now -> normalize and tokenize the text -> build
 * postinglists -> inverted index -> figure out a way to store/persist the inverted index
 * */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> readFromDoc(std::string dirName, std::string fileName) {
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

void normalizeText();
void tokenizeText();
void buildPostingLists();

int main() {
    std::vector<std::string> readLines = readFromDoc("dummy-data", "doc1.txt");

    for (int i = 0; i < readLines.size(); i++) {
        std::cout << readLines[i] << "\n";
    }

    return 0;
}
