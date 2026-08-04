// LC 3731. 查找缺失元素 — 博客 leetcode/array/ch44.md
#pragma once

#include <climits>
#include <unordered_set>
#include <vector>

namespace lc3731 {

// 解法一：哈希集合
class Solution {
public:
    std::vector<int> findMissingElements(std::vector<int>& nums) {
        std::unordered_set<int> my_set;
        int min_num = INT_MAX;
        int max_num = INT_MIN;
        for (const auto& num : nums) {
            my_set.insert(num);
            max_num = std::max(max_num, num);
            min_num = std::min(min_num, num);
        }
        std::vector<int> result;
        for (int i = min_num; i < max_num; i++) {
            if (my_set.find(i) == my_set.end()) {
                result.push_back(i);
            }
        }
        return result;
    }
};

// 解法二：值域 1~100 固定，bool 数组替代哈希表
class SolutionArray {
public:
    std::vector<int> findMissingElements(std::vector<int>& nums) {
        bool present[101] = {false};
        int min_num = INT_MAX;
        int max_num = INT_MIN;
        for (const auto& num : nums) {
            present[num] = true;
            max_num = std::max(max_num, num);
            min_num = std::min(min_num, num);
        }
        std::vector<int> result;
        for (int i = min_num; i < max_num; i++) {
            if (!present[i]) {
                result.push_back(i);
            }
        }
        return result;
    }
};

} // namespace lc3731
