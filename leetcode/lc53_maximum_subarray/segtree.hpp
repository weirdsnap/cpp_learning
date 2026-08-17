// LC 53. 最大子数组和（线段树版）— 博客 leetcode/dp/linear/ch19.md
#pragma once

#include <algorithm>
#include <vector>

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
#define LC53_HAS_SEGTREE 1

namespace lc53 {

// 线段树版（用户实现）：四字段节点，只建树读根，无 update/query
class SolutionSegTree {
public:
    struct Node {
        int pre, suf, sum, best;
    };

    int maxSubArray(std::vector<int>& nums) {
        std::vector<Node> tree(nums.size() * 4);

        auto merge = [&](const Node& left, const Node& right) {
            Node res;
            res.sum = left.sum + right.sum;
            res.pre = std::max(left.pre, left.sum + right.pre);
            res.suf = std::max(right.suf, right.sum + left.suf);
            res.best = std::max({left.best, right.best, left.suf + right.pre});
            return res;
        };

        auto build = [&](this auto&& self, int node, int l, int r) -> void {
            if (l == r) {
                tree[node] = {nums[l], nums[l], nums[l], nums[l]};
                return;
            }
            int mid = (l + r) / 2;
            self(node * 2, l, mid);
            self(node * 2 + 1, mid + 1, r);
            tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
        };

        build(1, 0, static_cast<int>(nums.size()) - 1);
        return tree[1].best;
    }
};

} // namespace lc53

#endif // __cpp_explicit_this_parameter

namespace lc53 {

// Kadane 参照版
class SolutionKadane {
public:
    int maxSubArray(std::vector<int>& nums) {
        int last = nums[0], result = last;
        for (int i = 1; i < static_cast<int>(nums.size()); i++) {
            int cur = std::max(nums[i], last + nums[i]);
            result = std::max(result, cur);
            last = cur;
        }
        return result;
    }
};

} // namespace lc53
