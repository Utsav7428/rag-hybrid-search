#include <iostream>
#include "BM25Engine.h"
#include "ToyHNSW.h"
#include "RRFFusion.h"

int main() {
    search::BM25Engine bm25;
    search::HNSW hnsw;
    search::RRFFusion rrf(60);

    bm25.add_document(101, "Distributed systems replication and consensus protocol.");
    bm25.add_document(102, "High performance vector databases using spatial indexing.");
    bm25.add_document(103, "Centralized architectures handle transaction isolation scaling poorly.");
    bm25.finalize_index();

    hnsw.insert(101, {0.9, 0.1, 0.2}, 2);
    hnsw.insert(102, {0.1, 0.95, 0.4}, 1);
    hnsw.insert(103, {0.3, 0.2, 0.85}, 0);

    std::string text_query = "high performance systems replication";
    search::Vector vector_query = {0.8, 0.4, 0.2}; 

    auto bm25_results = bm25.search_query(text_query, 3);
    auto hnsw_results = hnsw.search_vector(vector_query, 3);
    auto hybrid_results = rrf.fuse(bm25_results, hnsw_results, 3);

    for (const auto& res : hybrid_results) {
        std::cout << "ID: " << res.first << " | RRF: " << res.second << "\n";
    }

    return 0;
}
