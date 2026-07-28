// LC 198. 打家劫舍 — 博客 leetcode/dp/ch31.md
#pragma once

#include <algorithm>
#include <functional>
#include <vector>

namespace lc198 {

// 解法一：记忆化搜索，f(k) = 前 k 间房子的最大金额
class SolutionMemo {
public:
    int rob(std::vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        std::vector<int> dp(n + 1, -1);
        std::function<int(int)> f = [&](int k) -> int {
            if (dp[k] != -1) return dp[k];
            int result;
            if (k == 1) {
                result = nums[0];
            } else if (k == 2) {
                result = std::max(nums[0], nums[1]);
            } else {
                result = std::max(f(k - 1), f(k - 2) + nums[k - 1]);
            }
            dp[k] = result;
            return result;
        };
        return f(n);
    }
};

// 解法二：递推，下标从 1 开始
class SolutionIterative {
public:
    int rob(std::vector<int>& nums) {
        const int n = nums.size();
        std::vector<int> dp(n + 1, 0);
        dp[1] = nums[0];
        for (int i = 2; i <= n; i++) {
            dp[i] = std::max(nums[i - 1] + dp[i - 2], dp[i - 1]);
        }
        return dp[n];
    }
};

// 解法三：显式状态 DP，dp[i][0/1] = 第 i 间不偷/偷
class SolutionExplicit2D {
public:
    int rob(std::vector<int>& nums) {
        const int n = nums.size();
        if (n == 0) return 0;
        std::vector<std::vector<int>> dp(n, std::vector<int>(2, 0));
        dp[0][0] = 0;
        dp[0][1] = nums[0];
        for (int i = 1; i < n; i++) {
            dp[i][0] = std::max(dp[i - 1][0], dp[i - 1][1]);
            dp[i][1] = dp[i - 1][0] + nums[i];
        }
        return std::max(dp[n - 1][0], dp[n - 1][1]);
    }
};

// 解法四：滚动数组，O(1) 空间
class SolutionRolling {
public:
    int rob(std::vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        int prev2 = 0, prev1 = 0;
        for (int i = 0; i < n; i++) {
            int curr = std::max(prev1, prev2 + nums[i]);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};

} // namespace lc198
