#pragma once
#include <algorithm>
#include <map>
#include <vector>

namespace lc253 {

using std::map;
using std::vector;

// 用户提交版：差分（map 扫描线），起点 +1 终点 -1，峰值即答案
class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
        map<int, int> cnt;
        for (const auto& interval : intervals) {
            cnt[interval[0]]++;
            cnt[interval[1]]--;
        }
        int maxRooms = 0, curRooms = 0;
        for (const auto& [point, value] : cnt) {
            curRooms += value;
            maxRooms = std::max(maxRooms, curRooms);
        }
        return maxRooms;
    }
};

// 独立对拍：峰值必出现在某个起点时刻 s（半开 [a,b)，s 时刻进行中的会议
// 满足 a <= s < b）。O(n²) 直接统计每个起点处的并发数取最大。
inline int bruteForce(const vector<vector<int>>& intervals) {
    int best = 0;
    for (const auto& iv : intervals) {
        int s = iv[0];
        int cur = 0;
        for (const auto& x : intervals) {
            if (x[0] <= s && s < x[1]) cur++;
        }
        best = std::max(best, cur);
    }
    return best;  // 空数组时 0
}

} // namespace lc253
