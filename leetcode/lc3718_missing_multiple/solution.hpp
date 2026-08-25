#pragma once

#include <unordered_set>
#include <vector>

namespace lc3718 {

class Solution {
public:
    int missingMultiple(std::vector<int>& nums, int k) {
        std::unordered_set<int> cnt(nums.begin(), nums.end());
        // 仅需找到 k 的最小倍数且不在集合中；直接倍增即可
        int t = k;
        while (cnt.count(t)) {
            t += k;
        }
        return t;
    }
};

// 参考实现：不用哈希集合，每个倍数直接线性扫原数组
class SolutionLinear {
public:
    int missingMultiple(std::vector<int>& nums, int k) {
        for (int t = k;; t += k) {
            bool found = false;
            for (int x : nums) {
                if (x == t) {
                    found = true;
                    break;
                }
            }
            if (!found) return t;
        }
    }
};

} // namespace lc3718
