#include "BM25Engine.h"
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <cctype>

namespace search {

std::vector<std::string> BM25Engine::tokenize(const std::string& text) const {
    std::vector<std::string> res;
    std::string current;
    for (char ch : text) {
        if (std::isalnum(ch)) {
            current += std::tolower(ch);
        } else if (!current.empty()) {
            res.push_back(current);
            current.clear();
        }
    }
    if (!current.empty()) res.push_back(current);
    return res;
}

void BM25Engine::add_document(int id, const std::string& text) {
    auto tokens = tokenize(text);
    documents.push_back({id, text, tokens});
    doc_lens[id] = tokens.size();
    
    std::unordered_set<std::string> unique_tokens(tokens.begin(), tokens.end());
    for (const auto& token : unique_tokens) {
        df[token]++;
    }
    for (const auto& token : tokens) {
        tf[token][id]++;
    }
}

void BM25Engine::finalize_index() {
    num_docs = documents.size();
    double total_len = 0;
    for (const auto& pair : doc_lens) {
        total_len += pair.second;
    }
    avg_dl = num_docs > 0 ? total_len / num_docs : 0.0;
}

std::vector<std::pair<int, double>> BM25Engine::search_query(const std::string& query, int top_k) const {
    auto query_tokens = tokenize(query);
    std::vector<std::pair<int, double>> scores;

    for (const auto& doc : documents) {
        double score = 0.0;
        for (const auto& token : query_tokens) {
            if (df.find(token) == df.end()) continue;
            
            double idf = std::log((num_docs - df.at(token) + 0.5) / (df.at(token) + 0.5) + 1.0);
            int term_freq = tf.at(token).at(doc.id);
            double doc_len = doc_lens.at(doc.id);
            
            double numerator = term_freq * (k1 + 1);
            double denominator = term_freq + k1 * (1.0 - b + b * (doc_len / avg_dl));
            
            score += idf * (numerator / denominator);
        }
        if (score > 0.0) {
            scores.push_back({doc.id, score});
        }
    }

    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    if (scores.size() > static_cast<size_t>(top_k)) {
        scores.resize(top_k);
    }
    return scores;
}

}
