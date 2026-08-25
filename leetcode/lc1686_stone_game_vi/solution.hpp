// LC 1686. 石子游戏 VI — 博客 leetcode/greedy/ch62.md
#pragma once

#include <algorithm>
#include <climits>
#include <vector>

namespace lc1686 {

// 提交版：按 a[i]+b[i] 降序，轮流取
class Solution {
public:
    int stoneGameVI(std::vector<int>& aliceValues, std::vector<int>& bobValues) {
        int n = aliceValues.size();
        std::vector<int> idx(n);
        for (int i = 0; i < n; i++) idx[i] = i;
        std::sort(idx.begin(), idx.end(), [&](int x, int y) {
            return aliceValues[x] + bobValues[x] > aliceValues[y] + bobValues[y];
        });
        int a = 0, b = 0;
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) a += aliceValues[idx[i]];
            else            b += bobValues[idx[i]];
        }
        return a > b ? 1 : (a == b ? 0 : -1);
    }
};

// 暴力参照：Bitmask 博弈搜索，net = Alice 总分 - Bob 总分
// Alice 回合取 max，Bob 回合取 min，返回符号
class SolutionBrute {
public:
    int stoneGameVI(std::vector<int>& aliceValues, std::vector<int>& bobValues) {
        a = &aliceValues; b = &bobValues;
        n = aliceValues.size();
        memo.assign(1 << n, std::vector<long long>(2, UNSET));
        long long net = solve((1 << n) - 1, 0);
        return net > 0 ? 1 : (net == 0 ? 0 : -1);
    }

private:
    static constexpr long long UNSET = LLONG_MIN;
    std::vector<int>* a;
    std::vector<int>* b;
    int n;
    std::vector<std::vector<long long>> memo;  // memo[mask][turn]

    long long solve(int mask, int turn) {
        if (mask == 0) return 0;
        long long& res = memo[mask][turn];
        if (res != UNSET) return res;
        if (turn == 0) {  // Alice：最大化 net
            res = LLONG_MIN;
            for (int i = 0; i < n; i++)
                if (mask & (1 << i))
                    res = std::max(res, (*a)[i] + solve(mask ^ (1 << i), 1));
        } else {          // Bob：最小化 net（他加 b_i 等价于 net 减 b_i）
            res = LLONG_MAX;
            for (int i = 0; i < n; i++)
                if (mask & (1 << i))
                    res = std::min(res, -(*b)[i] + solve(mask ^ (1 << i), 0));
        }
        return res;
    }
};

} // namespace lc1686
