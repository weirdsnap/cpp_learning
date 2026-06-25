#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 221 example 1: 3x3 max square area 4") {
    std::vector<std::vector<char>> matrix = {
        {'1', '0', '1', '0', '0'},
        {'1', '0', '1', '1', '1'},
        {'1', '1', '1', '1', '1'},
        {'1', '0', '0', '1', '0'}
    };
    Solution s;
    CHECK(s.maximalSquare(matrix) == 4);
}

TEST_CASE("LC 221 example 2: 2x2 all 1s area 4") {
    std::vector<std::vector<char>> matrix = {
        {'0', '1'},
        {'1', '0'}
    };
    Solution s;
    CHECK(s.maximalSquare(matrix) == 1);
}

TEST_CASE("LC 221 example 3: 1x1 zero area 0") {
    std::vector<std::vector<char>> matrix = {{'0'}};
    Solution s;
    CHECK(s.maximalSquare(matrix) == 0);
}

TEST_CASE("LC 221 all 1s 2x2 area 4") {
    std::vector<std::vector<char>> matrix = {
        {'1', '1'},
        {'1', '1'}
    };
    Solution s;
    CHECK(s.maximalSquare(matrix) == 4);
}

TEST_CASE("LC 221 empty matrix area 0") {
    std::vector<std::vector<char>> matrix;
    Solution s;
    CHECK(s.maximalSquare(matrix) == 0);
}
