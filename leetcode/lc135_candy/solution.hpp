#pragma once

#include <numeric>
#include <vector>

namespace lc135 {

// 两次扫描
class SolutionTwoPass {
public:
    int candy(std::vector<int>& ratings) {
        const int n = static_cast<int>(ratings.size());
        if (n == 0) return 0;

        std::vector<int> extra(n, 0);
        for (int i = 1; i < n; ++i) {
            if (ratings[i] > ratings[i - 1]) {
                extra[i] = extra[i - 1] + 1;
            }
        }
        for (int i = n - 2; i >= 0; --i) {
            if (ratings[i] > ratings[i + 1]) {
                extra[i] = std::max(extra[i], extra[i + 1] + 1);
            }
        }
        return std::accumulate(extra.begin(), extra.end(), 0) + n;
    }
};

// 单次遍历 O(1) 空间
class SolutionOnePass {
public:
    int candy(std::vector<int>& ratings) {
        const int n = static_cast<int>(ratings.size());
        if (n == 0) return 0;

        int candies = 1;
        int up = 0;
        int down = 0;
        int peak = 0;

        for (int i = 1; i < n; ++i) {
            if (ratings[i] > ratings[i - 1]) {
                ++up;
                down = 0;
                peak = up;
                candies += 1 + up;
            } else if (ratings[i] < ratings[i - 1]) {
                ++down;
                up = 0;
                candies += down;
                if (down > peak) {
                    ++candies;
                }
            } else {
                up = 0;
                down = 0;
                peak = 0;
                ++candies;
            }
        }
        return candies;
    }
};

} // namespace lc135
