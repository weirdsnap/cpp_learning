// LC 877. 石子游戏 — 博客 leetcode/dp/interval/ch57.md
#pragma once

#include <algorithm>
#include <vector>

namespace lc877 {

// 提交版：偶数堆直接返回 true（数学结论），奇数堆走一维滚动 DP
class Solution {
public:
    bool stoneGame(std::vector<int>& piles) {
        int n = piles.size();
        std::vector<int> dp = piles;  // dp[i][i] = piles[i] 压缩进一维

        if (n % 2 == 0) return true;  // 偶数堆先手必胜

        for (int i = n - 2; i >= 0; --i) {
            for (int j = i + 1; j < n; ++j) {
                dp[j] = std::max(piles[i] - dp[j], piles[j] - dp[j - 1]);
                //                    dp[i+1][j]        dp[i][j-1]
            }
        }
        return dp[n - 1] >= 0;
    }
};

// 纯 DP 版：不依赖奇偶结论，任意 n 都适用，返回净胜分
class SolutionDp {
public:
    int stoneGameNet(std::vector<int>& piles) {
        int n = piles.size();
        std::vector<int> dp = piles;
        for (int i = n - 2; i >= 0; --i)
            for (int j = i + 1; j < n; ++j)
                dp[j] = std::max(piles[i] - dp[j], piles[j] - dp[j - 1]);
        return dp[n - 1];
    }
};

// 暴力参照：无记忆化递归，返回净胜分（当前玩家 - 对手）
class SolutionBrute {
public:
    int stoneGameNet(std::vector<int>& piles) {
        return solve(piles, 0, piles.size() - 1);
    }

private:
    int solve(std::vector<int>& piles, int i, int j) {
        if (i == j) return piles[i];
        return std::max(piles[i] - solve(piles, i + 1, j),
                        piles[j] - solve(piles, i, j - 1));
    }
};

} // namespace lc877
