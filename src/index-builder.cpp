// using dummy docs for now, so:
// dummy docs -> index builder
// later when the crawler and parser are implemented, those will replace this:
// crawler -> parser -> REAL parsed docs -> index builder

/*
 * TODO: read from the dummy data (docs) for now -> normalize and tokenize the text -> build
 * postinglists -> inverted index -> figure out a way to store/persist the inverted index
 * */

#include <fstream>

void readFromDocs();
void normalizeText();
void tokenizeText();
void buildPostingLists();
