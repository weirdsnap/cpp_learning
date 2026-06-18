// 验证 ch03 智能指针主题：
// 1. shared_ptr 引用计数共享所有权
// 2. weak_ptr 不增加强引用，通过 lock() 安全观测
// 3. unique_ptr 独占所有权
// 4. 自定义删除器

#include <iostream>
#include <memory>
#include <string>

struct Resource {
    std::string name;
    explicit Resource(std::string n) : name(std::move(n)) {
        std::cout << "Resource(" << name << ")\n";
    }
    ~Resource() { std::cout << "~Resource(" << name << ")\n"; }
};

int main() {
    {
        auto sp1 = std::make_shared<Resource>("shared");
        std::cout << "use_count: " << sp1.use_count() << "\n";
        {
            auto sp2 = sp1;
            std::cout << "use_count: " << sp1.use_count() << "\n";
        }
        std::cout << "use_count: " << sp1.use_count() << "\n";
    }

    {
        auto sp = std::make_shared<Resource>("weak-observed");
        std::weak_ptr<Resource> wp = sp;
        std::cout << "wp expired: " << wp.expired() << "\n";
        if (auto locked = wp.lock()) {
            std::cout << "locked: " << locked->name << "\n";
        }
        sp.reset();
        std::cout << "wp expired: " << wp.expired() << "\n";
    }

    {
        std::unique_ptr<Resource> up = std::make_unique<Resource>("unique");
        // auto up2 = up; // 编译错误：独占所有权不可复制
        auto up2 = std::move(up);
        std::cout << "up null: " << (up == nullptr) << "\n";
    }

    {
        auto custom = std::shared_ptr<Resource>(
            new Resource("custom-deleter"),
            [](Resource* p) {
                std::cout << "custom deleter for " << p->name << "\n";
                delete p;
            });
    }

    return 0;
}
