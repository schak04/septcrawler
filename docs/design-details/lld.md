# Low-Level Design

This document describes the internal design of my search engine. In this, I'll be documenting the planned internals, so as to have a clear structure in my mind before starting the development phase.

> I'll be writing code snippets for illustration, but they may be subject to change while coding.

---

# Data Structures

## Inverted Index

The inverted index is the core data structure of this search engine.  
It stores: `term -> documents`. This allows fast lookup.

```cpp
class InvertedIndex {
    unordered_map<string, PostingList> index;
};
```

---

## Posting List

A posting list contains all the documents in which a term appears.

```cpp
class PostingList {
    vector<Posting> entries;
    int totalFrequency;
};
```

---

## Posting

A posting represents a single entry in a posting list. It stores information about **one term in one document**.

> One posting for every (term, document) pair.

```cpp
class Posting {
    int docId;
    int termFrequency;
    vector<int> positions;
};
```

### Stored information

- **docId:** Unique identifier of the document.
- **termFrequency:** Number of times the term appears in the document.
- **positions:** Locations where the term appears. A term, say "cmake", appears at positions: 2, 17, 44. It will be stored as: `{2, 17, 44}`.

> [!NOTE]  
> Each term in the inverted index maps to exactly one posting list.  
> Each posting list contains one posting for every document in which that term appears.  
> Each posting stores information about that **same term in one specific document**.  
> So, conceptually:  
> term  
> |-> PostingList  
> ....... |-> Posting(doc1, tf=3, positions={...})  
> ....... |-> Posting(doc4, tf=2, positions={...})  
> ....... |-> Posting(doc8, tf=6, positions={...})
>
> A document can therefore appear in many posting lists, one for each term it contains.

---

## Candidate Documents

Candidate documents are generated dynamically during query execution.  
They are **not stored** anywhere in the storage layer.

```cpp
struct CandidateDocument {
    int docId;
    int matchedTerms;
    // additional lightweight statistics used for ranking
};
```

> The retrieval engine retrieves the posting list for each query term, merges or intersects them, and produces a set of candidate documents for the ranker. Then, the ranker scores them to determine the final search results.

---

# Index Construction

The ingestion pipeline builds the inverted index through the following stages:

`Web -> Crawler -> HTML Parser (-> Raw Documents Storage) -> Index Builder (Normalize -> Tokenize -> Generate Postings) -> Inverted Index -> Persistent Storage`

> Normalization, tokenization, and posting generation are internal steps of the Index Builder, not separate components.

During indexing:

1. Documents are fetched by the crawler.
2. HTML content is parsed into usable text.
3. Clean documents are written to Raw Documents storage.
4. The Index Builder normalizes the text, tokenizes it, generates posting lists, and constructs the inverted index.
5. The inverted index is written to persistent storage.

---

# Query Execution

When a user submits a query, the search engine executes the following sequence.

Example query: `GNU Debugger`

Execution flow: `User -> Search API -> Query Processor -> Retrieval Engine (<-> Inverted Index) -> Ranker -> Results`

1. The Search API receives the query.
2. The Query Processor normalizes the query. Example: "GNU Debugger" becomes "gnu debugger".
3. It then tokenizes the query. So: `{"gnu", "debugger"}`
4. The Retrieval Engine looks up each query term in the inverted index.
5. It then merges posting lists and generates candidate documents together with lightweight statistics required for ranking.
6. The Ranker scores each candidate document.

> I plan to use TF-IDF in v1 as the initial ranking algorithm.

7. Candidate documents are sorted by relevance and returned to the user.

---

# Module Responsibilities

## Crawler

- Fetching web pages
- Passing downloaded pages to the HTML parser

---

## HTML Parser

- Extracting text
- Extracting useful metadata
- Producing clean documents for indexing
- Writing clean documents to Raw Documents storage

---

## Index Builder

- Normalizing document text
- Tokenizing documents
- Generating postings and posting lists
- Constructing the inverted index
- Writing the index to persistent storage

---

## Query Processor

- Query normalization
- Query tokenization

---

## Retrieval Engine

- Reading the inverted index
- Looking up posting lists
- Merging posting lists
- Producing candidate documents

> The Retrieval Engine is the only component that directly accesses the inverted index during query execution.

---

## Ranker

- Scoring candidate documents
- Ordering results by relevance

> The Ranker operates only on candidate documents produced by the Retrieval Engine and does not directly access the inverted index.

---

# Design Principles

## Separation of Concerns

Each module has a single responsibility, as described in the [Module Responsibilities](#module-responsibilities) section.

---

## Retrieval Before Ranking

Ranking cannot begin until the retrieval stage has produced candidate documents.  
Retrieval determines **which documents should be considered**, while ranking determines **their order of relevance**.

---

## Read & Write Separation for Persistent Inverted Index Storage

During query execution, the inverted index is treated as read-only. Writing to it is only done during ingestion.  
This separation simplifies responsibilities and prevents the query pipeline from modifying indexed data.

---

## Persistent Inverted Index

The inverted index is written to persistent storage after construction and loaded by the search engine during query execution.  
This avoids rebuilding the index every time the application starts and enables efficient repeated searches.

---
