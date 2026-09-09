#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <random>

#include "solution.hpp"

TEST_CASE("LC 146 official example") {
    lc146::LRUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    CHECK(c.get(1) == 1);
    c.put(3, 3);        // 淘汰 2
    CHECK(c.get(2) == -1);
    c.put(4, 4);        // 淘汰 1
    CHECK(c.get(1) == -1);
    CHECK(c.get(3) == 3);
    CHECK(c.get(4) == 4);
}

TEST_CASE("LC 146 edge: capacity 1 and repeated put") {
    lc146::LRUCache c(1);
    c.put(1, 1);
    c.put(2, 2);        // 淘汰 1
    CHECK(c.get(1) == -1);
    CHECK(c.get(2) == 2);
    c.put(2, 3);        // 更新已存在的 key，不淘汰
    CHECK(c.get(2) == 3);
}

TEST_CASE("LC 146 get 也算使用：刷新淘汰顺序") {
    lc146::LRUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    CHECK(c.get(1) == 1);   // 1 变最近使用，2 变成最久未用
    c.put(3, 3);            // 应淘汰 2 而不是 1
    CHECK(c.get(2) == -1);
    CHECK(c.get(1) == 1);
}

TEST_CASE("LC 146 put 已存在 key 也算使用") {
    lc146::LRUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    c.put(1, 10);           // 1 变最近使用
    c.put(3, 3);            // 应淘汰 2
    CHECK(c.get(2) == -1);
    CHECK(c.get(1) == 10);
}

TEST_CASE("LC 146 random ops vs reference model") {
    std::mt19937 rng(146);
    for (int round = 0; round < 200; round++) {
        int cap = 1 + rng() % 8;
        int keySpace = cap + 1 + rng() % 6;   // key 空间略大于容量，逼出淘汰
        lc146::LRUCache c(cap);
        lc146::LRURef ref(cap);
        int ops = 200 + rng() % 800;
        for (int i = 0; i < ops; i++) {
            int key = rng() % keySpace;
            if (rng() % 2) {
                int got = c.get(key);
                int want = ref.get(key);
                REQUIRE_MESSAGE(got == want, "cap=", cap, " key=", key);
            } else {
                int val = rng() % 1000;
                c.put(key, val);
                ref.put(key, val);
            }
        }
    }
}
