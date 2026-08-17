// LC 2213. 由单个字符重复的最长子字符串 — 博客 leetcode/segment-tree/ch52.md
#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace lc2213 {

// 线段树版（用户实现）：只读根节点的 best，无区间查询
class Solution {
public:
    struct Node {
        char preChar, sufChar;  // 区间最左/最右字符
        int preLen;             // 前缀连续相同字符长度
        int sufLen;             // 后缀连续相同字符长度
        int best;               // 区间内最长连续相同字符长度
        int len;                // 区间长度
    };

    std::vector<int> longestRepeating(std::string s, std::string queryCharacters,
                                      std::vector<int>& queryIndices) {
        int n = s.size();
        std::vector<Node> tree(4 * n);

        auto merge = [](const Node& L, const Node& R) -> Node {
            Node res;
            res.preChar = L.preChar;
            res.sufChar = R.sufChar;
            res.len = L.len + R.len;

            if (L.sufChar == R.preChar) {
                int cross = L.sufLen + R.preLen;
                res.best = std::max(std::max(L.best, R.best), cross);
                res.preLen = (L.preLen == L.len) ? L.preLen + R.preLen : L.preLen;
                res.sufLen = (R.sufLen == R.len) ? L.sufLen + R.sufLen : R.sufLen;
            } else {
                res.best = std::max(L.best, R.best);
                res.preLen = L.preLen;
                res.sufLen = R.sufLen;
            }
            return res;
        };

        auto build = [&](this auto&& self, int node, int l, int r) -> void {
            if (l == r) {
                tree[node] = {s[l], s[l], 1, 1, 1, 1};
                return;
            }
            int mid = (l + r) / 2;
            self(node * 2, l, mid);
            self(node * 2 + 1, mid + 1, r);
            tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
        };

        auto update = [&](this auto&& self, int node, int l, int r, int pos, char c) -> void {
            if (l == r) {
                tree[node] = {c, c, 1, 1, 1, 1};
                return;
            }
            int mid = (l + r) / 2;
            if (pos <= mid) {
                self(node * 2, l, mid, pos, c);
            } else {
                self(node * 2 + 1, mid + 1, r, pos, c);
            }
            tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
        };

        build(1, 0, n - 1);

        std::vector<int> res;
        for (int i = 0; i < static_cast<int>(queryIndices.size()); i++) {
            update(1, 0, n - 1, queryIndices[i], queryCharacters[i]);
            res.push_back(tree[1].best);
        }
        return res;
    }
};

// 暴力参照版：每次修改后全串扫描最长连续段
class SolutionBrute {
public:
    std::vector<int> longestRepeating(std::string s, std::string queryCharacters,
                                      std::vector<int>& queryIndices) {
        std::vector<int> res;
        for (int i = 0; i < static_cast<int>(queryIndices.size()); i++) {
            s[queryIndices[i]] = queryCharacters[i];
            int best = 1, cur = 1;
            for (int j = 1; j < static_cast<int>(s.size()); j++) {
                cur = (s[j] == s[j - 1]) ? cur + 1 : 1;
                best = std::max(best, cur);
            }
            res.push_back(best);
        }
        return res;
    }
};

} // namespace lc2213
