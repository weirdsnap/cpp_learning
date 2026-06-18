// 验证 ch02 虚继承主题：
// 虚继承解决菱形继承导致的基类多份拷贝问题。

#include <iostream>
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

// 普通多重继承：Diamond 中包含两份 Base
class DiamondNormal : public Left, public Right {
public:
    std::string read() const {
        // 需要显式指定路径，否则二义性
        return Left::name + "|" + Right::name;
    }
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

// 虚继承：VDiamond 中只有一份 VBase
class VDiamond : public VLeft, public VRight {
public:
    std::string read() const {
        // 直接访问 name，无歧义
        return name;
    }
};

int main() {
    DiamondNormal dn;
    std::cout << dn.read() << std::endl;

    VDiamond vd;
    // 最终派生类 VDiamond 负责构造共享虚基类 VBase
    // 因此 VBase 只被初始化一次，name 由最后执行的派生类构造函数决定
    std::cout << vd.read() << std::endl;

    return 0;
}
