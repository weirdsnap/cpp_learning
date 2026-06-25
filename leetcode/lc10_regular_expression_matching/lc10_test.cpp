#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 10 example cases") {
    lc10::Solution sol;
    CHECK_FALSE(sol.isMatch("aa", "a"));
    CHECK(sol.isMatch("aa", "a*"));
    CHECK(sol.isMatch("ab", ".*"));
    CHECK(sol.isMatch("aab", "c*a*b"));
    CHECK_FALSE(sol.isMatch("mississippi", "mis*is*p*."));
}

TEST_CASE("LC 10 empty pattern") {
    lc10::Solution sol;
    CHECK(sol.isMatch("", ""));
    CHECK_FALSE(sol.isMatch("a", ""));
}

TEST_CASE("LC 10 zero occurrences with *") {
    lc10::Solution sol;
    CHECK(sol.isMatch("", "a*b*c*"));
}
