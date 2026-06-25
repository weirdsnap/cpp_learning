#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 135 example 1") {
    std::vector<int> ratings = {1, 0, 2};
    CHECK(lc135::SolutionTwoPass().candy(ratings) == 5);
    CHECK(lc135::SolutionOnePass().candy(ratings) == 5);
}

TEST_CASE("LC 135 example 2") {
    std::vector<int> ratings = {1, 2, 2};
    CHECK(lc135::SolutionTwoPass().candy(ratings) == 4);
    CHECK(lc135::SolutionOnePass().candy(ratings) == 4);
}

TEST_CASE("LC 135 all equal") {
    std::vector<int> ratings = {2, 2, 2, 2};
    CHECK(lc135::SolutionTwoPass().candy(ratings) == 4);
}

TEST_CASE("LC 135 peak") {
    std::vector<int> ratings = {1, 5, 4, 3, 2, 1};
    CHECK(lc135::SolutionOnePass().candy(ratings) == 16);
}
