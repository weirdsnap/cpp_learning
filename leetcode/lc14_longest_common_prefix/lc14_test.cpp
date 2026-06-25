#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 14 example 1") {
    std::vector<std::string> strs = {"flower", "flow", "flight"};
    CHECK(lc14::Solution().longestCommonPrefix(strs) == "fl");
}

TEST_CASE("LC 14 example 2") {
    std::vector<std::string> strs = {"dog", "racecar", "car"};
    CHECK(lc14::Solution().longestCommonPrefix(strs) == "");
}

TEST_CASE("LC 14 empty input") {
    std::vector<std::string> strs;
    CHECK(lc14::Solution().longestCommonPrefix(strs) == "");
}

TEST_CASE("LC 14 single string") {
    std::vector<std::string> strs = {"alone"};
    CHECK(lc14::Solution().longestCommonPrefix(strs) == "alone");
}
