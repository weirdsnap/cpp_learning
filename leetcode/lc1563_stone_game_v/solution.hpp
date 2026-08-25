// LC 1563. 石子游戏 V — 博客 leetcode/dp/interval/ch61.md
#pragma once

#include <algorithm>
#include <climits>
#include <vector>

namespace lc1563 {

// 提交版：区间 DP O(n³)，前缀和加速区间求和
class Solution {
public:
    int stoneGameV(std::vector<int>& stoneValue) {
        const int n = stoneValue.size();
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));

        std::vector<int> pre(n + 1, 0);  // pre[i] = 前 i 个元素的和
        for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + stoneValue[i];

        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len <= n; i++) {
                int j = i + len - 1;
                for (int k = i; k < j; k++) {          // k 归左侧
                    int leftSum  = pre[k + 1] - pre[i];
                    int rightSum = pre[j + 1] - pre[k + 1];
                    int temp;
                    if (leftSum > rightSum) {
                        temp = rightSum + dp[k + 1][j];
                    } else if (leftSum < rightSum) {
                        temp = leftSum + dp[i][k];
                    } else {
                        temp = leftSum + std::max(dp[i][k], dp[k + 1][j]);
                    }
                    dp[i][j] = std::max(dp[i][j], temp);
                }
            }
        }
        return dp[0][n - 1];
    }
};

// O(n²) 优化版：利用分值全正带来的分界点单调性
//
// 对区间 [i, j]，切点 k 右移时 leftSum 递增、rightSum 递减，
// "leftSum ≤ total/2" 的切点构成前缀 [i, p]，其余为右保留集合 [q, j-1]
// （等和切点两边都算）。j 扩张时 p 单调不减；i 左扩时 q 单调不增——
// 两个指针各只走 O(n) 步，配合滚动最大值，每个区间 O(1) 均摊。
class SolutionFast {
public:
    int stoneGameV(std::vector<int>& stoneValue) {
        const int n = stoneValue.size();
        if (n <= 1) return 0;
        std::vector<long long> pre(n + 1, 0);
        for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + stoneValue[i];
        std::vector<std::vector<long long>> dp(n, std::vector<long long>(n, 0));

        const long long NEG = LLONG_MIN / 4;
        std::vector<int> p(n), q(n);
        std::vector<long long> best1(n, NEG), best2(n, NEG);
        for (int i = 0; i < n; i++) { p[i] = i - 1; q[i] = i; }

        for (int len = 2; len <= n; len++) {
            std::vector<long long> c1(n, NEG), c2(n, NEG);
            // 行扫描：i 升序，j = i+len-1 递增；p[i] 单调右移
            for (int i = 0; i + len <= n; i++) {
                int j = i + len - 1;
                long long total = pre[j + 1] - pre[i];
                while (p[i] + 1 <= j - 1 &&
                       2 * (pre[p[i] + 2] - pre[i]) <= total) {
                    p[i]++;
                    // 左保留候选：leftSum + dp[i][k] = (pre[k+1] + dp[i][k]) - pre[i]
                    best1[i] = std::max(best1[i], pre[p[i] + 1] + dp[i][p[i]]);
                }
                if (p[i] >= i) c1[i] = best1[i] - pre[i];
            }
            // 列扫描：i 降序；q[j] 单调左移
            for (int i = n - len; i >= 0; i--) {
                int j = i + len - 1;
                long long total = pre[j + 1] - pre[i];
                while (q[j] - 1 >= i &&
                       2 * (pre[q[j]] - pre[i]) >= total) {
                    q[j]--;
                    // 右保留候选：rightSum + dp[k+1][j] = pre[j+1] + (dp[k+1][j] - pre[k+1])
                    best2[j] = std::max(best2[j], dp[q[j] + 1][j] - pre[q[j] + 1]);
                }
                if (q[j] <= j - 1) c2[i] = pre[j + 1] + best2[j];
            }
            for (int i = 0; i + len <= n; i++)
                dp[i][i + len - 1] = std::max(c1[i], c2[i]);
        }
        return static_cast<int>(dp[0][n - 1]);
    }
};

// O(n²) 优化版（表驱动写法）：maxLeft/maxRight 滚动表替代逐 len 扫描
//
// maxLeft[i][j]  = max over k ∈ [i, j] of (dp[i][k] + sum[i..k])   —— 左保留候选
// maxRight[i][j] = max over k ∈ [i, j] of (dp[k][j] + sum[k..j])   —— 右保留候选
// 行内指针 i0 单调右移（分值全正），表项在 dp[i][j] 算出后原地滚动更新。
class SolutionFastTable {
public:
    int stoneGameV(std::vector<int>& stoneValue) {
        const int n = stoneValue.size();
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
        std::vector<std::vector<int>> maxLeft(n, std::vector<int>(n, 0));
        std::vector<std::vector<int>> maxRight(n, std::vector<int>(n, 0));

        std::vector<int> pre(n + 1, 0);
        for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + stoneValue[i];

        for (int i = n - 1; i >= 0; i--) {
            maxLeft[i][i] = maxRight[i][i] = stoneValue[i];
            int suml = 0;      // sum[i..i0]
            int i0 = i - 1;    // 最大的满足 suml*2 <= sum 的切点 k
            for (int j = i + 1; j < n; j++) {
                int sum = pre[j + 1] - pre[i];

                while (i0 + 1 < j && (suml + stoneValue[i0 + 1]) * 2 <= sum) {
                    suml += stoneValue[i0 + 1];
                    i0++;
                }

                int best = 0;
                if (i <= i0) best = std::max(best, maxLeft[i][i0]);          // 左保留
                if (i0 + 1 < j) best = std::max(best, maxRight[i0 + 2][j]);  // 右保留
                if (suml * 2 == sum) best = std::max(best, maxRight[i0 + 1][j]);  // 等和补一刀
                dp[i][j] = best;

                maxLeft[i][j] = std::max(maxLeft[i][j - 1], sum + dp[i][j]);
                maxRight[i][j] = std::max(maxRight[i + 1][j], sum + dp[i][j]);
            }
        }
        return dp[0][n - 1];
    }
};

// 写法变体：半开区间 [i, j)，i 降序 + j 升序替代 len 枚举
class SolutionHalfOpen {
public:
    int stoneGameV(std::vector<int>& stoneValue) {
        const int n = stoneValue.size();
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));

        std::vector<int> pre(n + 1, 0);
        for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + stoneValue[i];

        for (int i = n - 2; i >= 0; i--) {
            for (int j = i + 2; j <= n; j++) {   // [i, j)，长度 >= 2
                for (int k = i + 1; k < j; k++) { // 切点：左 [i,k)，右 [k,j)
                    int leftSum  = pre[k] - pre[i];
                    int rightSum = pre[j] - pre[k];
                    int temp;
                    if (leftSum > rightSum)      temp = rightSum + dp[k][j];
                    else if (leftSum < rightSum) temp = leftSum + dp[i][k];
                    else temp = leftSum + std::max(dp[i][k], dp[k][j]);
                    dp[i][j] = std::max(dp[i][j], temp);
                }
            }
        }
        return dp[0][n];
    }
};

// 暴力参照：无记忆化递归，直译定义（仅供小规模对拍）
class SolutionBrute {
public:
    int stoneGameV(std::vector<int>& stoneValue) {
        int n = stoneValue.size();
        pre.assign(n + 1, 0);
        for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + stoneValue[i];
        return solve(0, n - 1);
    }

private:
    std::vector<int> pre;
    int solve(int i, int j) {
        if (i >= j) return 0;
        int best = 0;
        for (int k = i; k < j; k++) {
            int leftSum  = pre[k + 1] - pre[i];
            int rightSum = pre[j + 1] - pre[k + 1];
            int temp;
            if (leftSum > rightSum)      temp = rightSum + solve(k + 1, j);
            else if (leftSum < rightSum) temp = leftSum + solve(i, k);
            else temp = leftSum + std::max(solve(i, k), solve(k + 1, j));
            best = std::max(best, temp);
        }
        return best;
    }
};

} // namespace lc1563
