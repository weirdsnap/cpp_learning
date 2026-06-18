#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>

class Base {
public:
    std::string name = "Base";
    virtual ~Base() = default;
};

class Left : public Base {
public:
    Left() { name = "Left"; }
};

class Right : public Base {
public:
    Right() { name = "Right"; }
};

class DiamondNormal : public Left, public Right {
public:
    std::string read() const { return Left::name + "|" + Right::name; }
};

class VBase {
public:
    std::string name = "VBase";
    virtual ~VBase() = default;
};

class VLeft : virtual public VBase {
public:
    VLeft() { name = "VLeft"; }
};

class VRight : virtual public VBase {
public:
    VRight() { name = "VRight"; }
};

class VDiamond : public VLeft, public VRight {
public:
    std::string read() const { return name; }
};

TEST_CASE("普通多重继承保留多份基类子对象") {
    DiamondNormal dn;
    CHECK(dn.read() == "Left|Right");
}

TEST_CASE("虚继承只保留一份共享虚基类子对象") {
    VDiamond vd;
    // 构造顺序：VBase -> VLeft -> VRight -> VDiamond
    CHECK(vd.read() == "VRight");
}

TEST_CASE("虚继承下共享基类成员访问无歧义") {
    VDiamond vd;
    // 不需要 Left::name / Right::name 限定
    CHECK(vd.name == "VRight");
}
