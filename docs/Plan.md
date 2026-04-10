# Early Development Planning

## Project Name

**SeptCrawler**

> [!NOTE]  
> Why? Because "Sept" is the literal English translation of my first name's shortened form (Saptaparno -> Sapto) which is also my nickname. Therefore, I'd like to name my search engine project as SeptCrawler.

## Tech Stack

- **Node.js (I/O Layer):** networking, crawling, API, orchestration
- **C++ (Core Search Engine):** indexing, ranking, heavy computation

> [!NOTE]  
> Since I mainly work with C and C++ (I write C++ like it's C while leveraging some selective C++ features) and Node.js, and Node.js being single-threaded cannot handle Indexing + Ranking alone optimally, so I'll be using both C++ and Node.js for building this search engine project.

## System Architecture

[Architecture.md](./Architecture.md)

---
