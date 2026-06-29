# High-Level Design

This document describes the high-level architecture of my search engine. In this, I'll mainly be documenting the major components, how they interact with each other, and how data flows between and through them.

The system is divided into 2 independent pipelines: an offline ingestion pipeline responsible for crawling and indexing documents, and an online query pipeline responsible for processing search requests. These pipelines share the `Persistent Inverted Index` but otherwise operate independently.

## Architecture Components

### Client Layer

- **User <-> (Web Browser / CLI):** Sends search queries and receives ranked results.

### Node.js API & Orchestration Layer

> This layer is responsible for coordination, not heavy computation.

- **Search API:** Entry point for all queries. Sends queries to the C++ engine and returns final results at the end.
- **Web Crawler:** Responsible for fetching target resources during ingestion.
- **HTML Parser:** Extracts usable text and metadata from HTML.

### C++ Search Engine Layer

> This layer contains the core search engine logic and is responsible for indexing, retrieval, and ranking operations.

- **Query Processor:** Normalizes (lowercase, cleanup) and tokenizes query.
- **Retrieval Engine:** Looks up tokens in the inverted index, fetches posting lists, merges/intersects results, and produces candidate documents for ranking.
- **Ranker:** Ranks candidate documents and orders results by relevance.
- **Index Builder:** Builds the inverted index by receiving parsed documents, tokenizing them, and writing to disk.

### Storage Layer

- **Raw Documents Storage:** Stores cleaned/extracted documents for reference and indexing.
- **Persistent Inverted Index Storage:** Stores the inverted index on disk for retrieval during query execution.

---

## Data Flow

### Query Flow (Runtime System)

`Query -> normalized query -> tokens -> postings -> candidate docs -> ranked results`

1. A user enters a query -> the client sends a request to the Search API.
2. The API forwards the query to the Query Processor.
3. The Query Processor normalizes and tokenizes the query.
4. The Retrieval Engine uses tokens to look up inverted index, fetches posting lists, and merges the results into candidate docs.
5. The Ranker scores the candidate documents and sorts them by relevance.
6. The API returns ranked results to the user.

### Ingestion Flow (Index Building System)

`Web -> HTML -> clean docs -> tokens -> inverted index`

1. The Crawler fetches web pages.
2. The HTML Parser extracts structured text from them.
3. Raw documents are stored in the `Raw Documents` storage.
4. The Index Builder then normalizes all the text, tokenizes the content, and builds the inverted index.
5. The inverted index is stored in the `Persistent Inverted Index` storage.

---
