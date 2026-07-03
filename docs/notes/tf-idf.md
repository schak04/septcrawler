# TF-IDF

> I'll be keeping my TF-IDF learnings/notes in this file (info I gather from studying about it pre-implementation).

TF-IDF (Term Frequency-Inverse Document Frequency) is a statistical method used in natural language processing and information retrieval to evaluate how important a word is to a document in relation to a larger collection of documents.

It combines 2 components:

1. **Term Frequency (TF):** Measures how often a word appears in a document. A higher frequency suggests greater importance. If a term appears frequently in a document, it is likely relevant to the document’s content.

`TF(t,d) = Number of times term t appears in document d / Total number of terms in document d`

2. **Inverse Document Frequency (IDF):** Reduces the weight of common words across multiple documents while increasing the weight of rare words. If a term appears in fewer documents, it is more likely to be meaningful and specific.

`IDF(t,D) = log(Total number of documents in corpus D / Number of documents containing term t)`
