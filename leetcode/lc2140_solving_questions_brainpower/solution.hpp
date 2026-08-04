// LC 2140. 解决智力问题 — 博客 leetcode/dp/ch34.md
#pragma once

#include <algorithm>
#include <vector>

namespace lc2140 {

// 解法二：自底向上递推（从后往前，无递归栈溢出风险）
class SolutionIterative {
public:
    long long mostPoints(std::vector<std::vector<int>>& questions) {
        const int n = questions.size();
        std::vector<long long> dp(n + 1, 0);
        for (int i = n - 1; i >= 0; --i) {
            long long skip = dp[i + 1];
            long long solve = questions[i][0];
            int next = i + questions[i][1] + 1;
            if (next < n) solve += dp[next];
            dp[i] = std::max(skip, solve);
        }
        return dp[0];
    }
};

// 解法一：记忆化搜索，使用 C++23 deducing this 的 lambda 递归
// CI 的 GCC 13 不支持显式对象参数，用特性宏保护（GCC 14 定义的是 202110L）
#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
#define LC2140_HAS_MEMO 1
class SolutionMemo {
public:
    long long mostPoints(std::vector<std::vector<int>>& questions) {
        const int n = questions.size();
        std::vector<long long> dp(n + 1, -1);
        dp[n] = questions[n - 1][0];
        auto helper = [&](this auto&& self, int i) -> long long {
            if (i > n) return 0LL;
            if (dp[i] == -1) {
                dp[i] = std::max(
                    self(i + 1),
                    self(i + questions[i - 1][1] + 1) + questions[i - 1][0]);
            }
            return dp[i];
        };
        return helper(1);
    }
};
#else
#define LC2140_HAS_MEMO 0
#endif

} // namespace lc2140
