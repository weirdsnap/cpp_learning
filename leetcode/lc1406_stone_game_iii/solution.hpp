// LC 1406. 石子游戏 III — 博客 leetcode/dp/interval/ch59.md
#pragma once

#include <algorithm>
#include <climits>
#include <string>
#include <vector>

namespace lc1406 {

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
#define LC1406_HAS_MEMO 1

// 提交版：记忆化搜索（deducing this 递归 lambda），净胜分定义
class Solution {
public:
    std::string stoneGameIII(std::vector<int>& stoneValue) {
        const int n = stoneValue.size();
        std::vector<int> dp(n + 1, -1);  // -1 = 未计算（注意：-1 本身是合法净胜分，见文章）

        auto func = [&](this auto&& self, int i) -> int {
            if (i >= n) return 0;
            if (dp[i] == -1) {
                int best = INT_MIN;
                int sum = 0;
                for (int k = 1; k < 4 && i + k <= n; k++) {
                    sum += stoneValue[i + k - 1];
                    best = std::max(best, sum - self(i + k));
                }
                dp[i] = best;
            }
            return dp[i];
        };

        int total = func(0);
        return total > 0 ? "Alice" : (total == 0 ? "Tie" : "Bob");
    }
};

#endif // __cpp_explicit_this_parameter

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
#define LC1406_HAS_MEMO_SUFFIX 1

// 变体：哨兵修正为 INT_MIN/2，后缀和数组 O(1) 计算拿走的分数
class SolutionMemoSuffix {
public:
    std::string stoneGameIII(std::vector<int>& stoneValue) {
        const int n = stoneValue.size();
        std::vector<int> dp(n + 1, INT_MIN / 2);

        std::vector<int> suffix(n + 1, 0);
        for (int i = n - 1; i >= 0; --i) suffix[i] = suffix[i + 1] + stoneValue[i];

        auto func = [&](this auto&& self, int i) -> int {
            if (i >= n) return 0;
            if (dp[i] == INT_MIN / 2) {
                for (int k = 1; k <= 3 && i + k <= n; ++k) {
                    int gain = suffix[i] - suffix[i + k];
                    dp[i] = std::max(dp[i], gain - self(i + k));
                }
            }
            return dp[i];
        };

        int total = func(0);
        return total > 0 ? "Alice" : (total == 0 ? "Tie" : "Bob");
    }
};

#endif // __cpp_explicit_this_parameter (suffix variant)

// 递推变体：dp 多开 3 格消除越界判断，running 增量后缀和
class SolutionDpPadded {
public:
    std::string stoneGameIII(std::vector<int>& stoneValue) {
        const int n = stoneValue.size();
        std::vector<int> dp(n + 3, 0);  // dp[n..n+2] = 0，读越界位也安全
        for (int i = n - 1; i >= 0; --i) {
            int running = 0;
            int best = INT_MIN;
            for (int k = 1; k <= 3; ++k) {
                if (i + k - 1 < n) running += stoneValue[i + k - 1];
                best = std::max(best, running - dp[i + k]);
            }
            dp[i] = best;
        }
        int total = dp[0];
        return total > 0 ? "Alice" : (total == 0 ? "Tie" : "Bob");
    }
};

// 递推版：三步走的最后一步，自底向上消除递归
class SolutionDp {
public:
    std::string stoneGameIII(std::vector<int>& stoneValue) {
        const int n = stoneValue.size();
        std::vector<int> dp(n + 1, 0);  // dp[n] = 0 天然边界
        for (int i = n - 1; i >= 0; i--) {
            int best = INT_MIN, sum = 0;
            for (int k = 1; k < 4 && i + k <= n; k++) {
                sum += stoneValue[i + k - 1];
                best = std::max(best, sum - dp[i + k]);
            }
            dp[i] = best;
        }
        return dp[0] > 0 ? "Alice" : (dp[0] == 0 ? "Tie" : "Bob");
    }
};

// 暴力参照：无记忆化递归
class SolutionBrute {
public:
    std::string stoneGameIII(std::vector<int>& stoneValue) {
        int net = solve(stoneValue, 0);
        return net > 0 ? "Alice" : (net == 0 ? "Tie" : "Bob");
    }

private:
    int solve(std::vector<int>& v, int i) {
        if (i >= (int)v.size()) return 0;
        int best = INT_MIN, sum = 0;
        for (int k = 1; k < 4 && i + k <= (int)v.size(); k++) {
            sum += v[i + k - 1];
            best = std::max(best, sum - solve(v, i + k));
        }
        return best;
    }
};

} // namespace lc1406
