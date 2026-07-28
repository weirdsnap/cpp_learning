// LC 53. 最大子数组和 — 博客 leetcode/dp/ch19.md
#pragma once

#include <algorithm>
#include <climits>
#include <vector>

namespace lc53 {

// 解法一：DP 数组，dp[i] = 以 i 结尾的连续子数组最大和
class SolutionDP {
public:
    int maxSubArray(std::vector<int>& nums) {
        std::vector<int> dp(nums.begin(), nums.end());
        const int n = nums.size();
        int result = dp[0];
        for (int i = 1; i < n; i++) {
            dp[i] = std::max(dp[i], dp[i - 1] + nums[i]);
            result = std::max(result, dp[i]);
        }
        return result;
    }
};

// 解法二：滚动变量，O(1) 空间
class SolutionRolling {
public:
    int maxSubArray(std::vector<int>& nums) {
        int cur, last;
        const int n = nums.size();
        last = nums[0];
        int result = last;
        for (int i = 1; i < n; i++) {
            cur = std::max(nums[i], last + nums[i]);
            result = std::max(result, cur);
            last = cur;
        }
        return result;
    }
};

// 解法三：前缀和转化，max(pre[i] - min_pre)
class SolutionPrefixSum {
public:
    int maxSubArray(std::vector<int>& nums) {
        int pre = 0;
        int min_pre = 0;
        int result = INT_MIN;
        for (const auto& num : nums) {
            pre += num;
            result = std::max(result, pre - min_pre);
            min_pre = std::min(min_pre, pre);
        }
        return result;
    }
};

} // namespace lc53
