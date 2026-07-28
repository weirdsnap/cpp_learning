// LC 3517. 最小回文重排 I — 博客 leetcode/string/ch33.md
#pragma once

#include <map>
#include <string>

namespace lc3517 {

// 解法一：map 计数（按键有序，遍历即升序）
class SolutionMap {
public:
    std::string smallestPalindrome(std::string s) {
        std::map<char, int> cnt;
        for (const auto& c : s) cnt[c]++;
        std::string mid;
        std::string result;
        result.reserve(s.size() / 2);
        for (auto& [k, v] : cnt) {
            if (v % 2 == 1) {
                mid += k;
                v--;
            }
            result.append(v / 2, k);
        }
        return result + mid + std::string(result.rbegin(), result.rend());
    }
};

// 解法二：数组计数（字符集固定 26，常数更小）
class SolutionArray {
public:
    std::string smallestPalindrome(std::string s) {
        int cnt[26] = {0};
        for (const auto& c : s) cnt[c - 'a']++;
        std::string mid;
        std::string result;
        result.reserve(s.size() / 2);
        for (int i = 0; i < 26; i++) {
            if (cnt[i] % 2 == 1) {
                mid += char('a' + i);
                cnt[i]--;
            }
            result.append(cnt[i] / 2, char('a' + i));
        }
        return result + mid + std::string(result.rbegin(), result.rend());
    }
};

} // namespace lc3517
