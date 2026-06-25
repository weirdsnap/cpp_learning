#pragma once

#include <algorithm>
#include <vector>

namespace lc42 {

// 解法一：预处理左右最大高度
class SolutionDP {
public:
    int trap(std::vector<int>& height) {
        const int n = static_cast<int>(height.size());
        if (n == 0) return 0;

        std::vector<int> left_max(n);
        std::vector<int> right_max(n);
        left_max[0] = height[0];
        for (int i = 1; i < n; ++i) {
            left_max[i] = std::max(left_max[i - 1], height[i]);
        }
        right_max[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            right_max[i] = std::max(right_max[i + 1], height[i]);
        }

        int result = 0;
        for (int i = 0; i < n; ++i) {
            int water = std::min(left_max[i], right_max[i]) - height[i];
            if (water > 0) result += water;
        }
        return result;
    }
};

// 解法二：双指针
class SolutionTwoPointers {
public:
    int trap(std::vector<int>& height) {
        const int n = static_cast<int>(height.size());
        if (n == 0) return 0;

        int left = 0;
        int right = n - 1;
        int left_max = 0;
        int right_max = 0;
        int result = 0;

        while (left < right) {
            if (height[left] < height[right]) {
                if (height[left] >= left_max) {
                    left_max = height[left];
                } else {
                    result += left_max - height[left];
                }
                ++left;
            } else {
                if (height[right] >= right_max) {
                    right_max = height[right];
                } else {
                    result += right_max - height[right];
                }
                --right;
            }
        }
        return result;
    }
};

// 解法三：单调递减栈
class SolutionMonotonicStack {
public:
    int trap(std::vector<int>& height) {
        const int n = static_cast<int>(height.size());
        if (n == 0) return 0;

        std::vector<int> st;
        int result = 0;

        for (int i = 0; i < n; ++i) {
            while (!st.empty() && height[i] >= height[st.back()]) {
                int bottom = height[st.back()];
                st.pop_back();
                if (st.empty()) break;
                int left = height[st.back()];
                int width = i - st.back() - 1;
                result += (std::min(left, height[i]) - bottom) * width;
            }
            st.push_back(i);
        }
        return result;
    }
};

} // namespace lc42
