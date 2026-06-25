#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

TEST_CASE("LC 4 example 1") {
    std::vector<int> nums1 = {1, 3};
    std::vector<int> nums2 = {2};
    CHECK(lc4::SolutionTwoPointers().findMedianSortedArrays(nums1, nums2) == doctest::Approx(2.0));
    CHECK(lc4::SolutionBinarySearch().findMedianSortedArrays(nums1, nums2) == doctest::Approx(2.0));
}

TEST_CASE("LC 4 example 2") {
    std::vector<int> nums1 = {1, 2};
    std::vector<int> nums2 = {3, 4};
    CHECK(lc4::SolutionTwoPointers().findMedianSortedArrays(nums1, nums2) == doctest::Approx(2.5));
    CHECK(lc4::SolutionBinarySearch().findMedianSortedArrays(nums1, nums2) == doctest::Approx(2.5));
}

TEST_CASE("LC 4 one empty") {
    std::vector<int> nums1;
    std::vector<int> nums2 = {1, 2, 3, 4, 5};
    CHECK(lc4::SolutionBinarySearch().findMedianSortedArrays(nums1, nums2) == doctest::Approx(3.0));
}

TEST_CASE("LC 4 all negatives and positives") {
    std::vector<int> nums1 = {-5, 3, 6};
    std::vector<int> nums2 = {-2, 0, 9};
    CHECK(lc4::SolutionTwoPointers().findMedianSortedArrays(nums1, nums2) == doctest::Approx(1.5));
}
