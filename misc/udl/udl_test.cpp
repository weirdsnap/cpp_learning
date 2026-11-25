#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <chrono>

struct Length {
    long double meters;
    bool operator==(const Length& other) const {
        return std::abs(meters - other.meters) < 0.001L;
    }
};

Length operator""_km(long double km) { return Length{km * 1000.0L}; }
Length operator""_m(long double m)   { return Length{m}; }
Length operator""_cm(long double cm) { return Length{cm / 100.0L}; }
Length operator""_km(unsigned long long km) { return Length{static_cast<long double>(km * 1000ULL)}; }
Length operator""_m(unsigned long long m)   { return Length{static_cast<long double>(m)}; }

TEST_CASE("长度 UDL") {
    auto d1 = 5.0_km;
    CHECK(d1.meters == doctest::Approx(5000.0L));

    auto d2 = 300.0_m;
    CHECK(d2.meters == doctest::Approx(300.0L));

    auto d3 = 50.0_cm;
    CHECK(d3.meters == doctest::Approx(0.5L));

    auto d4 = 2_km;
    CHECK(d4.meters == doctest::Approx(2000.0L));
}

TEST_CASE("chrono UDL") {
    using namespace std::chrono_literals;

    auto t1 = 60s;
    CHECK(t1.count() == 60);

    auto t2 = 100ms;
    CHECK(t2.count() == 100);

    auto total = 1h + 30min;
    // 1h + 30min 的结果是 minutes（90分钟）
    CHECK(total.count() == 90);
}

// 原始字面量
std::string operator""_binary(const char* str, std::size_t len) {
    std::string result;
    for (std::size_t i = 0; i < len; ++i) {
        if (str[i] == '0' || str[i] == '1') result += str[i];
    }
    return result;
}

TEST_CASE("原始字面量 UDL") {
    auto bin = "101010"_binary;
    CHECK(bin == "101010");
}
