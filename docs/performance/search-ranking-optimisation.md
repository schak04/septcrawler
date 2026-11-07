# TF-IDF Search Ranking Optimisation

## Optimisation History

### Score and Statistics Precomputation

In the MVP (v0.1.0), the `std::sort` comparator calculated both scores every time it compared two candidate documents. Each calculation traversed the full inverted index to rebuild corpus statistics, then searched each query term's posting list for the candidate. Since `std::sort` invokes its comparator repeatedly, this work dominated ranking.

`CorpusStatistics` stores each document's total term frequency and the set of distinct document IDs. `buildCorpusStatistics` calculates both in one traversal of all postings. `rankCandidateDocuments` now builds these statistics once per ranking operation, scores each candidate once, sorts candidate/score pairs by their stored scores, and returns the documents in that order.

### Query-term Posting Lookup

Posting lists are vectors, and their order is not guaranteed for indexes loaded through the bridge. The ranker now builds a docID-to-termFrequency hash map for each query term once per ranking operation. Candidate scoring uses these maps instead of scanning every posting list for every candidate. The lookup data is local to ranking and covers only query terms, leaving the shared posting-list representation and retrieval code unchanged.

The statistics-aware `calcTFIDFScore` overload creates query-term lookup data for each call. The `rankCandidateDocuments` path builds it once and reuses it for all candidates. The two-argument compatibility overload remains, rebuilding corpus statistics and query-term lookup data on every call.

## Complexity

Let:

- `P` = total number of postings in the inverted index
- `N` = number of candidate documents
- `Q` = number of query terms
- `L` = average length of a query term's posting list

The original comparator recalculated scores across `O(N log N)` comparisons. Each score rebuilt statistics in `O(P)` and scanned query posting lists in `O(QL)`, giving:

`O(N log N x P + N log N x QL)`

The first optimisation (score and statistics precomputation) built corpus statistics once, scored each candidate by scanning query posting lists, and sorted the precomputed scores:

`O(P + NQL + N log N)`

The second optimisation (query-term posting lookup) builds statistics and query-term maps once, then performs expected constant-time hash lookups for each candidate/query-term pair:

`O(P + QL + NQ + N log N)`

These bounds assume expected constant-time `unordered_map` and `unordered_set` operations. Map construction covers at most the posting lists for the query's distinct terms, and the query lookup maps use `O(QL)` additional memory. These expressions exclude index loading, retrieval, and output. The compatibility overload performs the `O(P + QL)` setup on each call.

## Benchmark Evidence

**Environment:** x86_64 Linux, kernel `7.1.3-arch1-3`, GCC 16.1.1, Go 1.26.5. Both C++ builds used the Makefile settings `-std=c++20 -Wall -Wextra`.

### 1. Corpus Statistics and Candidate Score Precomputation

The original implementation (v0.1.0) recalculated TF-IDF scores inside the `std::sort` comparator. Each score calculation rebuilt corpus statistics by traversing the entire inverted index, repeating this work across sorting comparisons. The optimised implementation builds corpus statistics once per ranking operation, calculates each candidate's score once, and sorts using the precomputed scores.

I compared the original and optimised executables over five runs using the same index and the `vector clear` query.

| Version   | Runs | Median wall time | Results |
| --------- | ---: | ---------------: | ------: |
| Original  |    5 |   10.743 seconds |      24 |
| Optimised |    5 |    1.047 seconds |      24 |

Both versions produced identical results in the same ranking order. This represents approximately a **90.3% reduction in median end-to-end search time**, or roughly **10.3x faster execution**. The measurements include index loading, retrieval, ranking, and output, so they show the end-to-end improvement but do not isolate ranking performance.

Earlier preliminary measurements recorded a 7.91-second original median and one optimised run at 1.027 seconds. I have kept these separate because they used different sample counts.

The original benchmark output and methodology are documented here: [score precomputation benchmark report](./benchmarks/score-precomputation.md)

### 2. Query-Term Posting Lookup

The next optimisation replaced repeated linear scans through query-term posting lists with docID-to-termFrequency hash maps built once per ranking operation. The pre-lookup executable already included corpus statistics and candidate-score precomputation, so this comparison measures the additional query-term lookup change.

I ran both versions with the same index, documents, machine, and build settings. Each query ran five times per version, alternating execution order between repetitions.

| Query          | Version          | Runs | Median wall time |               Range | Results |
| -------------- | ---------------- | ---: | ---------------: | ------------------: | ------: |
| `vector clear` | Pre-lookup       |    5 |    1.097 seconds | 1.061-1.136 seconds |      24 |
| `vector clear` | Lookup optimised |    5 |    1.090 seconds | 1.071-1.112 seconds |      24 |
| `template`     | Pre-lookup       |    5 |    1.082 seconds | 1.074-1.111 seconds |     184 |
| `template`     | Lookup optimised |    5 |    1.095 seconds | 1.053-1.098 seconds |     184 |

Every run produced byte-for-byte identical output between versions, including result order.

The end-to-end timings are close and show no measurable CLI-level improvement. Index loading and other operations dominate, so these measurements do not isolate candidate-scoring performance.

The complete command output and per-run measurements are documented here: [candidate posting lookup benchmark report](./benchmarks/candidate-posting-lookup.md)

## Remaining work

The two-argument compatibility overload still rebuilds corpus statistics and query-term lookup maps on every call. Callers calculating scores for multiple candidates should use the ranking path, which shares both structures. A dedicated ranker benchmark that excludes index loading would measure the lookup change directly.
