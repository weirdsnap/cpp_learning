#pragma once
#include <algorithm>
#include <utility>
#include <vector>

namespace lc163 {

using std::pair;
using std::vector;

// 用户提交版：哨兵扫描，long long 防溢出
class Solution {
public:
    vector<vector<int>> findMissingRanges(vector<int>& nums, int lower, int upper) {
        vector<vector<int>> result;
        long long start = lower;
        for (int num : nums) {
            if (num > start) {
                result.push_back({(int)start, num - 1});
            }
            start = (long long)num + 1;
        }
        if (start <= upper) {
            result.push_back({(int)start, upper});
        }
        return result;
    }
};

// 性质校验（比对拍更独立：不重新实现算法，验证输出满足定义）：
//  R1 每个区间 a<=b 且落在 [lower, upper] 内
//  R2 区间升序、不相交、不相邻（极大性：相邻区间必须合并）
//  R3 没有任何 nums 元素落在某个区间内
//  R4 区间与 nums 的点合并后恰好无缝铺满 [lower, upper]
inline bool validRanges(const vector<int>& nums, int lower, int upper,
                        const vector<vector<int>>& result) {
    using ll = long long;
    // R1
    for (auto& r : result) {
        if (r.size() != 2 || r[0] > r[1]) return false;
        if ((ll)r[0] < lower || (ll)r[1] > upper) return false;
    }
    // R2
    for (size_t i = 1; i < result.size(); i++) {
        if ((ll)result[i][0] <= (ll)result[i - 1][1] + 1) return false;
    }
    // R3: nums 升序，区间升序，双指针检查无 nums 落在区间内
    {
        size_t k = 0;
        for (int num : nums) {
            while (k < result.size() && (ll)result[k][1] < num) k++;
            if (k < result.size() && (ll)result[k][0] <= num) return false;
        }
    }
    // R4: 把 nums 当点区间，与 result 合并，要求无缝覆盖 [lower, upper]
    {
        vector<pair<ll, ll>> segs;
        for (int num : nums) segs.emplace_back(num, num);
        for (auto& r : result) segs.emplace_back(r[0], r[1]);
        std::sort(segs.begin(), segs.end());
        ll expect = lower;
        for (auto& [a, b] : segs) {
            if (a > expect) return false;      // 有缺口
            expect = std::max(expect, b + 1);  // b+1 不溢出：b <= upper <= 1e9
        }
        if (expect != (ll)upper + 1) return false;  // 末尾没铺满
    }
    return true;
}

} // namespace lc163
