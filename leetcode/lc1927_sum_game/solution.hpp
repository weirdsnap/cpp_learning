// LC 1927. 求和游戏 — 博客 leetcode/greedy/ch64.md
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace lc1927 {

// 提交版：数学结论，O(n)
// Alice 必胜 ⟺ diff != diff_cnt * 4.5
//   diff     = 左半已知数字和 - 右半已知数字和
//   diff_cnt = 右半 '?' 数 - 左半 '?' 数
class Solution {
public:
    bool sumGame(std::string num) {
        int left = 0, right = 0, left_cnt = 0, right_cnt = 0;
        int half = num.size() / 2;
        for (int i = 0; i < (int)num.size(); i++) {
            if (i < half) {
                if (num[i] != '?') left += num[i] - '0';
                else               left_cnt++;
            } else {
                if (num[i] != '?') right += num[i] - '0';
                else               right_cnt++;
            }
        }
        int diff = left - right;
        int diff_cnt = right_cnt - left_cnt;
        return diff != diff_cnt * 4.5;
    }
};

// 整数版变体：避免浮点（4.5 虽可精确表示，整数比较更干净）
// diff != diff_cnt * 4.5  ⟺  2*diff != 9*diff_cnt
class SolutionInt {
public:
    bool sumGame(std::string num) {
        int left = 0, right = 0, left_cnt = 0, right_cnt = 0;
        int half = num.size() / 2;
        for (int i = 0; i < (int)num.size(); i++) {
            if (i < half) {
                if (num[i] != '?') left += num[i] - '0';
                else               left_cnt++;
            } else {
                if (num[i] != '?') right += num[i] - '0';
                else               right_cnt++;
            }
        }
        return 2 * (left - right) != 9 * (right_cnt - left_cnt);
    }
};

// 暴力参照：博弈树全枚举（'?' ≤ 4 时用）。返回 Alice 是否必胜。
class SolutionBrute {
public:
    bool sumGame(std::string num) {
        s = num;
        half = num.size() / 2;
        m = 0;
        for (int i = 0; i < (int)num.size(); i++)
            if (num[i] == '?') pos.push_back(i), m++;
        assign.assign(m, -1);
        return aliceWins(0);  // turn 0 = Alice, 1 = Bob
    }

private:
    std::string s;
    std::vector<int> pos;      // '?' 在原串中的下标
    std::vector<int8_t> assign;
    int half, m;

    // Alice 视角：她能让最终两半和不相等吗
    bool aliceWins(int turn) {
        int p = -1;
        for (int i = 0; i < m; i++)
            if (assign[i] == -1) { p = i; break; }
        if (p == -1) {  // 全部填完，结算
            int left = 0, right = 0, qi = 0;
            for (int i = 0; i < (int)s.size(); i++) {
                int d = (s[i] == '?') ? assign[qi++] : s[i] - '0';
                if (i < half) left += d; else right += d;
            }
            return left != right;
        }
        if (turn == 0) {  // Alice：存在一种填法使她赢
            for (int i = 0; i < m; i++) {
                if (assign[i] != -1) continue;
                for (int d = 0; d <= 9; d++) {
                    assign[i] = d;
                    if (aliceWins(1)) { assign[i] = -1; return true; }
                    assign[i] = -1;
                }
            }
            return false;
        } else {          // Bob：所有填法 Alice 都还能赢，才算 Alice 赢
            for (int i = 0; i < m; i++) {
                if (assign[i] != -1) continue;
                for (int d = 0; d <= 9; d++) {
                    assign[i] = d;
                    if (!aliceWins(0)) { assign[i] = -1; return false; }
                    assign[i] = -1;
                }
            }
            return true;
        }
    }
};

} // namespace lc1927
