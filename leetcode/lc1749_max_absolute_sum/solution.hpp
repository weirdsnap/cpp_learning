// LC 1749. 任意子数组和的绝对值的最大值 — 博客 leetcode/dp/ch38.md
#pragma once

#include <algorithm>
#include <cstdlib>
#include <vector>

namespace lc1749 {

// 双极值 Kadane：同时维护以 i 结尾的最大/最小子数组和，取绝对值较大者
class Solution {
public:
    int maxAbsoluteSum(std::vector<int>& nums) {
        int min_pre = nums[0];
        int max_pre = nums[0];
        int result = std::abs(nums[0]);
        for (int i = 1; i < (int)nums.size(); i++) {
            int cur = nums[i];
            int n_min_pre = std::min({cur, cur + min_pre});
            int n_max_pre = std::max({cur, cur + max_pre});
            result = std::max({result, std::abs(n_max_pre), std::abs(n_min_pre)});
            min_pre = n_min_pre;
            max_pre = n_max_pre;
        }
        return result;
    }
};

} // namespace lc1749
