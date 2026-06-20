#include "RRFFusion.h"
#include <unordered_map>
#include <algorithm>

namespace search {

RRFFusion::RRFFusion(int k_val) : k(k_val) {}

std::vector<std::pair<int, double>> RRFFusion::fuse(
    const std::vector<std::pair<int, double>>& list_a, 
    const std::vector<std::pair<int, double>>& list_b, 
    int top_k) const 
{
    std::unordered_map<int, double> rrf_scores;

    for (size_t rank = 0; rank < list_a.size(); ++rank) {
        int doc_id = list_a[rank].first;
        rrf_scores[doc_id] += 1.0 / (k + (rank + 1));
    }

    for (size_t rank = 0; rank < list_b.size(); ++rank) {
        int doc_id = list_b[rank].first;
        rrf_scores[doc_id] += 1.0 / (k + (rank + 1));
    }

    std::vector<std::pair<int, double>> final_ranking(rrf_scores.begin(), rrf_scores.end());
    std::sort(final_ranking.begin(), final_ranking.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    if (final_ranking.size() > static_cast<size_t>(top_k)) {
        final_ranking.resize(top_k);
    }
    return final_ranking;
}

}
