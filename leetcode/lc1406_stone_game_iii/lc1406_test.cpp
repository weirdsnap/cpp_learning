#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 1406 examples") {
    std::vector<int> a{1, 2, 3, 7};
    std::vector<int> b{1, 2, 3, -9};
    std::vector<int> c{1, 2, 3, 6};
#ifdef LC1406_HAS_MEMO
    std::vector<int> a2 = a, b2 = b, c2 = c;
    CHECK(lc1406::Solution().stoneGameIII(a2) == "Bob");
    CHECK(lc1406::Solution().stoneGameIII(b2) == "Alice");
    CHECK(lc1406::Solution().stoneGameIII(c2) == "Tie");
#endif
    CHECK(lc1406::SolutionDp().stoneGameIII(a) == "Bob");
    CHECK(lc1406::SolutionDp().stoneGameIII(b) == "Alice");
    CHECK(lc1406::SolutionDp().stoneGameIII(c) == "Tie");
}

TEST_CASE("LC 1406 memo vs DP vs brute") {
    std::mt19937 rng(1406);
    for (int it = 0; it < 300; it++) {
        int n = 1 + rng() % 10;
        std::vector<int> v(n);
        for (auto& x : v) x = static_cast<int>(rng() % 21) - 10;  // 含负数
        auto c1 = v, c2 = v, c3 = v, c4 = v;
        std::string expected = lc1406::SolutionBrute().stoneGameIII(v);
        CAPTURE(n);
#ifdef LC1406_HAS_MEMO
        CHECK(lc1406::Solution().stoneGameIII(c1) == expected);
#endif
#ifdef LC1406_HAS_MEMO_SUFFIX
        CHECK(lc1406::SolutionMemoSuffix().stoneGameIII(c3) == expected);
#endif
        CHECK(lc1406::SolutionDp().stoneGameIII(c2) == expected);
        CHECK(lc1406::SolutionDpPadded().stoneGameIII(c4) == expected);
    }
}

#ifdef LC1406_HAS_MEMO
TEST_CASE("LC 1406 net score can be -1 (sentinel collision case)") {
    // 净胜分 -1 是合法值：构造净胜分恰为 -1 的输入，验证提交版答案仍正确
    // （-1 哨兵只会导致重算，不影响结果——但说明哨兵选得不干净）
    std::vector<int> v{1, 2};  // Alice 拿 1 → 净胜 1-2=-1；拿 1,2 → 3。-1 vs 3，Alice 选 3
    CHECK(lc1406::Solution().stoneGameIII(v) == "Alice");
    std::vector<int> w{1};     // 净胜 1
    CHECK(lc1406::Solution().stoneGameIII(w) == "Alice");
    std::vector<int> z{0, 0};  // 净胜 0
    CHECK(lc1406::Solution().stoneGameIII(z) == "Tie");
    // 负净胜分状态：[-5, 1] Alice 拿 -5 → -5-1=-6；拿 -5,1 → -4。最大 -4
    std::vector<int> u{-5, 1};
    CHECK(lc1406::Solution().stoneGameIII(u) == "Bob");
    std::vector<int> u2 = u;
    CHECK(lc1406::SolutionDp().stoneGameIII(u2) == "Bob");
}
#endif
