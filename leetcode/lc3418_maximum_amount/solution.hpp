// LC 3418. 机器人能获得的最大金币数 — 博客 leetcode/dp/path/ch48.md
#pragma once

#include <algorithm>
#include <array>
#include <climits>
#include <vector>

namespace lc3418 {

// 滚动数组版：dp[j][c] 表示到达当前行第 j 列、已用 c 次感化的最大金币
// 关键：每一行内必须按 c 降序更新（2 -> 1 -> 0），否则 dp[j][c-1] 会被本行新值覆盖
class Solution {
public:
    int maximumAmount(std::vector<std::vector<int>>& coins) {
        const int n = coins.size();
        const int m = coins[0].size();
        int dp[500][3];

        // 初始化第一行
        dp[0][0] = coins[0][0];
        dp[0][1] = 0;
        dp[0][2] = 0;
        for (int j = 1; j < m; j++) {
            dp[j][0] = dp[j - 1][0] + coins[0][j];
            dp[j][1] = std::max(dp[j - 1][0], dp[j - 1][1] + coins[0][j]);
            dp[j][2] = std::max(dp[j - 1][1], dp[j - 1][2] + coins[0][j]);
        }

        for (int i = 1; i < n; i++) {
            // 第一列特殊处理：只有"上"一个来源，c 降序更新
            dp[0][2] = std::max(dp[0][2] + coins[i][0], dp[0][1]);
            dp[0][1] = std::max(dp[0][1] + coins[i][0], dp[0][0]);
            dp[0][0] = dp[0][0] + coins[i][0];

            for (int j = 1; j < m; j++) {
                dp[j][2] = std::max({
                    dp[j - 1][2] + coins[i][j],  // 左，感化 2 次已用，照付
                    dp[j - 1][1],                // 左，在本格感化（第 2 次）
                    dp[j][2] + coins[i][j],      // 上，照付
                    dp[j][1]                     // 上，在本格感化
                });
                dp[j][1] = std::max({
                    dp[j - 1][1] + coins[i][j],
                    dp[j - 1][0],
                    dp[j][1] + coins[i][j],
                    dp[j][0]
                });
                dp[j][0] = std::max(dp[j - 1][0], dp[j][0]) + coins[i][j];
            }
        }

        return std::ranges::max(dp[m - 1]);
    }
};

// 三维朴素版（原作者手写）：只对负数格尝试感化，INT_MIN/2 表示不可达
class SolutionPlain {
public:
    int maximumAmount(std::vector<std::vector<int>>& coins) {
        const int n = coins.size();
        const int m = coins[0].size();
        static int dp[500][500][3];  // 3MB，栈上放不下，用 static 挪到 BSS
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                for (int c = 0; c < 3; ++c)
                    dp[i][j][c] = INT_MIN / 2;

        dp[0][0][0] = coins[0][0];
        dp[0][0][1] = (coins[0][0] < 0) ? 0 : coins[0][0];
        dp[0][0][2] = dp[0][0][1];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (i == 0 && j == 0) continue;
                for (int c = 0; c <= 2; ++c) {
                    int best = INT_MIN / 2;
                    if (i > 0) best = std::max(best, dp[i - 1][j][c]);
                    if (j > 0) best = std::max(best, dp[i][j - 1][c]);
                    if (best != INT_MIN / 2)
                        dp[i][j][c] = best + coins[i][j];

                    if (c >= 1 && coins[i][j] < 0) {
                        int best_save = INT_MIN / 2;
                        if (i > 0) best_save = std::max(best_save, dp[i - 1][j][c - 1]);
                        if (j > 0) best_save = std::max(best_save, dp[i][j - 1][c - 1]);
                        if (best_save != INT_MIN / 2)
                            dp[i][j][c] = std::max(dp[i][j][c], best_save);
                    }
                }
            }
        }

        return std::max({dp[n - 1][m - 1][0], dp[n - 1][m - 1][1], dp[n - 1][m - 1][2]});
    }
};

// 三维参照版：dp[i][j][c] 全量存储，逻辑直白
class Solution3D {
public:
    int maximumAmount(std::vector<std::vector<int>>& coins) {
        const int n = coins.size();
        const int m = coins[0].size();
        const int NEG = -1e9;
        std::vector<std::vector<std::array<int, 3>>> dp(
            n, std::vector<std::array<int, 3>>(m, {NEG, NEG, NEG}));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int c = 0; c <= 2; c++) {
                    if (i == 0 && j == 0) {
                        dp[0][0][c] = (c == 0) ? coins[0][0] : std::max(coins[0][0], 0);
                        continue;
                    }
                    int best = NEG;
                    // 照付：同化次数不变，从上方/左方同 c 转移
                    if (i > 0 && dp[i - 1][j][c] != NEG)
                        best = std::max(best, dp[i - 1][j][c] + coins[i][j]);
                    if (j > 0 && dp[i][j - 1][c] != NEG)
                        best = std::max(best, dp[i][j - 1][c] + coins[i][j]);
                    // 感化：同化次数 -1 转移过来，本格取 0
                    if (c > 0) {
                        if (i > 0 && dp[i - 1][j][c - 1] != NEG)
                            best = std::max(best, dp[i - 1][j][c - 1]);
                        if (j > 0 && dp[i][j - 1][c - 1] != NEG)
                            best = std::max(best, dp[i][j - 1][c - 1]);
                    }
                    dp[i][j][c] = best;
                }
            }
        }
        return std::ranges::max(dp[n - 1][m - 1]);
    }
};

} // namespace lc3418
