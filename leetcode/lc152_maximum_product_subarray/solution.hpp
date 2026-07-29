// LC 152. 乘积最大子数组 — 博客 leetcode/dp/ch37.md
#pragma once

#include <algorithm>
#include <vector>

namespace lc152 {

// 同时维护以 i 结尾的最大/最小乘积（负负得正，最小值可能翻盘）
class Solution {
public:
    int maxProduct(std::vector<int>& nums) {
        int max_pre = nums[0];
        int min_pre = nums[0];
        int result = nums[0];
        for (int i = 1; i < (int)nums.size(); i++) {
            int n_max_pre = std::max(
                {nums[i], max_pre * nums[i], min_pre * nums[i]});
            int n_min_pre = std::min(
                {nums[i], max_pre * nums[i], min_pre * nums[i]});
            result = std::max(result, n_max_pre);
            max_pre = n_max_pre;
            min_pre = n_min_pre;
        }
        return result;
    }
};

} // namespace lc152
