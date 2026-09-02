#pragma once
#include <cstdlib>
#include <string>
#include <unordered_map>

namespace lc1165 {

using std::string;
using std::unordered_map;

// 用户提交版：字符 → 位置的直接定址表
class Solution {
public:
    int calculateTime(string keyboard, string word) {
        int index = 0;
        int count = 0;
        unordered_map<int, int> key_map;  // 遗留声明，未使用
        int key_pos[26];
        for (int i = 0; i < 26; i++) {
            key_pos[keyboard[i] - 'a'] = i;
        }
        for (int i = 0; i < (int)word.size(); i++) {
            count += abs(key_pos[word[i] - 'a'] - index);
            index = key_pos[word[i] - 'a'];
        }
        return count;
    }
};

// 独立对拍：不建表，每次线性扫描 keyboard 找位置
inline int bruteForce(const string& keyboard, const string& word) {
    int index = 0, count = 0;
    for (char c : word) {
        int pos = 0;
        while (keyboard[pos] != c) pos++;  // keyboard 是 26 字母排列，必然找到
        count += abs(pos - index);
        index = pos;
    }
    return count;
}

} // namespace lc1165
