#pragma once
#include <algorithm>
#include <functional>
#include <vector>

namespace lc3876 {

using std::vector;
using std::min;
using std::min_element;

// 用户提交版：最小元素定目标奇偶 + 逐元素可行性检查
class Solution {
public:
    bool uniformArray(vector<int>& nums1) {
        int min_val = *min_element(nums1.begin(), nums1.end());

        if (min_val % 2) {  // 目标：全奇数
            int min_odd = min_val;
            for (int num : nums1) {
                if (num % 2) {
                    min_odd = min(min_odd, num);
                } else {
                    if (num < min_odd) {
                        return false;
                    }
                    min_odd = min(min_odd, num - min_odd);
                }
            }
            return true;
        } else {  // min 为偶数：若存在奇数则全偶不可行
            for (int num : nums1) {
                if (num % 2 == 1) {
                    return false;
                }
            }
            return true;
        }
    }
};

// 化简版：奇数分支实际恒真（min 是全局最小且为奇 → 所有偶数 x > min，
// x - min >= 1 且为奇），只需两行判定
class SolutionV2 {
public:
    bool uniformArray(vector<int>& nums1) {
        int mn = *min_element(nums1.begin(), nums1.end());
        if (mn % 2) return true;  // 最小为奇：全奇总能构造
        return std::none_of(nums1.begin(), nums1.end(),
                            [](int x) { return x % 2 == 1; });
    }
};

// 独立对拍：n 小时枚举每个下标的选择（保持 / 减去任一更小的 nums1[j]），
// 检查是否存在全同奇偶的组合。真正的指数级暴力。
inline bool bruteForce(const vector<int>& nums1) {
    int n = nums1.size();
    vector<vector<int>> options(n);
    for (int i = 0; i < n; i++) {
        options[i].push_back(nums1[i]);
        for (int j = 0; j < n; j++) {
            if (j != i && nums1[i] - nums1[j] >= 1) {
                options[i].push_back(nums1[i] - nums1[j]);
            }
        }
    }
    // DFS 枚举选择组合；usedMask: bit0=已出现偶, bit1=已出现奇，只能有一种
    std::function<bool(int, int)> dfs = [&](int i, int usedMask) -> bool {
        if (i == n) {
            return usedMask != 3;
        }
        for (int v : options[i]) {
            int bit = (v & 1) ? 2 : 1;
            if (usedMask & (3 ^ bit)) continue;  // 已锁定另一种奇偶
            if (dfs(i + 1, usedMask | bit)) return true;
        }
        return false;
    };
    return dfs(0, 0);
}

} // namespace lc3876

namespace lc3875 {
// 官方答案：恒 true（无 >= 1 约束，奇偶互补总能构造）
class Solution {
public:
    bool uniformArray(std::vector<int>&) { return true; }
};
} // namespace lc3875
