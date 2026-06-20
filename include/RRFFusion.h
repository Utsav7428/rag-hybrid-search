#pragma once

#include <vector>

namespace search {

class RRFFusion {
public:
    explicit RRFFusion(int k_val = 60);

    std::vector<std::pair<int, double>> fuse(
        const std::vector<std::pair<int, double>>& list_a, 
        const std::vector<std::pair<int, double>>& list_b, 
        int top_k) const;

private:
    int k;
};

}
