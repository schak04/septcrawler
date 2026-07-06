# Early Development Planning

## Project Name

**SeptCrawler**

> [!NOTE]  
> Why? Because "Sept" is the literal English translation of my first name's shortened form (Saptaparno -> Sapto) which is also my nickname. Therefore, I'd like to name my search engine project as SeptCrawler.

## Tech Stack

- **Node.js (I/O Layer):** networking, crawling, API, orchestration
- **C++ (Core Search Engine):** indexing, ranking, heavy computation

> [!NOTE]  
> Since I mainly work with C++ (and C) and Node.js, I'll build the search engine core (indexing and ranking) in C++ for its performance and low-level control, while using Node.js for the crawler, parser, and API, where its asynchronous I/O model is a good fit.

## System Architecture

[Architecture.md](./Architecture.md)

---
