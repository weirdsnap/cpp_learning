// 验证 ch03 对象所有权与特殊成员函数主题：
// 1. 拷贝构造 vs 拷贝赋值
// 2. 三/五/零法则
// 3. 用户声明拷贝构造抑制移动构造

#include <iostream>
#include <memory>
#include <string>

class RuleOfFive {
public:
    explicit RuleOfFive(int n) : size_(n), data_(new int[n]) {
        std::cout << "ctor " << size_ << "\n";
    }

    ~RuleOfFive() {
        std::cout << "dtor " << size_ << "\n";
        delete[] data_;
    }

    // 拷贝构造
    RuleOfFive(const RuleOfFive& other) : size_(other.size_), data_(new int[other.size_]) {
        std::cout << "copy ctor\n";
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // 拷贝赋值
    RuleOfFive& operator=(const RuleOfFive& other) {
        std::cout << "copy assign\n";
        if (this != &other) {
            int* new_data = new int[other.size_];
            std::copy(other.data_, other.data_ + other.size_, new_data);
            delete[] data_;
            data_ = new_data;
            size_ = other.size_;
        }
        return *this;
    }

    // 移动构造
    RuleOfFive(RuleOfFive&& other) noexcept : size_(other.size_), data_(other.data_) {
        std::cout << "move ctor\n";
        other.size_ = 0;
        other.data_ = nullptr;
    }

    // 移动赋值
    RuleOfFive& operator=(RuleOfFive&& other) noexcept {
        std::cout << "move assign\n";
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    int size() const { return size_; }

private:
    int size_;
    int* data_;
};

// 零法则：用智能指针管理资源，无需自定义特殊成员
class RuleOfZero {
public:
    explicit RuleOfZero(int n) : data_(std::make_unique<int[]>(n)), size_(n) {}
    int size() const { return size_; }
private:
    std::unique_ptr<int[]> data_;
    int size_;
};

// 用户声明拷贝构造，移动构造被抑制
class CopyOnly {
public:
    std::string tag;
    explicit CopyOnly(std::string t) : tag(std::move(t)) {}
    CopyOnly(const CopyOnly&) = default;
    // 未声明移动构造，编译器不会自动生成
};

int main() {
    RuleOfFive a(10);
    RuleOfFive b = a;       // 拷贝构造
    RuleOfFive c(5);
    c = a;                  // 拷贝赋值

    RuleOfFive d = std::move(a);  // 移动构造

    RuleOfZero rz1(20);
    RuleOfZero rz2 = std::move(rz1);  // 默认移动构造可用

    CopyOnly co("x");
    CopyOnly co2 = std::move(co);  // 实际调用拷贝构造，因为移动被抑制

    return 0;
}
