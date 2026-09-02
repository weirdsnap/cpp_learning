#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include <random>

#include "solution.hpp"

using VS = std::vector<std::string>;
using VVS = std::vector<std::vector<std::string>>;

TEST_CASE("LC 734 examples") {
    VS s1{"great", "acting", "skills"}, s2{"fine", "drama", "talent"};
    VVS p1{{"great", "fine"}, {"drama", "acting"}, {"skills", "talent"}};
    CHECK(lc734::Solution().areSentencesSimilar(s1, s2, p1) == true);
    CHECK(lc734::SolutionOrdered().areSentencesSimilar(s1, s2, p1) == true);

    VS a{"great"}, b{"great"};
    VVS p2{};
    CHECK(lc734::Solution().areSentencesSimilar(a, b, p2) == true);
    CHECK(lc734::SolutionOrdered().areSentencesSimilar(a, b, p2) == true);

    VS c{"great"}, d{"doubleplusgood"};
    VVS p3{{"great", "doubleplusgood"}};
    // 单词不同但相似对直接命中 → true
    CHECK(lc734::Solution().areSentencesSimilar(c, d, p3) == true);

    VS e{"great"}, f{"good"};
    VVS p4{};
    CHECK(lc734::Solution().areSentencesSimilar(e, f, p4) == false);

    VS g{"great", "fine"}, h{"great"};
    VVS p5{{"great", "fine"}};
    CHECK(lc734::Solution().areSentencesSimilar(g, h, p5) == false);  // 长度不同
}

TEST_CASE("LC 734 exhaustive: words {a,b,c}, len <= 2, all pair subsets") {
    // 无序对共 3 种：a-b, a-c, b-c；子集 2^3 = 8
    VS words{"a", "b", "c"};
    VS sentences;
    for (auto& w : words) sentences.push_back(w);
    for (auto& u : words)
        for (auto& v : words) sentences.push_back(u + v);
    for (int mask = 0; mask < 8; mask++) {
        VVS pairs;
        if (mask & 1) pairs.push_back({"a", "b"});
        if (mask & 2) pairs.push_back({"a", "c"});
        if (mask & 4) pairs.push_back({"b", "c"});
        for (auto& x : sentences) {
            for (auto& y : sentences) {
                VS s1{x}, s2{y};
                auto p = pairs;
                bool want = lc734::bruteForce(s1, s2, p);
                REQUIRE(lc734::Solution().areSentencesSimilar(s1, s2, p) == want);
                REQUIRE(lc734::SolutionOrdered().areSentencesSimilar(s1, s2, p) == want);
            }
        }
    }
}

TEST_CASE("LC 734 random: duplicate/self pairs, sentences up to 30 words") {
    std::mt19937 rng(734);
    for (int it = 0; it < 400; it++) {
        int vocab = 1 + rng() % 8;
        auto randWord = [&]() {
            int len = 1 + rng() % 4;
            std::string w(len, 'a');
            for (auto& c : w) c = 'a' + rng() % vocab;
            return w;
        };
        int len1 = 1 + rng() % 30;
        int len2 = rng() % 4 == 0 ? 1 + rng() % 30 : len1;  // 1/4 概率长度不同
        VS s1(len1), s2(len2);
        for (auto& w : s1) w = randWord();
        for (auto& w : s2) w = randWord();
        VVS pairs;
        int np = rng() % 15;
        for (int t = 0; t < np; t++) pairs.push_back({randWord(), randWord()});  // 允许自对、重复对
        bool want = lc734::bruteForce(s1, s2, pairs);
        CHECK(lc734::Solution().areSentencesSimilar(s1, s2, pairs) == want);
        CHECK(lc734::SolutionOrdered().areSentencesSimilar(s1, s2, pairs) == want);
    }
}
