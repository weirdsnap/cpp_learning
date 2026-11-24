// 类型擦除三段式骨架验证（博客 ch07/12）
//
// 演示 std::function 背后的标准手法：
//   ① 非模板基类 Concept：只描述"能做什么"
//   ② 模板壳子 Model<T>：实例化时知道真实类型
//   ③ 持有者 Func：类型固定，构造函数是模板，捕获类型后擦除
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

struct Concept {
    virtual void call() = 0;
    virtual ~Concept() = default;
};

template <class T>
struct Model : Concept {
    T fn;
    explicit Model(T f) : fn(std::move(f)) {}
    void call() override { fn(); }
};

struct Func {
    std::unique_ptr<Concept> p;
    template <class T>
    Func(T f) : p(std::make_unique<Model<T>>(std::move(f))) {}
    void operator()() { p->call(); }
};

void freeFunction() { std::cout << "free function\n"; }

struct Functor {
    void operator()() const { std::cout << "functor\n"; }
};

int main() {
    // 同一个 Func 类型装三种完全不同的可调用对象
    std::vector<Func> funcs;
    funcs.emplace_back([] { std::cout << "lambda\n"; });
    funcs.emplace_back(&freeFunction);
    funcs.emplace_back(Functor{});
    for (auto& f : funcs) f();

    std::cout << "sizeof(std::function<void()>) = " << sizeof(std::function<void()>) << '\n';

    // SBO 装不下的大对象走堆分配（能否装下是实现细节，但一定能正确工作）
    auto big = [arr = std::array<long long, 8>{}]() mutable { arr[0]++; };
    std::function<void()> sf = big;
    sf();
}
