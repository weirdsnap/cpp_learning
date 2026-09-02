#pragma once
#include <algorithm>
#include <vector>

namespace lc2091 {

using std::vector;
using std::min;
using std::max;

// 用户提交版：一次遍历定位 min/max 下标，三种策略取最小
class Solution {
public:
    int minimumDeletions(vector<int>& nums) {
        const int n = nums.size();
        if (n == 1) return 1;
        int min_idx = 0, max_idx = 0;
        for (int i = 0; i < n; i++) {
            if (nums[min_idx] > nums[i]) min_idx = i;
            if (nums[max_idx] < nums[i]) max_idx = i;
        }
        int temp1 = min(min_idx, max_idx);
        int temp2 = max(min_idx, max_idx);
        int left = temp1 + 1;
        int right = n - temp2;
        int result1 = left + right;
        int result2 = temp2 + 1;
        int result3 = n - temp1;
        return min({result1, result2, result3});
    }
};

// 独立对拍：BFS 模拟真实删除过程（每次从头部或尾部删一个），
// 直到最小值和最大值都被移除，返回最少删除次数。n 小时使用。
inline int bfsOracle(const vector<int>& nums) {
    const int n = (int)nums.size();
    int min_idx = 0, max_idx = 0;
    for (int i = 0; i < n; i++) {
        if (nums[i] < nums[min_idx]) min_idx = i;
        if (nums[i] > nums[max_idx]) max_idx = i;
    }
    // 状态: 剩余窗口 [l, r]，允许 l == r+1 表示空窗口；
    // 已删除次数 = l + (n - 1 - r)。BFS 保证首次命中即最少次数。
    vector<vector<char>> seen(n + 1, vector<char>(n, 0));
    struct State { int l, r; };
    vector<State> q{{0, n - 1}};
    seen[0][n - 1] = 1;
    for (size_t head = 0; head < q.size(); head++) {
        auto [l, r] = q[head];
        if ((min_idx < l || min_idx > r) && (max_idx < l || max_idx > r)) {
            return l + (n - 1 - r);
        }
        if (l <= r) {
            if (!seen[l + 1][r]) { seen[l + 1][r] = 1; q.push_back({l + 1, r}); }
            if (r - 1 >= 0 && !seen[l][r - 1]) { seen[l][r - 1] = 1; q.push_back({l, r - 1}); }
        }
    }
    return -1; // unreachable
}

} // namespace lc2091
