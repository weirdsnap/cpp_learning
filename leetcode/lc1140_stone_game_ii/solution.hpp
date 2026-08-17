// LC 1140. 石子游戏 II — 博客 leetcode/dp/interval/ch58.md
#pragma once

#include <algorithm>
#include <climits>
#include <numeric>
#include <vector>

namespace lc1140 {

// 提交版：自底向上 DP，dp[i][m] 为净胜分，返回答案时换算成 Alice 的石子数
class Solution {
public:
    int stoneGameII(std::vector<int>& piles) {
        int n = piles.size();
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, INT_MIN));
        for (int i = n; i >= 0; i--) {
            for (int m = 1; m <= n; m++) {
                if (i == n) {
                    dp[i][m] = 0;
                } else {
                    int sum = 0;
                    for (int x = 1; x <= 2 * m; x++) {
                        if (i + x > n) break;
                        sum += piles[i + x - 1];
                        dp[i][m] = std::max(dp[i][m], sum - dp[i + x][std::min(n, std::max(m, x))]);
                    }
                }
            }
        }
        return (dp[0][1] + std::accumulate(piles.begin(), piles.end(), 0)) / 2;
    }
};

// 参照版：记忆化搜索，同样返回净胜分，逻辑直译定义
class SolutionMemo {
public:
    int stoneGameII(std::vector<int>& piles) {
        n = piles.size();
        p = &piles;
        memo.assign(n + 1, std::vector<int>(n + 1, INT_MIN));
        int net = solve(0, 1);
        int total = std::accumulate(piles.begin(), piles.end(), 0);
        return (net + total) / 2;
    }

private:
    int n;
    std::vector<int>* p;
    std::vector<std::vector<int>> memo;

    int solve(int i, int m) {
        if (i == n) return 0;
        int& res = memo[i][m];
        if (res != INT_MIN) return res;
        int sum = 0;
        for (int x = 1; x <= 2 * m && i + x <= n; x++) {
            sum += (*p)[i + x - 1];
            res = std::max(res, sum - solve(i + x, std::min(n, std::max(m, x))));
        }
        return res;
    }
};

} // namespace lc1140
