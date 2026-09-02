#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

using VS = std::vector<std::string>;

TEST_CASE("LC 422 examples") {
    VS a{"abcd", "bnrt", "crmy", "dtye"};
    CHECK(lc422::Solution().validWordSquare(a) == true);
    VS b{"abcd", "bnrt", "crm", "dt"};
    CHECK(lc422::Solution().validWordSquare(b) == true);
    VS c{"ball", "area", "read", "lady"};
    CHECK(lc422::Solution().validWordSquare(c) == false);
    VS d{"abc", "b", "c"};   // 经典边界：短行补空后仍合法
    CHECK(lc422::Solution().validWordSquare(d) == true);
    VS e{"abcd"};            // 行比 n 长 → false
    CHECK(lc422::Solution().validWordSquare(e) == false);
}

TEST_CASE("LC 422 exhaustive: words over {a,b}, lengths 1..3, n <= 3") {
    VS pool;
    for (int len = 1; len <= 3; len++) {
        int total = 1 << len;
        for (int mask = 0; mask < total; mask++) {
            std::string w(len, 'a');
            for (int p = 0; p < len; p++) w[p] = 'a' + ((mask >> p) & 1);
            pool.push_back(w);
        }
    }
    // n = 1, 2, 3 的全部组合: 14 + 196 + 2744
    for (auto& x : pool) {
        VS w1{x};
        REQUIRE(lc422::Solution().validWordSquare(w1) == lc422::bruteForce(w1));
        for (auto& y : pool) {
            VS w2{x, y};
            REQUIRE(lc422::Solution().validWordSquare(w2) == lc422::bruteForce(w2));
            for (auto& z : pool) {
                VS w3{x, y, z};
                REQUIRE(lc422::Solution().validWordSquare(w3) ==
                        lc422::bruteForce(w3));
            }
        }
    }
}

TEST_CASE("LC 422 random: words over {a..e}, lengths 1..8, n <= 6") {
    std::mt19937 rng(422);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 6;
        VS words(n);
        for (auto& w : words) {
            int len = 1 + rng() % 8;
            w.assign(len, 'a');
            for (auto& c : w) c = 'a' + rng() % 5;
        }
        CHECK(lc422::Solution().validWordSquare(words) == lc422::bruteForce(words));
    }
}
