#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 439 examples") {
    CHECK(lc439::Solution().parseTernary("T?2:3") == "2");
    CHECK(lc439::Solution().parseTernary("F?1:T?4:5") == "4");
    CHECK(lc439::Solution().parseTernary("T?T?F:5:3") == "F");
    CHECK(lc439::Solution().parseTernary("T") == "T");
    CHECK(lc439::Solution().parseTernary("9") == "9");
}

// 生成所有深度 <= d 的合法表达式：expr := atom | atom '?' expr ':' expr
static void genExpr(int d, std::vector<std::string>& out) {
    static const char atoms[] = {'T', 'F', '0', '1'};
    if (d == 0) {
        for (char a : atoms) out.emplace_back(1, a);
        return;
    }
    std::vector<std::string> sub;
    genExpr(d - 1, sub);
    out = sub;
    for (char a : atoms) {
        for (auto& x : sub) {
            for (auto& y : sub) {
                out.push_back(std::string(1, a) + "?" + x + ":" + y);
            }
        }
    }
}

TEST_CASE("LC 439 exhaustive: all valid expressions, depth <= 2") {
    std::vector<std::string> exprs;
    genExpr(2, exprs);  // d1=68；d2=68+4*68*68=18,564
    for (auto& e : exprs) {
        REQUIRE_MESSAGE(lc439::Solution().parseTernary(e) == lc439::oracle(e),
                        "expr=", e);
    }
}

TEST_CASE("LC 439 random depth-3 expressions") {
    std::mt19937 rng(439);
    std::vector<std::string> pool;
    genExpr(2, pool);
    static const char atoms[] = {'T', 'F', '0', '1'};
    for (int it = 0; it < 400; it++) {
        // 深度 3：atom ? depth2 : depth2
        std::string e = std::string(1, atoms[rng() % 4]) + "?" +
                        pool[rng() % pool.size()] + ":" + pool[rng() % pool.size()];
        CHECK(lc439::Solution().parseTernary(e) == lc439::oracle(e));
    }
}
