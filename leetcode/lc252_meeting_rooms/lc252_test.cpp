#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

using VVI = std::vector<std::vector<int>>;

TEST_CASE("LC 252 examples") {
    VVI a{{0, 30}, {5, 10}, {15, 20}};
    CHECK(lc252::Solution().canAttendMeetings(a) == false);
    VVI b{{7, 10}, {2, 4}};
    CHECK(lc252::Solution().canAttendMeetings(b) == true);
    VVI c{{5, 10}, {10, 15}};   // 端点相接，不算重叠
    CHECK(lc252::Solution().canAttendMeetings(c) == true);
    VVI d{};
    CHECK(lc252::Solution().canAttendMeetings(d) == true);
    VVI e{{1, 5}};
    CHECK(lc252::Solution().canAttendMeetings(e) == true);
}

TEST_CASE("LC 252 exhaustive: endpoints in [0,5], interval count <= 3") {
    VVI pool;
    for (int a = 0; a <= 5; a++)
        for (int b = a; b <= 5; b++) pool.push_back({a, b});
    int P = pool.size();  // 21
    // 规模 0..3 的全部子集
    for (int i = 0; i < P; i++) {
        VVI w1{pool[i]};
        REQUIRE(lc252::Solution().canAttendMeetings(w1) == lc252::bruteForce(w1));
        for (int j = i; j < P; j++) {
            VVI w2{pool[i], pool[j]};
            REQUIRE(lc252::Solution().canAttendMeetings(w2) == lc252::bruteForce(w2));
            for (int k = j; k < P; k++) {
                VVI w3{pool[i], pool[j], pool[k]};
                REQUIRE(lc252::Solution().canAttendMeetings(w3) ==
                        lc252::bruteForce(w3));
            }
        }
    }
    VVI empty{};
    REQUIRE(lc252::Solution().canAttendMeetings(empty) == lc252::bruteForce(empty));
}

TEST_CASE("LC 252 random intervals up to 50") {
    std::mt19937 rng(252);
    for (int it = 0; it < 500; it++) {
        int n = rng() % 51;
        VVI iv(n);
        for (auto& v : iv) {
            int a = rng() % 1000;
            int b = a + rng() % 100;
            v = {a, b};
        }
        CHECK(lc252::Solution().canAttendMeetings(iv) == lc252::bruteForce(iv));
    }
}
