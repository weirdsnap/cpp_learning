// LC 624. 数组中的最大距离 — 博客 leetcode/greedy/ch55.md
#pragma once

#include <algorithm>
#include <climits>
#include <vector>

namespace lc624 {

// 贪心：滚动维护已见数组的全局最小/最大值，O(m)，m 为数组个数
class Solution {
public:
    int maxDistance(std::vector<std::vector<int>>& arrays) {
        int result = 0;
        int last_min = arrays[0].front();
        int last_max = arrays[0].back();
        for (size_t i = 1; i < arrays.size(); ++i) {
            auto& array = arrays[i];              // 引用避免拷贝整个 vector
            int min_num = array.front();          // 每个数组已升序，首尾即最值
            int max_num = array.back();
            result = std::max(result, max_num - last_min);
            result = std::max(result, last_max - min_num);
            last_min = std::min(last_min, min_num);
            last_max = std::max(last_max, max_num);
        }
        return result;
    }
};

// 暴力参照：枚举所有跨数组数对，O(总元素数的平方)
class SolutionBrute {
public:
    int maxDistance(std::vector<std::vector<int>>& arrays) {
        int best = 0;
        for (size_t i = 0; i < arrays.size(); ++i)
            for (size_t j = 0; j < arrays.size(); ++j) {
                if (i == j) continue;
                for (int a : arrays[i])
                    for (int b : arrays[j])
                        best = std::max(best, std::abs(a - b));
            }
        return best;
    }
};

} // namespace lc624
