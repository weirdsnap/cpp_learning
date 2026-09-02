#pragma once

#include <algorithm>
#include <vector>

namespace lc2058 {

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// 提交的原始版本：一趟遍历，三指针
class Solution {
public:
    std::vector<int> nodesBetweenCriticalPoints(ListNode* head) {
        ListNode* last = head;
        ListNode* cur = head->next;
        ListNode* next = cur->next;
        int first_node = -1;
        int last_node = -1;
        int index = 1;
        int min_distance = -1;
        while (next != nullptr) {
            bool is_node =
                (last->val < cur->val && next->val < cur->val) ||
                (last->val > cur->val && next->val > cur->val);
            if (is_node) {
                if (first_node == -1) {
                    first_node = index;
                } else {
                    int distance = index - last_node;
                    min_distance = min_distance == -1
                                       ? distance
                                       : std::min(min_distance, distance);
                }
                last_node = index;
            }
            index++;
            last = cur;
            cur = next;
            next = next->next;
        }
        int max_distance = (first_node != -1 && first_node != last_node)
                               ? last_node - first_node
                               : -1;
        return {min_distance, max_distance};
    }
};

// 参考实现：先收值到数组，找出全部临界点再算距离
class SolutionRef {
public:
    std::vector<int> nodesBetweenCriticalPoints(ListNode* head) {
        std::vector<int> vals;
        for (auto* p = head; p; p = p->next) vals.push_back(p->val);
        std::vector<int> crit;
        for (int i = 1; i + 1 < (int)vals.size(); i++) {
            bool lo = vals[i] < vals[i - 1] && vals[i] < vals[i + 1];
            bool hi = vals[i] > vals[i - 1] && vals[i] > vals[i + 1];
            if (lo || hi) crit.push_back(i);
        }
        if (crit.size() < 2) return {-1, -1};
        int mn = INT32_MAX;
        for (int i = 1; i < (int)crit.size(); i++)
            mn = std::min(mn, crit[i] - crit[i - 1]);
        return {mn, crit.back() - crit.front()};
    }
};

// 测试辅助：vector 建链表，比完即弃（小测试堆上残留可接受）
inline ListNode* build(const std::vector<int>& v) {
    ListNode* head = nullptr;
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        head = new ListNode(*it, head);
    return head;
}

} // namespace lc2058
