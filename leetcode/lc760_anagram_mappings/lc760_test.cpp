#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <random>

#include "solution.hpp"

TEST_CASE("LC 760 examples") {
    std::vector<int> a1{12, 28, 46, 32, 50}, a2{50, 12, 32, 46, 28};
    CHECK(lc760::validMapping(a1, a2, lc760::Solution().anagramMappings(a1, a2)));
    CHECK(lc760::validMapping(a1, a2, lc760::SolutionV2().anagramMappings(a1, a2)));
    std::vector<int> b1{84, 46}, b2{84, 46};
    CHECK(lc760::validMapping(b1, b2, lc760::Solution().anagramMappings(b1, b2)));
    CHECK(lc760::validMapping(b1, b2, lc760::SolutionV2().anagramMappings(b1, b2)));
}

TEST_CASE("LC 760 exhaustive nums1 over {0,1,2}, len <= 8, x permutations") {
    std::mt19937 rng(760);
    for (int len = 1; len <= 8; len++) {
        int total = 1;
        for (int t = 0; t < len; t++) total *= 3;
        for (int mask = 0; mask < total; mask++) {
            std::vector<int> nums1(len);
            int m = mask;
            for (int p = 0; p < len; p++) { nums1[p] = m % 3; m /= 3; }
            // 每个 nums1 测 4 种 nums2 排列：原序、逆序、两次确定性 shuffle
            std::vector<std::vector<int>> perms;
            perms.push_back(nums1);
            perms.push_back(std::vector<int>(nums1.rbegin(), nums1.rend()));
            for (int rep = 0; rep < 2; rep++) {
                auto p = nums1;
                std::shuffle(p.begin(), p.end(), rng);
                perms.push_back(p);
            }
            for (auto& nums2 : perms) {
                auto m1 = lc760::Solution().anagramMappings(nums1, nums2);
                auto m2 = lc760::SolutionV2().anagramMappings(nums1, nums2);
                REQUIRE_MESSAGE(lc760::validMapping(nums1, nums2, m1),
                                "v1 invalid, len=", len, " mask=", mask);
                REQUIRE_MESSAGE(lc760::validMapping(nums1, nums2, m2),
                                "v2 invalid, len=", len, " mask=", mask);
            }
        }
    }
}

TEST_CASE("LC 760 random with heavy duplicates") {
    std::mt19937 rng(76);
    for (int it = 0; it < 300; it++) {
        int len = 1 + rng() % 100;
        std::vector<int> nums1(len);
        for (auto& x : nums1) x = rng() % 4;   // 大量重复值
        auto nums2 = nums1;
        std::shuffle(nums2.begin(), nums2.end(), rng);
        CHECK(lc760::validMapping(nums1, nums2,
                                  lc760::Solution().anagramMappings(nums1, nums2)));
        CHECK(lc760::validMapping(nums1, nums2,
                                  lc760::SolutionV2().anagramMappings(nums1, nums2)));
    }
}
