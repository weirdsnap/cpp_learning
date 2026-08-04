// LC 62. 不同路径 — 博客 leetcode/dp/path/ch43.md
#pragma once

#include <algorithm>
#include <vector>

namespace lc62 {

// 一维滚动数组版：交换维度让 dp 数组取较短边
class Solution {
public:
    int uniquePaths(int m, int n) {
        if (m < n) std::swap(m, n);
        std::vector<int> dp(n, 1);
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                dp[j] = dp[j - 1] + dp[j];
            }
        }
        return dp[n - 1];
    }
};

// 二维参照版
class Solution2D {
public:
    int uniquePaths(int m, int n) {
        std::vector<std::vector<int>> dp(m, std::vector<int>(n, 1));
        for (int i = 1; i < m; i++)
            for (int j = 1; j < n; j++)
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        return dp[m - 1][n - 1];
    }
};

// 组合数参照版：C(m + n - 2, m - 1)，边乘边除防溢出
class SolutionMath {
public:
    int uniquePaths(int m, int n) {
        long long res = 1;
        for (int i = 1; i <= m - 1; i++)
            res = res * (n - 1 + i) / i;
        return (int)res;
    }
};

} // namespace lc62
