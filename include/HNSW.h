#pragma once

#include <vector>
#include <unordered_map>

namespace search {

using Vector = std::vector<double>;

struct HNSWNode {
    int id;
    Vector embedding;
    std::vector<std::vector<int>> neighbors; 
};

class HNSW {
public:
    void insert(int id, const Vector& vec, int assign_level);
    std::vector<std::pair<int, double>> search_vector(const Vector& query, int top_k) const;

private:
    double cosine_similarity(const Vector& a, const Vector& b) const;

    std::unordered_map<int, HNSWNode> nodes;
    int enter_node_id = -1;
    int max_level = 2; 
};

}
  