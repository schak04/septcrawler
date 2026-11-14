# Requirements

> [!NOTE]  
> These are the requirements of the system that I listed out in the design phase but did not keep under the docs directory earlier.
> This document is highly likely to receive updates as the system improves.

## Functional

- An ingestion-pipeline-running binary (separate from the main search engine binary) that crawls a given website (`cppreference` by default if no argument provided), processes them into documents, stores raw documents for later use, and sends the documents to the search engine core, specifically the index builder, which then constructs the inverted index and persists it (as JSON initially).
- An interface or medium (the search API, and preferably also a CLI that lets the user interact with the search API) that allows users to search for a query.
- Ranked results in the form of a list of URLs (plus additional data) as output.

> Out of Scope for the MVP: Ingestion automation, advanced ranking algorithms, etc.

## Non-functional

- Searching and ingestion should be low-latency operations. Ideally, 5 seconds maximum for ingestion, and 2 seconds maximum for search execution.

---
