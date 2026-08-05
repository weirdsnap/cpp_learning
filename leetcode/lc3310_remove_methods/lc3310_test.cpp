#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <functional>
#include <random>

#include "solution.hpp"

// 参照实现：递归 DFS 求可达集合 + 集合判定（独立写法）
static std::vector<int> refImpl(int n, int k,
                                std::vector<std::vector<int>>& invocations) {
    std::vector<std::vector<int>> g(n);
    for (auto& e : invocations) g[e[0]].push_back(e[1]);
    std::vector<bool> sus(n, false);
    std::function<void(int)> dfs = [&](int u) {
        sus[u] = true;
        for (int v : g[u])
            if (!sus[v]) dfs(v);
    };
    dfs(k);
    for (auto& e : invocations) {
        if (sus[e[1]] && !sus[e[0]]) {
            std::vector<int> all(n);
            std::iota(all.begin(), all.end(), 0);
            return all;
        }
    }
    std::vector<int> res;
    for (int i = 0; i < n; i++)
        if (!sus[i]) res.push_back(i);
    return res;
}

TEST_CASE("LC 3310 examples") {
    std::vector<std::vector<int>> e1{{1, 2}, {0, 1}, {3, 2}};
    std::vector<std::vector<int>> e2{{1, 2}, {0, 2}, {0, 1}, {3, 4}};
    std::vector<std::vector<int>> e3{{1, 2}, {0, 1}, {2, 0}};
    std::vector<std::vector<int>> c1 = e1, c2 = e2, c3 = e3;
    CHECK(lc3310::Solution().remainingMethods(4, 1, e1) == std::vector<int>{0, 1, 2, 3});
    CHECK(lc3310::Solution().remainingMethods(5, 0, e2) == std::vector<int>{3, 4});
    CHECK(lc3310::Solution().remainingMethods(3, 2, e3).empty());
    CHECK(lc3310::SolutionV1().remainingMethods(4, 1, c1) == std::vector<int>{0, 1, 2, 3});
    CHECK(lc3310::SolutionV1().remainingMethods(5, 0, c2) == std::vector<int>{3, 4});
    CHECK(lc3310::SolutionV1().remainingMethods(3, 2, c3).empty());
}

TEST_CASE("LC 3310 edge cases") {
    // 没有调用边：只有 k 可疑，删掉 k
    std::vector<std::vector<int>> none;
    std::vector<std::vector<int>> none2;
    CHECK(lc3310::Solution().remainingMethods(3, 0, none) == std::vector<int>{1, 2});
    CHECK(lc3310::SolutionV1().remainingMethods(3, 0, none2) == std::vector<int>{1, 2});
}

TEST_CASE("LC 3310 random cross-check vs DFS reference") {
    std::mt19937 rng(3310);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 30;
        int k = rng() % n;
        int maxEdges = n * (n - 1);
        int m = maxEdges == 0 ? 0 : rng() % (std::min(2 * n, maxEdges) + 1);
        // 生成不重复、无自环的边
        std::vector<std::vector<int>> edges;
        std::vector<std::vector<bool>> used(n, std::vector<bool>(n, false));
        while ((int)edges.size() < m) {
            int a = rng() % n, b = rng() % n;
            if (a != b && !used[a][b]) {
                used[a][b] = true;
                edges.push_back({a, b});
            }
        }
        auto expected = refImpl(n, k, edges);
        CHECK(lc3310::Solution().remainingMethods(n, k, edges) == expected);
        CHECK(lc3310::SolutionV1().remainingMethods(n, k, edges) == expected);
    }
}
