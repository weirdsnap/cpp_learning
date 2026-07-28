#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <queue>
#include <random>
#include <unordered_set>

#include "solution.hpp"

// 无剪枝暴力 BFS（任意两位置交换），保证真最短，作为对拍基准
static int brute(std::string s1, std::string s2) {
    if (s1 == s2) return 0;
    std::queue<std::pair<std::string, int>> q;
    std::unordered_set<std::string> vis;
    q.push({s1, 0});
    vis.insert(s1);
    while (!q.empty()) {
        auto [cur, step] = q.front();
        q.pop();
        int n = cur.size();
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++) {
                std::swap(cur[i], cur[j]);
                if (cur == s2) return step + 1;
                if (!vis.count(cur)) {
                    vis.insert(cur);
                    q.push({cur, step + 1});
                }
                std::swap(cur[i], cur[j]);
            }
    }
    return -1;
}

TEST_CASE("LC 854 examples") {
    CHECK(lc854::SolutionBFS().kSimilarity("ab", "ba") == 1);
    CHECK(lc854::SolutionBFS().kSimilarity("abc", "bca") == 2);
    CHECK(lc854::SolutionAStar().kSimilarity("abac", "baca") == 2);
    CHECK(lc854::SolutionIDAStar().kSimilarity("abc", "bca") == 2);
    CHECK(lc854::SolutionIDAStar().kSimilarity("abc", "abc") == 0);
}

TEST_CASE("LC 854 random cross-check: BFS / A* / IDA* agree with unpruned brute BFS") {
    std::mt19937 rng(31);
    for (int it = 0; it < 300; it++) {
        int n = 2 + rng() % 7;
        std::string a(n, 'a');
        for (auto& c : a) c = char('a' + rng() % 4);
        std::string b = a;
        std::shuffle(b.begin(), b.end(), rng);

        int expect = brute(a, b);
        CHECK(lc854::SolutionBFS().kSimilarity(a, b) == expect);
        CHECK(lc854::SolutionAStar().kSimilarity(a, b) == expect);
        CHECK(lc854::SolutionIDAStar().kSimilarity(a, b) == expect);
    }
}
