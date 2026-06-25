#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "widget.h"

TEST_CASE("Pimpl Widget 基本功能") {
    Widget w;
    w.draw();
    CHECK(w.value() == 0);
}

TEST_CASE("Pimpl Widget 移动语义") {
    Widget w1;
    Widget w2 = std::move(w1);
    w2.draw();
}

TEST_CASE("Pimpl Widget 移动赋值") {
    Widget w1;
    Widget w2;
    w2 = std::move(w1);
    w2.draw();
}
