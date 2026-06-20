# Hybrid Search Pipeline

This project implements a self-contained, low-latency hybrid retrieval engine in C++17. It demonstrates the structural integration of traditional lexical search and multi-layered vector space search, fused together using standard ranking algorithms to achieve optimized accuracy and speed.

## Core Architectural Components

### 1. Lexical Search (BM25 Engine)
The keyword retrieval system implements the industry-standard Okapi BM25 scoring function. It tokenizes incoming raw text strings into normalized lowercase terms, builds an in-memory inverted index, tracks dynamic term frequencies across discrete documents, and calculates precise Inverse Document Frequency values. This engine ensures exact keyword matching capabilities across dense datasets.

### 2. Dense Vector Search (Toy HNSW)
The semantic search module utilizes a simplified model of the Hierarchical Navigable Small World algorithm. It organizes high-dimensional embeddings into a multi-layered proximity graph. Fast, top-down greedy routing is executed through sparse express layers down to the base layer. A final localized breadth-first queue traversal over nearest neighbors identifies close semantic vectors using cosine similarity dimensions, minimizing the need for brute-force matrix computations.

### 3. Rank Fusion (RRF Engine)
To combine the disparate scoring mechanisms of the BM25 and vector modules, the pipeline applies Reciprocal Rank Fusion. It entirely bypasses raw score normalization. Instead, it aggregates the reciprocal values of document positions across both sorted search lists, applying a configurable smoothing constant. The document results are reshuffled based on their consistency across both search domains.

## Building and Verification

### Prerequisites
* CMake 3.14 or higher
* A compliant C++17 compiler (such as GCC, Clang, or MSVC)

### Compilation Steps
The project uses automated shell scripts located in the root folder to handle build generation, compilation, and validation testing.

Execute the following from the root directory:
```bash
./build_and_run.bat
