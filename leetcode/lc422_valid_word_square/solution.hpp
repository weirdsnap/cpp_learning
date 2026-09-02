#pragma once
#include <string>
#include <vector>

namespace lc422 {

using std::string;
using std::vector;

// 用户提交版：n×n 对称比较 + 行长超 n 提前判负，缺失侧用 '\0' 哨兵
class Solution {
public:
    bool validWordSquare(vector<string>& words) {
        int n = words.size();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (words[i].size() > n) {
                    return false;
                }
                char a = (j < (int)words[i].size()) ? words[i][j] : '\0';
                char b = (i < (int)words[j].size()) ? words[j][i] : '\0';
                if (a != b) {
                    return false;
                }
            }
        }
        return true;
    }
};

// 独立对拍：严格按题面定义——k 取 0..max(行数,最长行长)-1，
// 第 k 行是 words[k]（k 超界视为空串），
// 第 k 列是所有"长度 > k 的行"的第 k 个字符顺次拼接，逐 k 比较。
inline bool bruteForce(const vector<string>& words) {
    int n = words.size();
    int numCols = 0;
    for (auto& w : words) numCols = std::max(numCols, (int)w.size());
    int lim = std::max(n, numCols);
    for (int k = 0; k < lim; k++) {
        string row = (k < n) ? words[k] : "";
        string col;
        for (int m = 0; m < n; m++) {
            if (k < (int)words[m].size()) col += words[m][k];
        }
        if (row != col) return false;
    }
    return true;
}

} // namespace lc422
