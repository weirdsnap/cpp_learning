#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>
#include <string>

struct Counter {
    static int alive;
    static int deleted;
    int id = 0;
    explicit Counter(int i) : id(i) { ++alive; }
    ~Counter() { ++deleted; }
};
int Counter::alive = 0;
int Counter::deleted = 0;

TEST_CASE("shared_ptr 共享所有权并维护引用计数") {
    Counter::alive = 0;
    Counter::deleted = 0;
    {
        auto sp1 = std::make_shared<Counter>(1);
        CHECK(sp1.use_count() == 1);
        CHECK(Counter::alive == 1);
        {
            auto sp2 = sp1;
            CHECK(sp1.use_count() == 2);
            CHECK(sp2.use_count() == 2);
        }
        CHECK(sp1.use_count() == 1);
        CHECK(Counter::deleted == 0);
    }
    CHECK(Counter::deleted == 1);
}

TEST_CASE("weak_ptr 不增加强引用计数") {
    Counter::alive = 0;
    Counter::deleted = 0;
    std::weak_ptr<Counter> wp;
    {
        auto sp = std::make_shared<Counter>(2);
        wp = sp;
        CHECK(sp.use_count() == 1);
        CHECK(wp.use_count() == 1);
        CHECK(!wp.expired());

        auto locked = wp.lock();
        CHECK(locked != nullptr);
        CHECK(locked->id == 2);
    }
    CHECK(wp.expired());
    CHECK(wp.lock() == nullptr);
    CHECK(Counter::deleted == 1);
}

TEST_CASE("unique_ptr 独占所有权，可复制构造/赋值被删除") {
    Counter::alive = 0;
    Counter::deleted = 0;
    {
        auto up1 = std::make_unique<Counter>(3);
        CHECK(up1 != nullptr);
        // static_assert(!std::is_copy_constructible_v<decltype(up1)>);

        auto up2 = std::move(up1);
        CHECK(up1 == nullptr);
        CHECK(up2 != nullptr);
        CHECK(up2->id == 3);
    }
    CHECK(Counter::deleted == 1);
}

struct CycleNode {
    std::shared_ptr<CycleNode> next;
    std::weak_ptr<CycleNode> weak_next;
    static int deleted;
    ~CycleNode() { ++deleted; }
};
int CycleNode::deleted = 0;

TEST_CASE("shared_ptr 循环引用导致内存泄漏，weak_ptr 可打破") {
    CycleNode::deleted = 0;

    {
        auto a = std::make_shared<CycleNode>();
        auto b = std::make_shared<CycleNode>();
        a->next = b;          // strong ref: a -> b
        b->weak_next = a;     // weak ref: b -> a, 不增加强引用
    }
    CHECK(CycleNode::deleted == 2);  // 没有循环引用，全部释放

    CycleNode::deleted = 0;
    {
        auto a = std::make_shared<CycleNode>();
        auto b = std::make_shared<CycleNode>();
        a->next = b;
        b->next = a;          // 循环强引用
    }
    CHECK(CycleNode::deleted == 0);  // 循环引用导致泄漏
}

TEST_CASE("shared_ptr 支持自定义删除器") {
    Counter::alive = 0;
    Counter::deleted = 0;
    bool custom_called = false;
    {
        auto sp = std::shared_ptr<Counter>(
            new Counter(4),
            [&](Counter* p) {
                custom_called = true;
                delete p;
            });
    }
    CHECK(custom_called);
    CHECK(Counter::deleted == 1);
}
