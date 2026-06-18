// 验证 ch03 初始化主题：
// 1. 成员按声明顺序构造/析构，与初始化列表书写顺序无关
// 2. 模板静态成员按需实例化

#include <iostream>
#include <string>

class Member {
public:
    static std::string log;
    explicit Member(char c) { log += c; }
};

std::string Member::log;

class InitOrder {
public:
    // 初始化列表先写 b 后写 a，但构造顺序仍按声明顺序 a -> b
    InitOrder() : b_('b'), a_('a') {}

private:
    Member a_;
    Member b_;
};

template<typename T>
struct Holder {
    static int count;
};

template<typename T>
int Holder<T>::count = 0;

int main() {
    Member::log.clear();
    {
        InitOrder obj;
    }
    std::cout << "construction order: " << Member::log << "\n";

    Holder<int>::count = 42;
    std::cout << "Holder<int>::count: " << Holder<int>::count << "\n";

    return 0;
}
