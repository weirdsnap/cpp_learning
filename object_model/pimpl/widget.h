#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include <memory>

// Pimpl 惯用法：将私有实现通过不透明指针转移到实现文件中
// 头文件仅暴露接口，隐藏实现、隔离编译依赖并保持 ABI 稳定
//
// 验证 ch02/16 Pimpl 惯用法

class Widget {
    class Impl;                     // 前置声明，不暴露实现细节
    std::unique_ptr<Impl> pImpl;    // 不透明指针

public:
    Widget();
    ~Widget();                      // 必须在 cpp 中定义（需要 Impl 完整类型）
    Widget(Widget&&) noexcept;      // 移动构造
    Widget& operator=(Widget&&) noexcept;

    void draw() const;
    int value() const;
};

#endif // WIDGET_H
