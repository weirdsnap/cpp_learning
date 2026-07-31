// 博客 cpp/ch07/34：max 家族（双参数 / initializer_list / max_element / ranges::max）
#pragma once

#include <algorithm>
#include <vector>

namespace stl_minmax {

// 双参数版返回 const T&
inline int max_two(int a, int b) { return std::max(a, b); }

// initializer_list 版（C++11）返回值
inline int max_list(std::initializer_list<int> il) { return std::max(il); }

// max_element 返回迭代器，空容器返回 end()
inline int max_elem(const std::vector<int>& v) {
    auto it = std::max_element(v.begin(), v.end());
    return it == v.end() ? -1 : *it;  // 调用方负责判空
}

inline bool max_elem_empty_returns_end(const std::vector<int>& v) {
    return std::max_element(v.begin(), v.end()) == v.end();
}

#if __cplusplus >= 202002L
// ranges::max 直接吃 range，返回值（空 range 是 UB，勿传空容器）
inline int ranges_max(const std::vector<int>& v) { return std::ranges::max(v); }
#endif

} // namespace stl_minmax
