#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 3612 plain letters") {
    CHECK(lc3612::SolutionSimulation().processStr("abc") == "abc");
}

TEST_CASE("LC 3612 star delete") {
    CHECK(lc3612::SolutionSimulation().processStr("ab*c") == "ac");
}

TEST_CASE("LC 3612 hash duplicate") {
    CHECK(lc3612::SolutionSimulation().processStr("ab#c") == "ababc");
}

TEST_CASE("LC 3612 percent reverse") {
    CHECK(lc3612::SolutionSimulation().processStr("ab%") == "ba");
}

TEST_CASE("LC 3612 combined") {
    CHECK(lc3612::SolutionDeque().processStr("ab#c*%") == "baba");
}

TEST_CASE("LC 3612 star on empty") {
    CHECK(lc3612::SolutionDeque().processStr("*") == "");
}
