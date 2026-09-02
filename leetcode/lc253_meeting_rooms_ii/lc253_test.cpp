#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

using VVI = std::vector<std::vector<int>>;

TEST_CASE("LC 253 examples") {
    VVI a{{0, 30}, {5, 10}, {15, 20}};
    CHECK(lc253::Solution().minMeetingRooms(a) == 2);
    VVI b{{7, 10}, {2, 4}};
    CHECK(lc253::Solution().minMeetingRooms(b) == 1);
    VVI c{{5, 10}, {10, 15}, {15, 20}};  // 端点相接不叠加
    CHECK(lc253::Solution().minMeetingRooms(c) == 1);
    VVI d{};
    CHECK(lc253::Solution().minMeetingRooms(d) == 0);
    VVI e{{0, 10}, {0, 10}, {0, 10}};    // 完全重合
    CHECK(lc253::Solution().minMeetingRooms(e) == 3);
}

TEST_CASE("LC 253 exhaustive: endpoints in [0,5], count <= 3") {
    VVI pool;
    for (int a = 0; a <= 5; a++)
        for (int b = a + 1; b <= 5; b++) pool.push_back({a, b});  // a < b
    int P = pool.size();  // 15
    VVI empty{};
    REQUIRE(lc253::Solution().minMeetingRooms(empty) == lc253::bruteForce(empty));
    for (int i = 0; i < P; i++) {
        VVI w1{pool[i]};
        REQUIRE(lc253::Solution().minMeetingRooms(w1) == lc253::bruteForce(w1));
        for (int j = i; j < P; j++) {
            VVI w2{pool[i], pool[j]};
            REQUIRE(lc253::Solution().minMeetingRooms(w2) == lc253::bruteForce(w2));
            for (int k = j; k < P; k++) {
                VVI w3{pool[i], pool[j], pool[k]};
                REQUIRE(lc253::Solution().minMeetingRooms(w3) ==
                        lc253::bruteForce(w3));
            }
        }
    }
}

TEST_CASE("LC 253 random intervals up to 60") {
    std::mt19937 rng(253);
    for (int it = 0; it < 500; it++) {
        int n = rng() % 61;
        VVI iv(n);
        for (auto& v : iv) {
            int a = rng() % 500;
            v = {a, a + 1 + (int)(rng() % 50)};
        }
        CHECK(lc253::Solution().minMeetingRooms(iv) == lc253::bruteForce(iv));
    }
}
