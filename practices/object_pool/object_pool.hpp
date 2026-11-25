// 对象池（Object Pool）— 博客 cpp/practices/object_pool_01.md
//
// 核心思想：一次性分配大块内存切成槽位，空闲槽位用侵入式链表串起来。
// create  = 从 freelist 摘一个槽位 + placement new 构造
// destroy = 显式调析构 + 槽位插回 freelist 头部
#pragma once

#include <cstddef>
#include <memory>
#include <new>
#include <utility>
#include <vector>

namespace object_pool {

template <typename T, std::size_t ChunkSize = 64>
class ObjectPool {
    // 槽位复用：空闲时当链表节点用，占用时当对象用。
    // union 的大小自动取 max(sizeof(T), sizeof(Slot*))，对齐也自动满足。
    union Slot {
        T obj;
        Slot* next;
        Slot() {}   // 不构造任何成员，内存保持原始状态
        ~Slot() {}  // 析构由池手动管理
    };

    // 已分配的块。块本身只进不出，保证对象地址稳定（不搬移）
    std::vector<std::unique_ptr<Slot[]>> chunks_;
    Slot* free_list_ = nullptr;

    void grow() {
        chunks_.push_back(std::make_unique<Slot[]>(ChunkSize));
        Slot* chunk = chunks_.back().get();
        // 新块整体串进 freelist：最后一个槽位指向旧头，然后头更新为块首
        for (std::size_t i = 0; i < ChunkSize - 1; ++i) {
            chunk[i].next = &chunk[i + 1];
        }
        chunk[ChunkSize - 1].next = free_list_;
        free_list_ = chunk;
    }

public:
    ObjectPool() = default;
    // 池不可拷贝（拷贝会共享槽位所有权，语义混乱）
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    ~ObjectPool() {
        // 注意：池只释放内存，不会替你析构还活着的对象。
        // 使用者必须保证 destroy 所有对象后再销毁池（与 std::pmr 池同语义）。
    }

    template <typename... Args>
    T* create(Args&&... args) {
        if (!free_list_) grow();
        Slot* s = free_list_;
        free_list_ = free_list_->next;
        return new (&s->obj) T(std::forward<Args>(args)...);  // placement new
    }

    void destroy(T* p) {
        p->~T();  // 显式析构，但不释放内存
        Slot* s = reinterpret_cast<Slot*>(p);
        s->next = free_list_;  // 就地复用对象内存当链表节点
        free_list_ = s;
    }
};

} // namespace object_pool
