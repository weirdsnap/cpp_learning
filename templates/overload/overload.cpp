// 验证 ch04 重载决议主题：
// 1. 模板实例化的精确匹配优于非模板的标准转换
// 2. 同为精确匹配时非模板优先
// 3. f<>() 强制只考虑模板候选

#include <iostream>
#include <string>

void f(int) { std::cout << "non-template int\n"; }

template<typename T>
void f(T) { std::cout << "template T\n"; }

int main() {
    int x = 1;
    f(x);       // 两者都是精确匹配，非模板优先
    f<>(x);     // 强制只考虑模板，调用 template<int>

    short s = 1;
    f(s);       // 非模板需要 short->int 标准转换；模板推导 short 是精确匹配，模板胜出

    return 0;
}
