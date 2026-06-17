#pragma once

#include <iostream>

// 用户自定义字面量（UDL）示例：距离单位
struct Distance {
    long double meters;
};

inline Distance operator"" _km(long double value) {
    return Distance{value * 1000.0L};
}

inline Distance operator"" _m(long double value) {
    return Distance{value};
}

inline std::ostream& operator<<(std::ostream& os, const Distance& d) {
    return os << d.meters << "m";
}
