// LC 63. 不同路径 II — 博客 leetcode/dp/path/ch42.md
#pragma once

#include <vector>

namespace lc63 {

// 一维滚动数组版：dp[j] 更新前是 dp[i-1][j]，dp[j-1] 是本行刚算的 dp[i][j-1]
// 障碍物格子必须显式清零，否则旧值残留会让路径"穿墙"
class Solution {
public:
    int uniquePathsWithObstacles(std::vector<std::vector<int>>& obstacleGrid) {
        const int n = obstacleGrid.size();
        const int m = obstacleGrid[0].size();
        std::vector<int> dp(m, 0);

        if (obstacleGrid[0][0] == 1 || obstacleGrid[n - 1][m - 1] == 1)
            return 0;

        dp[0] = 1;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (obstacleGrid[i][j] == 1) {
                    dp[j] = 0;
                } else if (j > 0) {
                    dp[j] += dp[j - 1];
                }
            }
        }

        return dp[m - 1];
    }
};

// 二维参照版：dp[i][j] = dp[i-1][j] + dp[i][j-1]，障碍物为 0
class Solution2D {
public:
    int uniquePathsWithObstacles(std::vector<std::vector<int>>& obstacleGrid) {
        const int n = obstacleGrid.size();
        const int m = obstacleGrid[0].size();
        std::vector<std::vector<int>> dp(n, std::vector<int>(m, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (obstacleGrid[i][j] == 1) continue;
                if (i == 0 && j == 0) dp[i][j] = 1;
                if (i > 0) dp[i][j] += dp[i - 1][j];
                if (j > 0) dp[i][j] += dp[i][j - 1];
            }
        }
        return dp[n - 1][m - 1];
    }
};

} // namespace lc63
