// 验证 ch01/27：std::string 的三种操作风格
// 1. C 风格：<cstring> 函数族
// 2. 成员函数 + 操作符重载：std::string
// 3. STL 算法风格：迭代器 + <algorithm>

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <cctype>

// ============================================================
// 1. C 风格：<cstring> 函数族
// ============================================================

void demo_c_style() {
    std::cout << "=== 1. C 风格 <cstring> ===\n";

    char s1[64] = "hello";
    char s2[64] = "world";
    char dest[128];

    // 拷贝
    std::strcpy(dest, s1);
    std::cout << "  strcpy(dest, s1): " << dest << "\n";

    std::strncpy(dest, s1, 3);
    dest[3] = '\0';  // strncpy 不保证补 \0
    std::cout << "  strncpy(dest, s1, 3): " << dest << "\n";

    // 拼接
    std::strcpy(dest, s1);
    std::strcat(dest, " ");
    std::strcat(dest, s2);
    std::cout << "  strcat: " << dest << "\n";

    // 长度
    std::cout << "  strlen(dest) = " << std::strlen(dest) << "\n";

    // 比较
    int cmp = std::strcmp(s1, s2);
    std::cout << "  strcmp(s1, s2) = " << cmp << " (<0 means s1 < s2)\n";

    // 查找子串
    const char* p = std::strstr(dest, "world");
    std::cout << "  strstr(dest, \"world\"): " << (p ? p : "null") << "\n";
}

// ============================================================
// 2. 成员函数 + 操作符重载：std::string
// ============================================================

void demo_string_member() {
    std::cout << "\n=== 2. std::string 成员函数 + 操作符 ===\n";

    std::string s = "hello";

    // 拼接
    s = s + " world";
    s += "!";
    s.append("!!");
    s.push_back('?');
    std::cout << "  after appends: " << s << "\n";

    // 子串
    std::string sub = s.substr(0, 5);
    std::cout << "  substr(0, 5): " << sub << "\n";

    // 查找
    auto pos = s.find("world");
    std::cout << "  find(\"world\"): " << pos << "\n";

    // 插入、删除、替换
    s.insert(5, " C++");
    std::cout << "  insert(5, \" C++\"): " << s << "\n";

    s.erase(5, 4);
    std::cout << "  erase(5, 4): " << s << "\n";

    s.replace(0, 5, "Hi");
    std::cout << "  replace(0, 5, \"Hi\"): " << s << "\n";

    // 长度与访问
    std::cout << "  size() = " << s.size() << "\n";
    std::cout << "  s[0] = " << s[0] << "\n";
    std::cout << "  s.at(0) = " << s.at(0) << "\n";
}

// ============================================================
// 3. STL 算法风格：迭代器 + <algorithm>
// ============================================================

void demo_stl_style() {
    std::cout << "\n=== 3. STL 算法风格 ===\n";

    std::string s = "hello world";

    // 查找字符
    auto it = std::find(s.begin(), s.end(), 'w');
    std::cout << "  find 'w': " << (it != s.end() ? std::string(1, *it) : "not found") << "\n";

    // 反转
    std::string rev = s;
    std::reverse(rev.begin(), rev.end());
    std::cout << "  reverse: " << rev << "\n";

    // 排序
    std::string sorted = s;
    std::sort(sorted.begin(), sorted.end());
    std::cout << "  sorted: " << sorted << "\n";

    // 统计
    int cnt = std::count(s.begin(), s.end(), 'l');
    std::cout << "  count 'l': " << cnt << "\n";

    // 替换
    std::string replaced = s;
    std::replace(replaced.begin(), replaced.end(), 'l', 'L');
    std::cout << "  replace 'l' -> 'L': " << replaced << "\n";

    // 判断回文
    std::string test = "racecar";
    bool is_pal = std::equal(test.begin(), test.begin() + test.size() / 2, test.rbegin());
    std::cout << "  \"" << test << "\" is palindrome: " << std::boolalpha << is_pal << "\n";
}

// ============================================================
// 综合例子：去掉首尾空格 + 转大写
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

int main() {
    demo_c_style();
    demo_string_member();
    demo_stl_style();

    std::cout << "\n=== 综合例子：trim + upper ===\n";
    std::string s = "  hello world  ";
    std::cout << "  input: \"" << s << "\"\n";
    std::cout << "  output: \"" << trim_and_upper(s) << "\"\n";

    return 0;
}
