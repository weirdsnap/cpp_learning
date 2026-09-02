#pragma once
#include <algorithm>
#include <vector>

namespace lc252 {

using std::vector;

// 用户提交版：按起点排序后检查相邻区间是否重叠
class Solution {
public:
    bool canAttendMeetings(vector<vector<int>>& intervals) {
        if (intervals.empty()) {
            return true;
        }
        std::sort(intervals.begin(), intervals.end(),
                  [](const vector<int>& a, const vector<int>& b) {
                      return a[0] < b[0];
                  });
        for (int i = 1; i < (int)intervals.size(); i++) {
            if (intervals[i][0] < intervals[i - 1][1]) {
                return false;
            }
        }
        return true;
    }
};

// 独立对拍：O(n²) 两两判重叠。端点相接不算重叠（半开语义）。
inline bool bruteForce(const vector<vector<int>>& intervals) {
    int n = intervals.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int a = intervals[i][0], b = intervals[i][1];
            int c = intervals[j][0], d = intervals[j][1];
            if (a < d && c < b) return false;  // 标准区间相交判定
        }
    }
    return true;
}

} // namespace lc252
