#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace lc2904 {

// 提交的原始版本
class Solution {
public:
    std::string shortestBeautifulSubstring(std::string s, int k) {
        const int n = s.size();
        int i = 0, j = 0;
        int cnt = 0;
        std::map<int, std::vector<std::string>> result;
        int min_len = n;
        while (j < n) {
            if (s[j] == '1') {
                cnt++;
            }
            while (cnt >= k) {
                if (cnt == k && s[i] == '0') {   // 移除前导 0
                    i++;
                    continue;
                }
                if (cnt == k) {
                    result[j - i + 1].push_back(s.substr(i, j - i + 1));
                    min_len = std::min(min_len, j - i + 1);
                }
                if (s[i] == '1') {
                    cnt--;
                }
                i++;
                if (cnt < k) break;
            }
            j++;
        }
        std::sort(result[min_len].begin(), result[min_len].end());
        return result[min_len].empty() ? "" : result[min_len][0];
    }
};

// 参考实现：1 的位置枚举候选窗口
// 最短窗口必然以 1 开头、以 1 结尾（否则可以剥掉端点的 0 变短），
// 所以候选只有 [ones[t], ones[t+k-1]] 这些，逐个比 (长度, 字典序)。
class SolutionRef {
public:
    std::string shortestBeautifulSubstring(std::string s, int k) {
        std::vector<int> ones;
        for (int i = 0; i < (int)s.size(); i++)
            if (s[i] == '1') ones.push_back(i);
        if ((int)ones.size() < k) return "";
        std::string best;
        for (int t = 0; t + k - 1 < (int)ones.size(); t++) {
            std::string cand =
                s.substr(ones[t], ones[t + k - 1] - ones[t] + 1);
            if (best.empty() || cand.size() < best.size() ||
                (cand.size() == best.size() && cand < best)) {
                best = cand;
            }
        }
        return best;
    }
};

} // namespace lc2904
