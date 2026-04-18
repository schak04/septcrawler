# System Architecture

## Layer 1: Node.js (I/O layer)

Handles:
- Crawling pages
- Parsing HTML
- Storing raw cleaned documents
- Sending documents to C++ indexer
- Exposing `/search` API

<!-- > as Node.js excels at async network work -->

## Layer 2: C++ (Core Search Engine)

Handles:
- Building inverted index
- Storing postings lists
- Implementing TF-IDF / BM25
- Query processing and scoring

Gives:
- Memory control
- Speed
- Data structure optimization
- Real IR learning depth

---
