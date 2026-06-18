// 验证 ch03 移动与返回值优化主题：
// 1. 移动语义避免深拷贝
// 2. C++17 起返回 prvalue 强制保证不触发拷贝/移动

#include <iostream>
#include <string>
#include <vector>

class Tracer {
public:
    static int copies;
    static int moves;

    Tracer() { std::cout << "default\n"; }
    explicit Tracer(std::string s) : data_(std::move(s)) { std::cout << "ctor\n"; }

    Tracer(const Tracer& other) : data_(other.data_) {
        ++copies;
        std::cout << "copy\n";
    }

    Tracer(Tracer&& other) noexcept : data_(std::move(other.data_)) {
        ++moves;
        std::cout << "move\n";
    }

    Tracer& operator=(const Tracer&) = default;
    Tracer& operator=(Tracer&&) = default;

private:
    std::string data_;
};

int Tracer::copies = 0;
int Tracer::moves = 0;

Tracer make_prvalue() {
    return Tracer("prvalue");  // C++17 guaranteed copy elision
}

Tracer make_named() {
    Tracer t("named");  // NRVO may apply
    return t;
}

int main() {
    Tracer::copies = 0;
    Tracer::moves = 0;

    std::cout << "--- prvalue ---\n";
    Tracer a = make_prvalue();
    std::cout << "copies: " << Tracer::copies << " moves: " << Tracer::moves << "\n";

    Tracer::copies = 0;
    Tracer::moves = 0;
    std::cout << "--- named ---\n";
    Tracer b = make_named();
    std::cout << "copies: " << Tracer::copies << " moves: " << Tracer::moves << "\n";

    Tracer::copies = 0;
    Tracer::moves = 0;
    std::cout << "--- move explicit ---\n";
    Tracer c("source");
    Tracer d = std::move(c);
    std::cout << "copies: " << Tracer::copies << " moves: " << Tracer::moves << "\n";

    return 0;
}
