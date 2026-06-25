#pragma once

#include <algorithm>
#include <climits>
#include <vector>

namespace lc4 {

// 双指针 O(m+n)
class SolutionTwoPointers {
public:
    double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
        if (nums1.empty()) {
            int n = static_cast<int>(nums2.size());
            return n % 2 != 0 ? nums2[n / 2]
                              : (static_cast<double>(nums2[n / 2 - 1] + nums2[n / 2]) / 2.0);
        }
        if (nums2.empty()) {
            int m = static_cast<int>(nums1.size());
            return m % 2 != 0 ? nums1[m / 2]
                              : (static_cast<double>(nums1[m / 2 - 1] + nums1[m / 2]) / 2.0);
        }

        int len = static_cast<int>(nums1.size() + nums2.size());
        bool odd = len % 2 != 0;
        int target = 0;
        int lastone = 0;
        int i = 0;
        int j = 0;

        while (i + j < len / 2.0 + 1) {
            lastone = target;
            if (i >= static_cast<int>(nums1.size())) {
                target = nums2[j++];
            } else if (j >= static_cast<int>(nums2.size())) {
                target = nums1[i++];
            } else if (nums1[i] < nums2[j]) {
                target = nums1[i++];
            } else {
                target = nums2[j++];
            }
        }

        return odd ? static_cast<double>(lastone)
                   : (static_cast<double>(target + lastone) / 2.0);
    }
};

// 二分查找 O(log(min(m,n)))
class SolutionBinarySearch {
public:
    double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
        if (nums1.size() > nums2.size()) {
            return findMedianSortedArrays(nums2, nums1);
        }

        int m = static_cast<int>(nums1.size());
        int n = static_cast<int>(nums2.size());
        int left = 0;
        int right = m;

        while (left <= right) {
            int partition1 = (left + right) / 2;
            int partition2 = (m + n + 1) / 2 - partition1;

            int maxLeft1 = (partition1 == 0) ? INT_MIN : nums1[partition1 - 1];
            int minRight1 = (partition1 == m) ? INT_MAX : nums1[partition1];
            int maxLeft2 = (partition2 == 0) ? INT_MIN : nums2[partition2 - 1];
            int minRight2 = (partition2 == n) ? INT_MAX : nums2[partition2];

            if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1) {
                if ((m + n) % 2 == 0) {
                    return (std::max(maxLeft1, maxLeft2) +
                            std::min(minRight1, minRight2)) / 2.0;
                } else {
                    return std::max(maxLeft1, maxLeft2);
                }
            } else if (maxLeft1 > minRight2) {
                right = partition1 - 1;
            } else {
                left = partition1 + 1;
            }
        }
        return 0.0;
    }
};

} // namespace lc4
