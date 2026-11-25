#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>
#include <string>

TEST_CASE("istringstream 只读解析") {
    std::istringstream iss("42 3.14 hello");
    int i; double d; std::string s;
    iss >> i >> d >> s;
    CHECK(i == 42);
    CHECK(d == doctest::Approx(3.14));
    CHECK(s == "hello");
}

TEST_CASE("ostringstream 只写拼接") {
    std::ostringstream oss;
    oss << "Error: " << 404;
    CHECK(oss.str() == "Error: 404");
}

TEST_CASE("提取失败时流状态置位") {
    std::istringstream iss("abc");
    int x = 42;
    iss >> x;
    // C++11 起，提取失败时目标变量保证被修改为 0
    CHECK(x == 0);
    CHECK(iss.fail());
}

TEST_CASE("提取 int 时部分数字可提取") {
    std::istringstream iss("123abc");
    int x = -1;
    iss >> x;
    CHECK(x == 123);
    CHECK_FALSE(iss.fail());
}

TEST_CASE("流状态位检测") {
    std::istringstream iss("123");
    int a, b;

    iss >> a;
    CHECK(a == 123);
    CHECK(iss.eof());   // 读到末尾
    CHECK_FALSE(iss.fail());

    iss >> b;
    CHECK(iss.fail());  // 无法提取
}

TEST_CASE("重置复用") {
    std::ostringstream oss;
    oss << "first";
    CHECK(oss.str() == "first");

    oss.str("");
    oss.clear();
    oss << "second";
    CHECK(oss.str() == "second");
}

TEST_CASE("hex 持久生效") {
    std::ostringstream oss;
    oss << std::hex << 255 << " " << 256;
    CHECK(oss.str() == "ff 100");
}

TEST_CASE("setw 仅影响下一次输出") {
    std::ostringstream oss;
    oss << std::setw(5) << std::setfill('*') << 42;
    oss << 99;
    CHECK(oss.str() == "***4299");
}

TEST_CASE(">> 与 getline 混用需 ignore") {
    std::istringstream iss("42\nJohn Doe\n3.14");
    int id;
    std::string name;

    iss >> id;
    iss.ignore(1, '\n');
    std::getline(iss, name);
    CHECK(id == 42);
    CHECK(name == "John Doe");
}

TEST_CASE("while (iss >> x) 正确循环") {
    std::istringstream iss("1 2 3");
    int sum = 0, x;
    while (iss >> x) {
        sum += x;
    }
    CHECK(sum == 6);
}
