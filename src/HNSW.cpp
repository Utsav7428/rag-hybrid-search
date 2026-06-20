#include "HNSW.h"
#include <cmath>
#include <algorithm>
#include <queue>
#include <unordered_set>

namespace search {

double HNSW::cosine_similarity(const Vector& a, const Vector& b) const {
    double dot = 0.0, norm_a = 0.0, norm_b = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        dot += a[i] * b[i];
        norm_a += a[i] * a[i];
        norm_b += b[i] * b[i];
    }
    return (norm_a == 0 || norm_b == 0) ? 0.0 : dot / (std::sqrt(norm_a) * std::sqrt(norm_b));
}

void HNSW::insert(int id, const Vector& vec, int assign_level) {
    HNSWNode node;
    node.id = id;
    node.embedding = vec;
    node.neighbors.resize(max_level + 1);
    
    nodes[id] = node;

    if (enter_node_id == -1) {
        enter_node_id = id;
        return;
    }

    for (int l = 0; l <= assign_level; ++l) {
        for (auto& pair : nodes) {
            if (pair.first != id && pair.second.neighbors.size() > static_cast<size_t>(l)) {
                nodes[id].neighbors[l].push_back(pair.first);
                pair.second.neighbors[l].push_back(id);
            }
        }
    }
}

std::vector<std::pair<int, double>> HNSW::search_vector(const Vector& query, int top_k) const {
    if (enter_node_id == -1) return {};

    int curr_node = enter_node_id;
    double curr_dist = cosine_similarity(query, nodes.at(curr_node).embedding);

    for (int l = max_level; l >= 0; --l) {
        bool changed = true;
        while (changed) {
            changed = false;
            for (int neighbor : nodes.at(curr_node).neighbors[l]) {
                double dist = cosine_similarity(query, nodes.at(neighbor).embedding);
                if (dist > curr_dist) {
                    curr_dist = dist;
                    curr_node = neighbor;
                    changed = true;
                }
            }
        }
    }

    std::vector<std::pair<int, double>> local_results;
    std::unordered_set<int> visited;
    std::queue<int> queue;

    queue.push(curr_node);
    visited.insert(curr_node);

    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();
        local_results.push_back({u, cosine_similarity(query, nodes.at(u).embedding)});

        for (int neighbor : nodes.at(u).neighbors[0]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push(neighbor);
            }
        }
    }

    std::sort(local_results.begin(), local_results.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    if (local_results.size() > static_cast<size_t>(top_k)) {
        local_results.resize(top_k);
    }
    return local_results;
}

}
