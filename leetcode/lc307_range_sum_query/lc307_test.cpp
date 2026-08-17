#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 307 example") {
    std::vector<int> v{1, 3, 5};
    lc307::NumArray na(v);
    CHECK(na.sumRange(0, 2) == 9);
    na.update(1, 2);
    CHECK(na.sumRange(0, 2) == 8);
}

TEST_CASE("LC 307 single element") {
    std::vector<int> v{7};
    lc307::NumArray na(v);
    CHECK(na.sumRange(0, 0) == 7);
    na.update(0, -3);
    CHECK(na.sumRange(0, 0) == -3);
}

TEST_CASE("LC 307 segment tree vs naive randomized") {
    std::mt19937 rng(307);
    for (int round = 0; round < 100; round++) {
        int n = 1 + rng() % 64;
        std::vector<int> v(n);
        for (auto& x : v) x = static_cast<int>(rng() % 2001) - 1000;
        lc307::NumArray st(v);
        lc307::NumArrayNaive nv(v);
        for (int op = 0; op < 200; op++) {
            if (rng() % 2) {
                int idx = rng() % n, val = static_cast<int>(rng() % 2001) - 1000;
                st.update(idx, val);
                nv.update(idx, val);
            } else {
                int l = rng() % n, r = l + rng() % (n - l);
                CHECK(st.sumRange(l, r) == nv.sumRange(l, r));
            }
        }
    }
}
