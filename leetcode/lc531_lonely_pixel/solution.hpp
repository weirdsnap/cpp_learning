#pragma once
#include <vector>

namespace lc531 {

using std::vector;

// 用户提交版：两遍扫描，先统计行列 B 数，再判定
class Solution {
public:
    int findLonelyPixel(vector<vector<char>>& picture) {
        const int n = picture.size();
        const int m = picture[0].size();
        vector<int> row_cnt(n, 0);
        vector<int> col_cnt(m, 0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (picture[i][j] == 'B') {
                    row_cnt[i]++;
                    col_cnt[j]++;
                }
            }
        }
        int count = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (picture[i][j] == 'B' && row_cnt[i] == 1 && col_cnt[j] == 1) {
                    count++;
                }
            }
        }
        return count;
    }
};

// 独立对拍：对每个 B 格，直接扫描其所在行和列，确认无其他 B
inline int bruteForce(const vector<vector<char>>& picture) {
    const int n = picture.size();
    const int m = picture[0].size();
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (picture[i][j] != 'B') continue;
            bool lonely = true;
            for (int k = 0; k < m && lonely; k++)
                if (k != j && picture[i][k] == 'B') lonely = false;
            for (int k = 0; k < n && lonely; k++)
                if (k != i && picture[k][j] == 'B') lonely = false;
            if (lonely) count++;
        }
    }
    return count;
}

} // namespace lc531
