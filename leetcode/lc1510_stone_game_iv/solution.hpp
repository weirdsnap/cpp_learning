// LC 1510. 石子游戏 IV — 博客 leetcode/dp/interval/ch60.md
#pragma once

#include <vector>

namespace lc1510 {

// 提交版：一维布尔 DP，f[i] = 剩余 i 块石头时当前玩家是否必胜
class Solution {
public:
    bool winnerSquareGame(int n) {
        std::vector<int> f(n + 1);   // f[0] = 0：无石头可取，当前玩家必败
        for (int i = 1; i <= n; ++i) {
            for (int k = 1; k * k <= i; ++k) {
                if (!f[i - k * k]) {  // 存在一步让对手落入必败态
                    f[i] = true;
                    break;            // 找到一步即可，提前退出
                }
            }
        }
        return f[n];
    }
};

// 参照版：记忆化递归，直译定义
class SolutionMemo {
public:
    bool winnerSquareGame(int n) {
        memo.assign(n + 1, -1);  // -1 未计算, 0 必败, 1 必胜
        return solve(n);
    }

private:
    std::vector<int> memo;
    int solve(int i) {
        if (i == 0) return 0;
        if (memo[i] != -1) return memo[i];
        for (int k = 1; k * k <= i; ++k) {
            if (!solve(i - k * k)) return memo[i] = 1;
        }
        return memo[i] = 0;
    }
};

// 暴力参照：无记忆化递归（仅供小规模对拍）
class SolutionBrute {
public:
    bool winnerSquareGame(int n) { return solve(n); }

private:
    bool solve(int i) {
        if (i == 0) return false;
        for (int k = 1; k * k <= i; ++k)
            if (!solve(i - k * k)) return true;
        return false;
    }
};

} // namespace lc1510
