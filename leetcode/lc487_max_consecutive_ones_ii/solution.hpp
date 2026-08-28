#pragma once

#include <algorithm>
#include <vector>

namespace lc487 {

// 提交的原始版本：段追踪，O(1) 空间
class Solution {
public:
    int findMaxConsecutiveOnes(std::vector<int>& nums) {
        int last = 0;      // 上一段连续 1 的长度
        int cur = 0;       // 当前段连续 1 的长度
        int result = 0;    // 允许翻转一个 0 后的最长连续 1

        for (int i = 0; i < (int)nums.size(); i++) {
            if (nums[i] == 1) {
                cur++;
            } else {
                // 遇到 0：尝试把前一段 + 当前段 + 翻转的这个 0 拼接起来
                result = std::max(result, last + cur + 1);
                last = cur;
                cur = 0;
            }
        }
        result = std::max(result, last + cur + 1);

        return std::min((int)nums.size(), result);
    }
};

// 参考实现：滑动窗口模板，窗口内 0 的个数 ≤ 1，最长型
class SolutionSW {
public:
    int findMaxConsecutiveOnes(std::vector<int>& nums) {
        int i = 0, zeros = 0, result = 0;
        for (int j = 0; j < (int)nums.size(); j++) {
            if (nums[j] == 0) zeros++;
            while (zeros > 1) {
                if (nums[i] == 0) zeros--;
                i++;
            }
            result = std::max(result, j - i + 1);
        }
        return result;
    }
};

} // namespace lc487
