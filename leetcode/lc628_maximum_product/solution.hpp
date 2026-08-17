// LC 628. 三个数的最大乘积 — 博客 leetcode/sorting/ch54.md
#pragma once

#include <algorithm>
#include <climits>
#include <vector>

namespace lc628 {

// 排序版：O(n log n)
class Solution {
public:
    int maximumProduct(std::vector<int>& nums) {
        const int n = nums.size();
        std::sort(nums.begin(), nums.end());
        return std::max(
            nums[n - 1] * nums[n - 2] * nums[n - 3],
            nums[0] * nums[1] * nums[n - 1]
        );
    }
};

// 单趟扫描版：只维护最大的 3 个和最小的 2 个，O(n)
class SolutionScan {
public:
    int maximumProduct(std::vector<int>& nums) {
        int max1 = INT_MIN, max2 = INT_MIN, max3 = INT_MIN;
        int min1 = INT_MAX, min2 = INT_MAX;
        for (int num : nums) {
            if (num > max1) {
                max3 = max2; max2 = max1; max1 = num;
            } else if (num > max2) {
                max3 = max2; max2 = num;
            } else if (num > max3) {
                max3 = num;
            }
            if (num < min1) {
                min2 = min1; min1 = num;
            } else if (num < min2) {
                min2 = num;
            }
        }
        return std::max(max1 * max2 * max3, min1 * min2 * max1);
    }
};

// 暴力参照：枚举所有三元组
class SolutionBrute {
public:
    int maximumProduct(std::vector<int>& nums) {
        int n = nums.size(), best = INT_MIN;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                for (int k = j + 1; k < n; k++)
                    best = std::max(best, nums[i] * nums[j] * nums[k]);
        return best;
    }
};

} // namespace lc628
