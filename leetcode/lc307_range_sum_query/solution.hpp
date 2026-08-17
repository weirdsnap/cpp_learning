// LC 307. 区域和检索 - 数组可修改 — 博客 leetcode/segment-tree/ch50.md
#pragma once

#include <numeric>
#include <vector>

namespace lc307 {

// 线段树版（用户实现）：以 1 为根的堆式索引，pos*2 / pos*2+1 为左右子
class NumArray {
public:
    struct Node {
        int sum;
        int len;
    };

    std::vector<Node> node;
    std::vector<int> nums;

    NumArray(std::vector<int>& nums) {
        this->nums = nums;
        // 以 1 为根时堆式索引最坏约需 4n 空间
        this->node.resize(nums.size() * 4);
        this->build(1, 0, static_cast<int>(nums.size()) - 1);
    }

    void build(int pos, int i, int j) {
        if (i == j) {
            node[pos] = {nums[i], 1};
            return;
        }
        int mid = (i + j) / 2;
        build(pos * 2, i, mid);
        build(pos * 2 + 1, mid + 1, j);
        node[pos] = {node[pos * 2].sum + node[pos * 2 + 1].sum,
                     node[pos * 2].len + node[pos * 2 + 1].len};
    }

    void update(int index, int val) {
        update_helper(1, 0, static_cast<int>(nums.size()) - 1, index, val);
    }

    void update_helper(int pos, int i, int j, int index, int value) {
        if (i == j) {
            nums[i] = value;
            node[pos].sum = value;
            return;
        }
        int mid = (i + j) / 2;
        if (index <= mid) {  // index == mid 归左侧（左区间是 [i, mid]）
            update_helper(pos * 2, i, mid, index, value);
        } else {
            update_helper(pos * 2 + 1, mid + 1, j, index, value);
        }
        node[pos].sum = node[pos * 2].sum + node[pos * 2 + 1].sum;
    }

    int sumRange(int left, int right) {
        return query(1, 0, static_cast<int>(nums.size()) - 1, left, right);
    }

    int query(int pos, int i, int j, int left, int right) {
        if (left > j || right < i) return 0;              // 不相交
        if (left <= i && right >= j) return node[pos].sum; // 全覆盖
        int mid = (i + j) / 2;
        return query(pos * 2, i, mid, left, right)
             + query(pos * 2 + 1, mid + 1, j, left, right);
    }
};

// 朴素参照版：update O(1)，sumRange O(n)
class NumArrayNaive {
public:
    std::vector<int> nums;
    NumArrayNaive(std::vector<int>& n) : nums(n) {}
    void update(int index, int val) { nums[index] = val; }
    int sumRange(int left, int right) {
        return std::accumulate(nums.begin() + left, nums.begin() + right + 1, 0);
    }
};

} // namespace lc307
