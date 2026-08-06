// LC 3345. 最小整除数位积 I — 博客 leetcode/greedy/ch47.md
#pragma once

namespace lc3345 {

// 枚举版：从 n 开始逐个检查数位积能否被 t 整除
// 上界 n+10 足够：任意 10 个连续整数里必有一个以 0 结尾，数位积为 0，0 % t == 0
class Solution {
public:
    int smallestNumber(int n, int t) {
        for (int x = n; x < n + 10; x++) {
            if (digitProduct(x) % t == 0) return x;
        }
        return -1;  // 不会执行到这里
    }

private:
    int digitProduct(int x) {
        int p = 1;
        while (x != 0) {
            p *= x % 10;
            x /= 10;
        }
        return p;
    }
};

// 错误示范：把数位积写成数位和（i += j % 10），对拍会暴露
class SolutionSum {
public:
    int smallestNumber(int n, int t) {
        for (int x = n; x <= n + 100; x++) {
            if (digitSum(x) % t == 0) return x;
        }
        return -1;
    }

private:
    int digitSum(int x) {
        int s = 0;
        while (x != 0) {
            s += x % 10;
            x /= 10;
        }
        return s;
    }
};

} // namespace lc3345
