#include "widget.h"

#include <iostream>
#include <string>

// ============================================================
// 私有实现：所有实现细节都在这里
// 修改这个类的成员不会影响头文件，不会破坏 ABI
// ============================================================

class Widget::Impl {
public:
    Impl() : name_("default"), id_(0) {}

    void draw() const {
        std::cout << "  Widget::Impl::draw(): name=" << name_
                  << ", id=" << id_ << "\n";
    }

    int value() const { return id_; }

private:
    std::string name_;
    int id_;
};

// ============================================================
// Widget 公开接口：仅转发到 Impl
// ============================================================

Widget::Widget()
    : pImpl(std::make_unique<Impl>())
{}

// 析构函数必须在 cpp 中定义，因为 unique_ptr 的删除器需要 Impl 的完整定义
Widget::~Widget() = default;

Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::draw() const {
    pImpl->draw();
}

int Widget::value() const {
    return pImpl->value();
}
