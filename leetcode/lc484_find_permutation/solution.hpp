#pragma once
#include <algorithm>
#include <stack>
#include <string>
#include <vector>

namespace lc484 {

using std::reverse;
using std::string;
using std::vector;

// 用户提交版：构建递增排列，连续 D 段逆序
class Solution {
public:
    vector<int> findPermutation(string s) {
        const int n = s.size() + 1;
        vector<int> perm(n, 1);
        int cnt = 0;

        for (int i = 0; i < n; i++) {
            char cur = s[i];  // i == n-1 时读 s[s.size()]，C++11 起合法返回 '\0'

            if (i == n - 1) {
                cur = 'I';  // 哨兵：强制触发最后一次 reverse
            } else {
                perm[i + 1] = i + 2;
            }

            if (cur == 'D') {
                cnt++;
            } else {
                if (cnt > 0) {
                    reverse(perm.begin() + i - cnt, perm.begin() + i + 1);
                    cnt = 0;
                }
            }
        }
        return perm;
    }
};

// 栈写法：遇 'I' 或末尾弹空栈，LIFO 天然把连续 D 段逆序
class SolutionStack {
public:
    vector<int> findPermutation(string s) {
        const int n = s.size() + 1;
        vector<int> perm;
        std::stack<int> st;

        for (int i = 0; i < n; i++) {
            st.push(i + 1);  // 依次压入下一个数字

            // 遇到 'I' 或到末尾：弹空栈，
            // 栈顶先出的特性正好把连续 'D' 段逆序
            if (i == n - 1 || s[i] == 'I') {
                while (!st.empty()) {
                    perm.push_back(st.top());
                    st.pop();
                }
            }
        }
        return perm;
    }
};

// 独立对拍：n 小，枚举 [1..n] 全部排列，滤出匹配 s 的，取字典序最小
inline vector<int> bruteForce(const string& s) {
    int n = s.size() + 1;
    vector<int> p(n);
    for (int i = 0; i < n; i++) p[i] = i + 1;
    vector<int> best;
    do {
        bool ok = true;
        for (int i = 0; i < n - 1; i++) {
            if (s[i] == 'I' && !(p[i] < p[i + 1])) { ok = false; break; }
            if (s[i] == 'D' && !(p[i] > p[i + 1])) { ok = false; break; }
        }
        if (ok && (best.empty() || p < best)) best = p;
    } while (std::next_permutation(p.begin(), p.end()));
    return best;
}

} // namespace lc484
