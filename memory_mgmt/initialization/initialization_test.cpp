#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>

class Member {
public:
    static std::string log;
    explicit Member(char c) { log += c; }
};
std::string Member::log;

class InitOrder {
public:
    // 初始化列表顺序与声明顺序相反，但构造仍按声明顺序
    InitOrder() : b_('b'), a_('a') {}
private:
    Member a_;
    Member b_;
};

template<typename T>
struct Holder {
    static int count;
    static bool touched;
};

template<typename T>
int Holder<T>::count = 0;

template<typename T>
bool Holder<T>::touched = []() {
    Holder<T>::count = 100;
    return true;
}();

TEST_CASE("成员构造按类内声明顺序，与初始化列表书写顺序无关") {
    Member::log.clear();
    InitOrder obj;
    CHECK(Member::log == "ab");
}

TEST_CASE("模板静态成员按需实例化") {
    // 访问 Holder<int> 才会实例化其静态成员
    CHECK(Holder<int>::touched == true);
    CHECK(Holder<int>::count == 100);

    // Holder<double> 未被使用，count 保持默认值 0
    CHECK(Holder<double>::count == 0);
}
