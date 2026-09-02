#pragma once

#include <unordered_map>
#include <vector>

namespace lc760 {

// 提交的版本一：单值 pos + 溢出队列 dups
class Solution {
public:
    std::vector<int> anagramMappings(std::vector<int>& nums1,
                                     std::vector<int>& nums2) {
        std::unordered_map<int, int> pos;
        std::unordered_map<int, std::vector<int>> dups;
        for (int i = 0; i < (int)nums2.size(); i++) {
            auto it = pos.find(nums2[i]);
            if (it == pos.end()) {
                pos[nums2[i]] = i;
            } else {
                dups[nums2[i]].push_back(it->second);
                it->second = i;
            }
        }
        std::vector<int> mapping(nums1.size());
        for (int i = 0; i < (int)nums1.size(); i++) {
            auto it = pos.find(nums1[i]);
            mapping[i] = it->second;
            auto d = dups.find(nums1[i]);
            if (d != dups.end() && !d->second.empty()) {
                it->second = d->second.back();
                d->second.pop_back();
            }
        }
        return mapping;
    }
};

// 提交的版本二：值 → 索引栈，pop_back 取用
class SolutionV2 {
public:
    std::vector<int> anagramMappings(std::vector<int>& nums1,
                                     std::vector<int>& nums2) {
        std::unordered_map<int, std::vector<int>> index;
        for (int i = 0; i < (int)nums2.size(); i++) {
            index[nums2[i]].push_back(i);
        }
        std::vector<int> mapping(nums1.size());
        for (int i = 0; i < (int)nums1.size(); i++) {
            auto& v = index[nums1[i]];
            mapping[i] = v.back();
            v.pop_back();
        }
        return mapping;
    }
};

// 答案不唯一：校验合法性而不是比对输出
inline bool validMapping(const std::vector<int>& nums1,
                         const std::vector<int>& nums2,
                         const std::vector<int>& mapping) {
    if (mapping.size() != nums1.size()) return false;
    std::vector<bool> used(nums2.size(), false);
    for (size_t i = 0; i < nums1.size(); i++) {
        int j = mapping[i];
        if (j < 0 || j >= (int)nums2.size()) return false;
        if (used[j]) return false;               // 一个位置只能用一次
        if (nums2[j] != nums1[i]) return false;  // 值必须相等
        used[j] = true;
    }
    return true;
}

} // namespace lc760
