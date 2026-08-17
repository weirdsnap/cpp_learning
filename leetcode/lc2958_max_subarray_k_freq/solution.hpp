// LC 2958. 长度最小的子数组中每个元素最多出现 K 次 — 博客 leetcode/array/ch53.md
#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace lc2958 {

// 滑动窗口版（用户实现）
class Solution {
public:
    int maxSubarrayLength(std::vector<int>& nums, int k) {
        const int n = nums.size();
        int i = 0, j = 1, len = 1;

        std::unordered_map<int, int> cnt;
        cnt[nums[0]] = 1;
        while (j < n) {
            cnt[nums[j]] += 1;
            while (cnt[nums[j]] > k) {
                cnt[nums[i]] -= 1;
                i++;
            }
            len = std::max(j - i + 1, len);
            j++;
        }
        return len;
    }
};

// 暴力参照版：枚举所有子数组，检查每元素频率 ≤ k
class SolutionBrute {
public:
    int maxSubarrayLength(std::vector<int>& nums, int k) {
        int n = nums.size(), best = 0;
        for (int i = 0; i < n; i++) {
            std::unordered_map<int, int> cnt;
            for (int j = i; j < n; j++) {
                if (++cnt[nums[j]] > k) break;
                best = std::max(best, j - i + 1);
            }
        }
        return best;
    }
};

} // namespace lc2958
