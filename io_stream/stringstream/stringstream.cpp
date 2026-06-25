// 验证 ch07 stringstream 家族相关文章
//
// 1. stringstream 家族与继承体系 (ch07/106)
// 2. operator>> 提取陷阱 (ch07/107)
// 3. 流状态位 eof/fail/bad (ch07/108)
// 4. 重置与复用 str()/clear() (ch07/109)
// 5. 格式化操纵器持久性陷阱 (ch07/111)
// 6. >> 与 getline 混用陷阱 (ch07/115)

#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>

// ============================================================
// 1. stringstream 家族
// ============================================================

void demo_stringstream_family() {
    std::cout << "=== 1. stringstream 家族 ===\n";

    // istringstream：只读解析
    std::istringstream iss("42 3.14 hello");
    int i; double d; std::string s;
    iss >> i >> d >> s;
    std::cout << "  istringstream: " << i << " " << d << " " << s << "\n";

    // ostringstream：只写拼接
    std::ostringstream oss;
    oss << "Error code: " << 404 << ", path: /api";
    std::cout << "  ostringstream: " << oss.str() << "\n";

    // stringstream：双向
    std::stringstream ss;
    ss << "data";
    ss.seekg(0);
    std::string r;
    ss >> r;
    std::cout << "  stringstream bidirectional: " << r << "\n";
}

// ============================================================
// 2. operator>> 提取陷阱
// ============================================================

void demo_extraction_traps() {
    std::cout << "\n=== 2. operator>> 提取陷阱 ===\n";

    // 提取到 int 时前导非数字直接失败
    std::istringstream iss1("abc123");
    int x = -1;
    iss1 >> x;
    std::cout << "  int from \"abc123\": x=" << x << " (未修改), fail="
              << iss1.fail() << "\n";

    // 提取到 int 时部分数字可提取
    std::istringstream iss2("123abc");
    int y = -1;
    iss2 >> y;
    std::cout << "  int from \"123abc\": y=" << y << ", fail="
              << iss2.fail() << ", remaining: ";
    std::string rest;
    iss2 >> rest;
    std::cout << rest << "\n";

    // 提取 string 以空白分隔
    std::istringstream iss3("hello world");
    std::string a, b;
    iss3 >> a >> b;
    std::cout << "  string from \"hello world\": a=\"" << a << "\", b=\"" << b << "\"\n";
}

// ============================================================
// 3. 流状态位 eof/fail/bad
// ============================================================

void demo_stream_state() {
    std::cout << "\n=== 3. 流状态位 ===\n";

    std::istringstream iss("123");
    int a, b;

    iss >> a;
    std::cout << "  After reading 123: eof=" << iss.eof()
              << " fail=" << iss.fail() << " good=" << iss.good() << "\n";

    iss >> b;
    std::cout << "  After reading again: eof=" << iss.eof()
              << " fail=" << iss.fail() << " good=" << iss.good() << "\n";

    // 经典循环
    std::istringstream nums("1 2 3");
    int x;
    std::cout << "  while (iss >> x): ";
    while (nums >> x) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // 错误：while (!iss.eof()) 会导致多读一次
    std::istringstream nums2("1 2 3");
    std::cout << "  while (!iss.eof()) (错误方式): ";
    int y;
    while (!nums2.eof()) {
        nums2 >> y;
        if (nums2.good()) std::cout << y << " ";
    }
    std::cout << "(最后一次失败但已进入循环)\n";
}

// ============================================================
// 4. 重置与复用
// ============================================================

void demo_reset_reuse() {
    std::cout << "\n=== 4. 重置与复用 ===\n";

    std::ostringstream oss;
    oss << "First log: " << 42;
    std::cout << "  " << oss.str() << "\n";

    // 复用：str("") 清空内容，clear() 清除状态位
    oss.str("");
    oss.clear();
    oss << "Second log: " << 99;
    std::cout << "  " << oss.str() << "\n";

    // 注意：str("") 不保证释放内存，capacity 可能保留
    std::cout << "  After str(\"\"), capacity = " << oss.str().capacity() << "\n";
}

// ============================================================
// 5. 格式化操纵器持久性陷阱
// ============================================================

void demo_manipulator_persistence() {
    std::cout << "\n=== 5. 操纵器持久性陷阱 ===\n";

    std::ostringstream oss;

    // hex 持久生效
    oss << std::hex << 255 << " ";
    oss << 256;
    std::cout << "  hex persistent: " << oss.str() << " (ff 100)\n";

    // setw 仅影响下一次输出
    oss.str(""); oss.clear();
    oss << std::setw(10) << std::setfill('*') << std::left << 42;
    oss << 99;
    std::cout << "  setw only once: " << oss.str() << " (42********99)\n";

    // boolalpha 持久
    oss.str(""); oss.clear();
    oss << std::boolalpha << true << " " << false;
    std::cout << "  boolalpha: " << oss.str() << "\n";

    // RAII 守卫保存/恢复状态
    oss.str(""); oss.clear();
    auto old_flags = oss.flags();
    oss << std::hex << 255;
    oss.flags(old_flags);  // 恢复
    oss << " " << 255;
    std::cout << "  RAII guard: " << oss.str() << " (ff 255)\n";
}

// ============================================================
// 6. >> 与 getline 混用陷阱
// ============================================================

void demo_mixed_extraction() {
    std::cout << "\n=== 6. >> 与 getline 混用陷阱 ===\n";

    // 错误方式：>> 留下换行
    std::istringstream iss("42\nJohn Doe\n3.14");
    int id;
    std::string name;
    double score;

    iss >> id;
    std::getline(iss, name);  // ❌ 读到空行
    std::cout << "  Without ignore: id=" << id << " name=\"" << name << "\"\n";

    // 正确方式：插入 ignore()
    std::istringstream iss2("42\nJohn Doe\n3.14");
    iss2 >> id;
    iss2.ignore(1, '\n');     // 消耗换行
    std::getline(iss2, name); // ✅ name = "John Doe"
    iss2 >> score;
    std::cout << "  With ignore: id=" << id << " name=\"" << name
              << "\" score=" << score << "\n";
}

int main() {
    demo_stringstream_family();
    demo_extraction_traps();
    demo_stream_state();
    demo_reset_reuse();
    demo_manipulator_persistence();
    demo_mixed_extraction();

    return 0;
}
