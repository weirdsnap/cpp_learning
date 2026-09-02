#pragma once
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace lc734 {

using std::set;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// 用户版一：unordered_map + unordered_set，O(1) 均摊查找
class Solution {
public:
    bool areSentencesSimilar(vector<string>& sentence1, vector<string>& sentence2,
                             vector<vector<string>>& similarPairs) {
        if (sentence1.size() != sentence2.size()) return false;
        unordered_map<string, unordered_set<string>> similarMap;
        for (const auto& pair : similarPairs) {
            similarMap[pair[0]].insert(pair[1]);
            similarMap[pair[1]].insert(pair[0]);
        }
        for (int i = 0; i < (int)sentence1.size(); i++) {
            if (sentence1[i] == sentence2[i]) continue;
            if (similarMap[sentence1[i]].count(sentence2[i]) == 0) return false;
        }
        return true;
    }
};

// 用户版二：unordered_map + set（有序），O(log n) 查找
class SolutionOrdered {
public:
    bool areSentencesSimilar(vector<string>& sentence1, vector<string>& sentence2,
                             vector<vector<string>>& similarPairs) {
        if (sentence1.size() != sentence2.size()) return false;
        unordered_map<string, set<string>> similarMap;
        for (const auto& pair : similarPairs) {
            similarMap[pair[0]].insert(pair[1]);
            similarMap[pair[1]].insert(pair[0]);
        }
        for (int i = 0; i < (int)sentence1.size(); i++) {
            if (sentence1[i] == sentence2[i]) continue;
            if (similarMap[sentence1[i]].count(sentence2[i]) == 0) return false;
        }
        return true;
    }
};

// 独立对拍：不建任何索引，直接扫描 similarPairs 原始数组逐对确认
inline bool bruteForce(const vector<string>& sentence1, const vector<string>& sentence2,
                       const vector<vector<string>>& similarPairs) {
    if (sentence1.size() != sentence2.size()) return false;
    for (size_t i = 0; i < sentence1.size(); i++) {
        if (sentence1[i] == sentence2[i]) continue;
        bool ok = false;
        for (const auto& p : similarPairs) {
            if ((p[0] == sentence1[i] && p[1] == sentence2[i]) ||
                (p[1] == sentence1[i] && p[0] == sentence2[i])) {
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }
    return true;
}

} // namespace lc734
