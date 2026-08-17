// LC 2996. 大于等于顺序前缀和的最小缺失整数 — 博客 leetcode/array/ch49.md
#pragma once

#include <unordered_set>
#include <vector>

namespace lc2996 {

// 线性扫描版：利用 n ≤ 50 的约束，不开哈希表
class Solution {
public:
    int missingInteger(std::vector<int>& nums) {
        int sum = nums[0];
        for (int i = 1; i < static_cast<int>(nums.size()); i++) {
            if (nums[i] != nums[i - 1] + 1) break;
            sum += nums[i];
        }

        while (true) {
            bool found = false;
            for (const int& x : nums) {
                if (x == sum) {
                    found = true;
                    break;
                }
            }
            if (!found) break;
            sum++;
        }
        return sum;
    }
};

// 哈希集合版：语义直观，O(n) 查找
class SolutionSet {
public:
    int missingInteger(std::vector<int>& nums) {
        int sum = nums[0];
        for (int i = 1; i < static_cast<int>(nums.size()); i++) {
            if (nums[i] != nums[i - 1] + 1) break;
            sum += nums[i];
        }
        std::unordered_set<int> s(nums.begin(), nums.end());
        while (s.count(sum)) sum++;
        return sum;
    }
};

} // namespace lc2996
