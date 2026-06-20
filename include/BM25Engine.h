#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace search {

struct Document {
    int id;
    std::string text;
    std::vector<std::string> tokens;
};

class BM25Engine {
public:
    void add_document(int id, const std::string& text);
    void finalize_index();
    std::vector<std::pair<int, double>> search_query(const std::string& query, int top_k) const;

private:
    std::vector<std::string> tokenize(const std::string& text) const;

    double k1 = 1.5;
    double b = 0.75;
    double avg_dl = 0.0;
    int num_docs = 0;
    
    std::unordered_map<int, int> doc_lens;
    std::unordered_map<std::string, int> df;
    std::unordered_map<std::string, std::unordered_map<int, int>> tf;
    std::vector<Document> documents;
};

}
