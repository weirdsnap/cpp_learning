// LC 1690. 石子游戏 VII — 博客 leetcode/dp/interval/ch63.md
#pragma once

#include <algorithm>
#include <climits>
#include <vector>

namespace lc1690 {

// 提交版：区间 DP，dp[i][j] = 净胜分
class Solution {
public:
    int stoneGameVII(std::vector<int>& stones) {
        const int n = stones.size();
        std::vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + stones[i];

        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
        for (int i = n - 2; i >= 0; i--) {
            for (int j = i + 1; j < n; j++) {
                int sumRemoveLeft  = prefix[j + 1] - prefix[i + 1];  // 移除左端得分
                int sumRemoveRight = prefix[j] - prefix[i];          // 移除右端得分
                dp[i][j] = std::max(
                    sumRemoveLeft - dp[i + 1][j],
                    sumRemoveRight - dp[i][j - 1]);
            }
        }
        return dp[0][n - 1];
    }
};

// 变体一：一维滚动数组（同 486 解法三的压缩逻辑）
class SolutionRolling {
public:
    int stoneGameVII(std::vector<int>& stones) {
        const int n = stones.size();
        std::vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + stones[i];

        std::vector<int> dp(n, 0);
        for (int i = n - 2; i >= 0; i--) {
            for (int j = i + 1; j < n; j++) {
                int sumRemoveLeft  = prefix[j + 1] - prefix[i + 1];
                int sumRemoveRight = prefix[j] - prefix[i];
                // dp[j] 赋值前是旧值 = dp[i+1][j]；dp[j-1] 本轮已更新 = dp[i][j-1]
                dp[j] = std::max(sumRemoveLeft - dp[j],
                                 sumRemoveRight - dp[j - 1]);
            }
        }
        return dp[n - 1];
    }
};

// 变体二：len 枚举区间
class SolutionLen {
public:
    int stoneGameVII(std::vector<int>& stones) {
        const int n = stones.size();
        std::vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + stones[i];

        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;
                int sumRemoveLeft  = prefix[j + 1] - prefix[i + 1];
                int sumRemoveRight = prefix[j] - prefix[i];
                dp[i][j] = std::max(sumRemoveLeft - dp[i + 1][j],
                                    sumRemoveRight - dp[i][j - 1]);
            }
        }
        return dp[0][n - 1];
    }
};

// 暴力参照：无记忆化递归，直译定义（仅供小规模对拍）
class SolutionBrute {
public:
    int stoneGameVII(std::vector<int>& stones) {
        int n = stones.size();
        pre.assign(n + 1, 0);
        for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + stones[i];
        return solve(0, n - 1);
    }

private:
    std::vector<int> pre;
    int solve(int i, int j) {
        if (i >= j) return 0;
        int sumRemoveLeft  = pre[j + 1] - pre[i + 1];
        int sumRemoveRight = pre[j] - pre[i];
        return std::max(sumRemoveLeft - solve(i + 1, j),
                        sumRemoveRight - solve(i, j - 1));
    }
};

} // namespace lc1690
