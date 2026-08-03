// LC 486. 预测赢家 — 博客 leetcode/dp/ch41.md
#pragma once

#include <algorithm>
#include <vector>

namespace lc486 {

// 递推版（区间 DP）：dp[i][j] = 子数组 [i, j] 上当前玩家能获得的最大净胜分
// dp[i][j] = max(nums[i] - dp[i+1][j], nums[j] - dp[i][j-1])
// 按区间长度从小到大填表，保证 len-1 的子区间已就绪
class SolutionDP {
public:
    bool predictTheWinner(std::vector<int>& nums) {
        int n = nums.size();
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

        for (int i = 0; i < n; ++i)
            dp[i][i] = nums[i];

        for (int len = 2; len <= n; ++len) {
            for (int i = 0; i + len - 1 < n; ++i) {
                int j = i + len - 1;
                dp[i][j] = std::max(nums[i] - dp[i + 1][j],
                                    nums[j] - dp[i][j - 1]);
            }
        }

        return dp[0][n - 1] >= 0;
    }
};

// 一维空间优化版：i 从大到小、j 从小到大，
// 读 dp[j] 是上一轮的 dp[i+1][j]，读 dp[j-1] 是本轮刚更新的 dp[i][j-1]
class SolutionDPOpt {
public:
    bool predictTheWinner(std::vector<int>& nums) {
        int n = nums.size();
        std::vector<int> dp = nums;  // dp[i][i] = nums[i]

        for (int i = n - 2; i >= 0; --i) {
            for (int j = i + 1; j < n; ++j) {
                dp[j] = std::max(nums[i] - dp[j], nums[j] - dp[j - 1]);
            }
        }

        return dp[n - 1] >= 0;
    }
};

// 解法：差分视角 + turn 符号技巧，C++23 deducing this 递归 lambda
// 部分编译器不支持显式对象参数，用特性宏保护（GCC 14 定义的是 202110L）
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
#define LC486_HAS_DEDUCING_THIS 1
class Solution {
public:
    bool predictTheWinner(std::vector<int>& nums) {
        auto func = [&](this auto&& self, int i, int j, int turn) -> int {
            if (i == j) return nums[i] * turn;

            int r1 = nums[i] * turn + self(i + 1, j, -turn);
            int r2 = nums[j] * turn + self(i, j - 1, -turn);
            return std::max(r1 * turn, r2 * turn) * turn;
        };

        return func(0, (int)nums.size() - 1, 1) >= 0;
    }
};
#else
#define LC486_HAS_DEDUCING_THIS 0
#endif

} // namespace lc486
