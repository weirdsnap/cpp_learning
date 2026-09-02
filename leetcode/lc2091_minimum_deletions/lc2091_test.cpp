#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <numeric>
#include <random>

#include "solution.hpp"

TEST_CASE("LC 2091 examples") {
    std::vector<int> a{2, 10, 7, 5, 4, 1, 8, 6};
    CHECK(lc2091::Solution().minimumDeletions(a) == 5);
    std::vector<int> b{0, -4, 19, 1, 8, -2, -3, 5};
    CHECK(lc2091::Solution().minimumDeletions(b) == 3);
    std::vector<int> c{101};
    CHECK(lc2091::Solution().minimumDeletions(c) == 1);
}

TEST_CASE("LC 2091 exhaustive permutations of [1..n], n <= 7") {
    for (int n = 1; n <= 7; n++) {
        std::vector<int> nums(n);
        std::iota(nums.begin(), nums.end(), 1);
        do {
            int got = lc2091::Solution().minimumDeletions(nums);
            int want = lc2091::bfsOracle(nums);
            REQUIRE_MESSAGE(got == want, "n=", n, " got=", got, " want=", want);
        } while (std::next_permutation(nums.begin(), nums.end()));
    }
}

TEST_CASE("LC 2091 random permutations, n up to 60") {
    std::mt19937 rng(2091);
    for (int it = 0; it < 500; it++) {
        int n = 1 + rng() % 60;
        std::vector<int> nums(n);
        std::iota(nums.begin(), nums.end(), 1);
        std::shuffle(nums.begin(), nums.end(), rng);
        CHECK(lc2091::Solution().minimumDeletions(nums) == lc2091::bfsOracle(nums));
    }
}
