#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 42 example: [0,1,0,2,1,0,1,3,2,1,2,1] -> 6") {
    std::vector<int> height = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    CHECK(lc42::SolutionDP().trap(height) == 6);
    CHECK(lc42::SolutionTwoPointers().trap(height) == 6);
    CHECK(lc42::SolutionMonotonicStack().trap(height) == 6);
}

TEST_CASE("LC 42 empty / flat") {
    std::vector<int> empty;
    CHECK(lc42::SolutionDP().trap(empty) == 0);
    CHECK(lc42::SolutionTwoPointers().trap(empty) == 0);
    CHECK(lc42::SolutionMonotonicStack().trap(empty) == 0);

    std::vector<int> flat(5, 1);
    CHECK(lc42::SolutionDP().trap(flat) == 0);
}

TEST_CASE("LC 42 monotonic hill") {
    std::vector<int> height = {0, 1, 2, 3, 2, 1, 0};
    CHECK(lc42::SolutionTwoPointers().trap(height) == 0);
}
