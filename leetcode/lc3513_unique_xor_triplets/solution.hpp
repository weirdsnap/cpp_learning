// LC 3513. 唯一异或三元组 I — 验证：n ≥ 3 时答案恰为大于 n 的最小 2 的幂
// 对拍：公式版 vs 暴力枚举所有 i ≤ j ≤ k 的 a^b^c 集合
#pragma once

#include <unordered_set>
#include <vector>

namespace lc3513 {

// 公式版：O(log n)
class Solution {
public:
    int uniqueXorTriplets(std::vector<int>& nums) {
        const int n = nums.size();
        if (n == 1) return 1;
        if (n == 2) return 2;
        int m = 1;
        while (m <= n) m <<= 1;  // 大于 n 的最小 2 的幂
        return m;
    }
};

// 暴力参照：真实枚举三元组，统计不同 XOR 值个数，O(n³)
class SolutionBrute {
public:
    int uniqueXorTriplets(std::vector<int>& nums) {
        const int n = nums.size();
        std::unordered_set<int> seen;
        for (int i = 0; i < n; i++)
            for (int j = i; j < n; j++)
                for (int k = j; k < n; k++)
                    seen.insert(nums[i] ^ nums[j] ^ nums[k]);
        return static_cast<int>(seen.size());
    }
};

} // namespace lc3513
