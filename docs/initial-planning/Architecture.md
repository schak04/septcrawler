# System Architecture

## Layer 1: Node.js (API & Orchestration Layer)

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
- Implementing TF-IDF (v1; BM25 considered for future iterations)
- Query processing and scoring

Gives:
- Memory control
- Speed
- Data structure optimization
- Real IR learning depth

## Layer 3: Storage

Handles:
- Raw Documents storage
- Persistent Inverted Index storage

---
