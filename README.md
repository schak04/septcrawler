# SeptCrawler

This project is a search engine for retrieving learning resources (documentation, references, tutorials, and discussion forums). I'm writing the search engine's core from scratch in C++, with Node.js for the API, crawler & parser. The project is designed to crawl and index learning resources, allowing users to search across them through a focused interface without having them polluted by entertainment-related results.

> [!IMPORTANT]  
> **Project Status: In Active Design + Study/Research Phase; Development Soon To Start**  
> SeptCrawler is a personal learning, research, recreational, and portfolio project being developed independently by [me](#author).  
> Feedback and discussions are always welcome, but the repository is not intended for external contributions.

> [!NOTE]  
> Why "SeptCrawler"?  
> I named this project "SeptCrawler" because, first of all, Sept comes from my name, and second of all, the crawler is where everything begins.

---

## Goal/Motivation

The goal of this project is for me to understand how traditional search engines work internally by building one from scratch.
It is intended to strengthen my backend engineering skills + my programming, researching, and system-designing skills as a whole.
I do not intend this to compete with existing search engines, as this is primarily for my learning of how such systems are designed and implemented.
Along the way, my goal is to make something that could be useful for learners and developers like myself searching for focused learning resources, while not having their flow-state disrupted.

---

## Technology Choices

The decision of using a combination of C++ and Node.js comes from the need to separate the system into performance-critical components and orchestration components.  
The search engine core (including query processing, indexing, retrieval, and ranking) is to be implemented in C++ because these components rely heavily on efficient data structures, memory management, and low-level control.  
Node.js is used for the web-facing and coordination layer, including the crawler, HTML parser, and search API, where networking, asynchronous operations, and ecosystem support are more important than raw performance.  
This separation keeps the architecture modular while allowing each component to use technologies suited to its responsibilities.

> _"But Saptaparno, can you really say you're making this **from scratch** if you're using Node.js for the API layer?"_  
> Well, hear me out...  
> When I say _from scratch_, I'm referring to the **search engine itself**. Not every supporting piece.  
> The core of SeptCrawler, including the inverted index, indexing pipeline, query processing, retrieval engine, ranking pipeline, is being designed and implemented by me from the ground up in C++. Node.js is for writing the supporting components.  
> My goal with this project is to learn how search engines work internally. That means spending my time building the search engine, not rebuilding the supporting components.  
> That doesn't mean those supporting components aren't interesting to me. Quite the opposite. I do want to build things like HTTP servers, parsers, and networking software from scratch as well, but I see those as projects with their own learning goals rather than things to bundle into this one. Hence, I eventually plan to make those separately for my learning (and enjoyment).

---

## System Overview

There are two main pipelines:

1. **Ingestion Pipeline (offline):** Crawls and indexes learning resources from the internet.
2. **Query Pipeline (online):** Processes user queries and returns ranked results.

The system is split into 3 layers:

- **C++ layer:** Core of the search engine
- **Node.js layer:** API + orchestration
- **Storage layer:** Documents + inverted index

---

## Design Decisions

1. **Separation of concerns:** Ingestion is separate from query processing, and retrieval is separate from ranking.
2. **Two independent pipelines:** Offline ingestion vs online query processing.
3. **Index is a database, not a pipeline stage:** It is queried, not passed through.
4. **Retrieval is the mandatory bridge:** Without retrieval, the ranker cannot operate and the index cannot be used meaningfully.

> [!NOTE]  
> When I say **online**, I mean work performed while handling a live user request, and **offline** refers to work performed independently of any live user request, typically in the background to prepare data for future searches.

---

## System Design

### <ins>Textual Documentation</ins>

- [High-Level Design](./docs/design-details/hld.md)
- [Low-Level Design](./docs/design-details/lld.md)

### <ins>Diagrams</ins>

#### <ins>High-Level Design</ins>

- **System Architecture Diagram:**  
  ![System Architecture](./docs/diagrams/system-architecture.png)

- **Data Flow Diagram:**  
  ![Data Flow Diagram](./docs/diagrams/dfd.png)

#### <ins>Low-Level Design</ins>

- **C++ Indexing Internals (Data Structure Diagram):**  
  ![C++ Indexing Internals](./docs/diagrams/dsd.png)

- **Search Execution Sequence (Sequence Diagram):**  
  ![Search Execution Sequence](./docs/diagrams/search-execution-sequence.png)

- **Component Breakdown (Component Diagram):**  
  ![Component Breakdown](./docs/diagrams/component-breakdown.png)

---

## References

- https://www.geeksforgeeks.org/machine-learning/understanding-tf-idf-term-frequency-inverse-document-frequency
- https://cppreference.com/cpp/filesystem

---

## Author

&copy; 2026 [Saptaparno Chakraborty](https://github.com/schak04).  
All rights reserved.

---
