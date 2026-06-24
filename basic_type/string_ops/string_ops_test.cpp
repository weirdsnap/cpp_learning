#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <algorithm>
#include <cstring>
#include <cctype>

// ============================================================
// 1. C 风格 <cstring> 函数
// ============================================================

TEST_CASE("C 风格 strcpy/strcat") {
    char buf[64] = {0};
    std::strcpy(buf, "hello");
    CHECK(std::string(buf) == "hello");

    std::strcat(buf, " world");
    CHECK(std::string(buf) == "hello world");
}

TEST_CASE("C 风格 strlen") {
    CHECK(std::strlen("") == 0);
    CHECK(std::strlen("hello") == 5);
}

TEST_CASE("C 风格 strcmp") {
    CHECK(std::strcmp("abc", "abc") == 0);
    CHECK(std::strcmp("abc", "abd") < 0);
    CHECK(std::strcmp("abd", "abc") > 0);
}

TEST_CASE("C 风格 strstr 查找子串") {
    const char* s = "hello world";
    const char* p = std::strstr(s, "world");
    CHECK(p != nullptr);
    CHECK(std::string(p) == "world");

    CHECK(std::strstr(s, "xyz") == nullptr);
}

// ============================================================
// 2. std::string 成员函数
// ============================================================

TEST_CASE("string 拼接操作") {
    std::string s = "hello";
    s += " world";
    CHECK(s == "hello world");

    s.append("!!");
    CHECK(s == "hello world!!");

    s.push_back('?');
    CHECK(s == "hello world!!?");
}

TEST_CASE("string 子串 substr") {
    std::string s = "hello world";
    CHECK(s.substr(0, 5) == "hello");
    CHECK(s.substr(6, 5) == "world");
}

TEST_CASE("string 查找 find") {
    std::string s = "hello world";
    CHECK(s.find("world") == 6);
    CHECK(s.find("xyz") == std::string::npos);
}

TEST_CASE("string 插入/删除/替换") {
    std::string s = "hello";
    s.insert(5, " world");
    CHECK(s == "hello world");

    s.erase(5, 6);
    CHECK(s == "hello");

    s.replace(0, 5, "Hi");
    CHECK(s == "Hi");
}

TEST_CASE("string 访问操作符 vs at") {
    std::string s = "hello";
    CHECK(s[0] == 'h');
    CHECK(s.at(0) == 'h');

    s[0] = 'H';
    CHECK(s == "Hello");
}

// ============================================================
// 3. STL 算法风格
// ============================================================

TEST_CASE("STL find 查找字符") {
    std::string s = "hello world";
    auto it = std::find(s.begin(), s.end(), 'w');
    CHECK(it != s.end());
    CHECK(*it == 'w');

    it = std::find(s.begin(), s.end(), 'z');
    CHECK(it == s.end());
}

TEST_CASE("STL reverse 反转字符串") {
    std::string s = "hello";
    std::reverse(s.begin(), s.end());
    CHECK(s == "olleh");
}

TEST_CASE("STL count 统计字符出现次数") {
    std::string s = "hello world";
    int cnt = std::count(s.begin(), s.end(), 'l');
    CHECK(cnt == 3);
}

TEST_CASE("STL replace 替换字符") {
    std::string s = "hello";
    std::replace(s.begin(), s.end(), 'l', 'L');
    CHECK(s == "heLLo");
}

TEST_CASE("STL equal 判断回文") {
    std::string s = "racecar";
    bool is_pal = std::equal(s.begin(), s.begin() + s.size() / 2, s.rbegin());
    CHECK(is_pal);

    s = "hello";
    is_pal = std::equal(s.begin(), s.begin() + s.size() / 2, s.rbegin());
    CHECK_FALSE(is_pal);
}

// ============================================================
// 综合：trim + upper
// ============================================================

std::string trim_and_upper(const std::string& s) {
    auto left = std::find_if(s.begin(), s.end(),
                             [](unsigned char c) { return !std::isspace(c); });
    auto right = std::find_if(s.rbegin(), s.rend(),
                              [](unsigned char c) { return !std::isspace(c); }).base();

    // 全空格字符串：left >= right，返回空字符串
    if (left >= right) return {};

    std::string result(left, right);
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

TEST_CASE("trim_and_upper 综合例子") {
    CHECK(trim_and_upper("  hello world  ") == "HELLO WORLD");
    CHECK(trim_and_upper("hello") == "HELLO");
    CHECK(trim_and_upper("") == "");
    CHECK(trim_and_upper("   ") == "");
}
