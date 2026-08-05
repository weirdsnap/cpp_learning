// LC 3742. 网格中的最大路径得分 — 博客 leetcode/dp/path/ch46.md
#pragma once

#include <algorithm>
#include <vector>

namespace lc3742 {

// 三维参照版：dp[i][j][c] = 到 (i,j) 花费恰好 c 的最大得分
class Solution3D {
public:
    int maxPathScore(std::vector<std::vector<int>>& grid, int k) {
        const int rows = grid.size(), cols = grid[0].size();
        int maxCost = std::min(k, rows + cols - 2);
        std::vector dp(rows, std::vector(cols, std::vector<int>(maxCost + 1, -1)));
        dp[0][0][0] = 0;  // 题意保证 grid[0][0] == 0
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (i == 0 && j == 0) continue;
                int cost = grid[i][j] ? 1 : 0, score = grid[i][j];
                for (int c = cost; c <= maxCost; c++) {
                    int best = -1;
                    if (i > 0 && dp[i - 1][j][c - cost] != -1)
                        best = std::max(best, dp[i - 1][j][c - cost] + score);
                    if (j > 0 && dp[i][j - 1][c - cost] != -1)
                        best = std::max(best, dp[i][j - 1][c - cost] + score);
                    dp[i][j][c] = best;
                }
            }
        }
        int ans = -1;
        for (int c = 0; c <= maxCost; c++)
            ans = std::max(ans, dp[rows - 1][cols - 1][c]);
        return ans;
    }
};

// ⚠️ 反例保留：滚动数组版（用户原代码，含 lastRow 奇偶选择 bug）
// bug：swap 条件是 i < rows - 1，最后一行算完没有 swap，答案永远在 cur 里；
// 奇偶公式 (rows - 1) % 2 == 0 ? cur : prev 在偶数行时错读上一行。
// 实测：样例 1 返回 1（应为 2），500 组随机对拍错 131 组。勿抄，用修正版。
class SolutionRolling {
public:
    int maxPathScore(std::vector<std::vector<int>>& grid, int k) {
        const int rows = grid.size();
        const int cols = grid[0].size();
        int maxCost = std::min(k, rows + cols - 2);
        std::vector<std::vector<int>> prev(cols, std::vector<int>(maxCost + 1, -1));
        std::vector<std::vector<int>> cur(cols, std::vector<int>(maxCost + 1, -1));

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int cost = (grid[i][j] == 0 ? 0 : 1);
                int score = grid[i][j];

                fill(cur[j].begin(), cur[j].end(), -1);

                if (i == 0 && j == 0) {
                    if (cost <= maxCost) cur[j][cost] = score;
                    continue;
                }

                for (int c = cost; c <= maxCost; c++) {
                    int best = -1;
                    if (i > 0 && prev[j][c - cost] != -1)
                        best = std::max(best, prev[j][c - cost] + score);
                    if (j > 0 && cur[j - 1][c - cost] != -1)
                        best = std::max(best, cur[j - 1][c - cost] + score);
                    cur[j][c] = best;
                }
            }
            if (i < rows - 1) std::swap(prev, cur);
        }

        int ans = -1;
        auto& lastRow = (rows - 1) % 2 == 0 ? cur : prev;
        for (int c = 0; c <= maxCost; c++)
            ans = std::max(ans, lastRow[cols - 1][c]);
        return ans;
    }
};

// 滚动数组修正版：最后一行算完没有 swap，答案永远在 cur 里
class SolutionRollingFixed {
public:
    int maxPathScore(std::vector<std::vector<int>>& grid, int k) {
        const int rows = grid.size();
        const int cols = grid[0].size();
        int maxCost = std::min(k, rows + cols - 2);
        std::vector<std::vector<int>> prev(cols, std::vector<int>(maxCost + 1, -1));
        std::vector<std::vector<int>> cur(cols, std::vector<int>(maxCost + 1, -1));

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int cost = (grid[i][j] == 0 ? 0 : 1);
                int score = grid[i][j];

                fill(cur[j].begin(), cur[j].end(), -1);

                if (i == 0 && j == 0) {
                    if (cost <= maxCost) cur[j][cost] = score;
                    continue;
                }

                for (int c = cost; c <= maxCost; c++) {
                    int best = -1;
                    if (i > 0 && prev[j][c - cost] != -1)
                        best = std::max(best, prev[j][c - cost] + score);
                    if (j > 0 && cur[j - 1][c - cost] != -1)
                        best = std::max(best, cur[j - 1][c - cost] + score);
                    cur[j][c] = best;
                }
            }
            if (i < rows - 1) std::swap(prev, cur);
        }

        int ans = -1;
        for (int c = 0; c <= maxCost; c++)
            ans = std::max(ans, cur[cols - 1][c]);  // 最后一行算完没有 swap
        return ans;
    }
};

} // namespace lc3742

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
#define LC3742_HAS_DEDUCING_THIS 1
namespace lc3742 {

// 记忆化搜索版（用户原代码，this auto self 按值传闭包）
class SolutionMemo {
public:
    int maxPathScore(std::vector<std::vector<int>>& grid, int k) {
        const int rows = grid.size();
        const int cols = grid[0].size();
        int maxCost = std::min(k, rows + cols - 2);
        std::vector<std::vector<std::vector<int>>> dp(
            rows, std::vector<std::vector<int>>(cols, std::vector<int>(maxCost + 1, -1)));

        int startCost = (grid[0][0] == 0 ? 0 : 1);
        int startScore = grid[0][0];
        if (startCost <= maxCost) dp[0][0][startCost] = startScore;

        auto func = [&](this auto self, int i, int j, int c) -> int {
            if (c < 0 || c > maxCost) return -1;
            if (dp[i][j][c] != -1) return dp[i][j][c];

            int curCost = (grid[i][j] == 0 ? 0 : 1);
            int curScore = grid[i][j];
            if (c < curCost) {
                dp[i][j][c] = -1;
                return -1;
            }

            int best = -1;
            if (i > 0) {
                int prev = self(i - 1, j, c - curCost);
                if (prev != -1) best = std::max(best, prev + curScore);
            }
            if (j > 0) {
                int prev = self(i, j - 1, c - curCost);
                if (prev != -1) best = std::max(best, prev + curScore);
            }
            dp[i][j][c] = best;
            return best;
        };

        int ans = -1;
        for (int c = 0; c <= maxCost; c++) {
            int score = func(rows - 1, cols - 1, c);
            if (score != -1) ans = std::max(ans, score);
        }
        return ans;
    }
};

} // namespace lc3742
#else
#define LC3742_HAS_DEDUCING_THIS 0
#endif
