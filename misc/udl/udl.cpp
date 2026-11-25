// 验证 ch10/01 用户自定义字面量（UDL）
//
// 1. 物理量 UDL：长度（米、千米）
// 2. 角度 UDL：度、弧度
// 3. 与 std::chrono UDL 对比
// 4. 原始字面量 vs  cooked 字面量

#include <iostream>
#include <chrono>
#include <string>
#include <cmath>

// ============================================================
// 1. 物理量 UDL：长度
// ============================================================

struct Length {
    long double meters;

    Length operator+(const Length& other) const {
        return Length{meters + other.meters};
    }
};

// cooked UDL：带小数点的字面量
Length operator""_km(long double km) {
    return Length{km * 1000.0L};
}

Length operator""_m(long double m) {
    return Length{m};
}

Length operator""_cm(long double cm) {
    return Length{cm / 100.0L};
}

// 整型 UDL
Length operator""_km(unsigned long long km) {
    return Length{static_cast<long double>(km * 1000ULL)};
}

Length operator""_m(unsigned long long m) {
    return Length{static_cast<long double>(m)};
}

void demo_length_udl() {
    std::cout << "=== 1. 长度 UDL ===\n";

    auto dist1 = 5.0_km;
    auto dist2 = 300.0_m;
    auto dist3 = 50.0_cm;
    auto dist4 = 2_km;  // 整型字面量

    std::cout << "  5.0_km  = " << dist1.meters << " m\n";
    std::cout << "  300.0_m = " << dist2.meters << " m\n";
    std::cout << "  50.0_cm = " << dist3.meters << " m\n";
    std::cout << "  2_km    = " << dist4.meters << " m\n";

    // 运算
    Length total = 1.0_km + 500.0_m;
    std::cout << "  1.0_km + 500.0_m = " << total.meters << " m\n";
}

// ============================================================
// 2. 角度 UDL
// ============================================================

struct Angle {
    long double radians;

    long double degrees() const { return radians * 180.0L / M_PI; }
};

Angle operator""_deg(long double deg) {
    return Angle{deg * M_PI / 180.0L};
}

Angle operator""_rad(long double rad) {
    return Angle{rad};
}

void demo_angle_udl() {
    std::cout << "\n=== 2. 角度 UDL ===\n";

    auto a1 = 90.0_deg;
    auto a2 = 1.5708_rad;

    std::cout << "  90.0_deg    = " << a1.radians << " rad\n";
    std::cout << "  1.5708_rad  = " << a2.degrees() << " deg\n";

    // 防止单位混淆
    auto angle = 45.0_deg;
    std::cout << "  sin(45 deg) = " << std::sin(angle.radians) << "\n";
}

// ============================================================
// 3. 与 std::chrono UDL 对比
// ============================================================

void demo_chrono_udl() {
    std::cout << "\n=== 3. std::chrono UDL ===\n";

    using namespace std::chrono_literals;

    auto t1 = 60s;
    auto t2 = 100ms;
    auto t3 = 2.5h;

    std::cout << "  60s   = " << t1.count() << " seconds\n";
    std::cout << "  100ms = " << t2.count() << " milliseconds\n";
    std::cout << "  2.5h  = " << t3.count() << " hours\n";

    // 运算
    auto total = 1h + 30min + 15s;
    std::cout << "  1h + 30min + 15s = " << total.count() << " seconds\n";
}

// ============================================================
// 4. 原始字面量 vs cooked 字面量
// ============================================================

// 原始字面量（raw）：接收 const char*，自己解析
std::string operator""_binary(const char* str, std::size_t len) {
    std::string result;
    for (std::size_t i = 0; i < len; ++i) {
        if (str[i] == '0' || str[i] == '1') {
            result += str[i];
        }
    }
    return result;
}

// cooked 字面量：接收 unsigned long long
std::string operator""_oct(unsigned long long val) {
    // 简单演示：返回八进制字符串
    if (val == 0) return "0";
    std::string result;
    while (val > 0) {
        result = char('0' + (val % 8)) + result;
        val /= 8;
    }
    return result;
}

void demo_raw_vs_cooked() {
    std::cout << "\n=== 4. 原始 vs cooked 字面量 ===\n";

    // 原始字面量：可以处理任意字符序列
    auto bin = "101010"_binary;
    std::cout << "  \"101010\"_binary = " << bin << "\n";

    // cooked 字面量：只能处理数字
    auto oct = 42_oct;
    std::cout << "  42_oct = " << oct << " (42 的八进制)\n";
}

int main() {
    demo_length_udl();
    demo_angle_udl();
    demo_chrono_udl();
    demo_raw_vs_cooked();

    return 0;
}
