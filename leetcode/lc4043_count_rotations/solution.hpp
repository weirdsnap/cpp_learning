#pragma once
#include <string>

namespace lc4043 {

using std::string;

// 用户提交版：统计循环相邻相等对，按 k 与 cnt 的关系直接出答案
class Solution {
public:
    int countRotations(string s, int k) {
        const int n = s.size();

        // 统计原字符串中「循环相邻相等」的对数（把首尾也视作相邻）
        int cnt = s[0] == s[n - 1] ? 1 : 0;

        char last = s[0];
        for (int i = 1; i < n; i++) {
            if (last == s[i]) {
                cnt++;
            }
            last = s[i];
        }

        if (k == cnt) {
            // 切开点不落在相等对中间：n - cnt 个位置
            return n - cnt;
        }
        if (k == cnt - 1) {
            // 少一对：切开点必须落在某一相等对中间，共 cnt 个位置
            return cnt;
        }
        return 0;
    }
};

// 独立对拍：枚举全部 n 个旋转，逐个线性统计相邻相等对
inline int bruteForce(const string& s, int k) {
    const int n = s.size();
    int ans = 0;
    for (int cut = 0; cut < n; cut++) {
        // 旋转后字符串：s[cut..n-1] + s[0..cut-1]
        int eq = 0;
        for (int i = 0; i + 1 < n; i++) {
            char a = s[(cut + i) % n];
            char b = s[(cut + i + 1) % n];
            if (a == b) eq++;
        }
        if (eq == k) ans++;
    }
    return ans;
}

} // namespace lc4043
