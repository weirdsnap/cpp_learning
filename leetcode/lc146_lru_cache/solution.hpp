#pragma once
#include <list>
#include <unordered_map>
#include <utility>

namespace lc146 {

// 用户提交版：哈希表 + 双向链表（list::splice 移头）
class LRUCache {
public:
    LRUCache(int capacity): cap(capacity) {
        m.reserve(cap);
    }

    int get(int key) {
        auto it = m.find(key);
        if (it == m.end()) return -1;
        moveToHead(it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = m.find(key);
        if (it != m.end()) {
            it->second->second = value;
            moveToHead(it->second);
            return;
        }
        if (lst.size() == cap) {
            int lastKey = lst.back().first;
            lst.pop_back();
            m.erase(lastKey);
        }
        lst.push_front({key, value});
        m[key] = lst.begin();
    }

private:
    using ListIter = std::list<std::pair<int,int>>::iterator;

    void moveToHead(ListIter it) {
        lst.splice(lst.begin(), lst, it);
    }

    int cap;
    std::list<std::pair<int,int>> lst;
    std::unordered_map<int, ListIter> m;
};

// 独立参考实现：有序表（前端 = 最近使用），逻辑直白但同样是 O(1) 语义模型
// 用于对拍，不追求速度
class LRURef {
public:
    LRURef(int capacity): cap(capacity) {}

    int get(int key) {
        for (auto it = order.begin(); it != order.end(); ++it) {
            if (it->first == key) {
                int v = it->second;
                order.erase(it);
                order.push_front({key, v});
                return v;
            }
        }
        return -1;
    }

    void put(int key, int value) {
        for (auto it = order.begin(); it != order.end(); ++it) {
            if (it->first == key) {
                order.erase(it);
                order.push_front({key, value});
                return;
            }
        }
        if ((int)order.size() == cap) order.pop_back();
        order.push_front({key, value});
    }

private:
    int cap;
    std::list<std::pair<int,int>> order;
};

} // namespace lc146
