#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "solution.hpp"

#include <vector>

namespace {

lc2095::ListNode* build(const std::vector<int>& vals) {
    lc2095::ListNode dummy;
    lc2095::ListNode* cur = &dummy;
    for (int v : vals) {
        cur->next = new lc2095::ListNode(v);
        cur = cur->next;
    }
    return dummy.next;
}

std::vector<int> to_vector(lc2095::ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

void free_list(lc2095::ListNode* head) {
    while (head != nullptr) {
        lc2095::ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

} // namespace

TEST_CASE("LC 2095 example 1") {
    lc2095::ListNode* head = build({1, 3, 4, 7, 1, 2, 6});
    lc2095::ListNode* result = lc2095::SolutionDummy().deleteMiddle(head);
    CHECK(to_vector(result) == std::vector<int>{1, 3, 4, 1, 2, 6});
    free_list(result);
}

TEST_CASE("LC 2095 example 2") {
    lc2095::ListNode* head = build({1, 2, 3, 4});
    lc2095::ListNode* result = lc2095::SolutionPrePointer().deleteMiddle(head);
    CHECK(to_vector(result) == std::vector<int>{1, 2, 4});
    free_list(result);
}

TEST_CASE("LC 2095 single node") {
    lc2095::ListNode* head = build({1});
    lc2095::ListNode* result = lc2095::SolutionDummy().deleteMiddle(head);
    CHECK(to_vector(result).empty());
    free_list(result);
}

TEST_CASE("LC 2095 two nodes") {
    lc2095::ListNode* head = build({1, 2});
    lc2095::ListNode* result = lc2095::SolutionPrePointer().deleteMiddle(head);
    CHECK(to_vector(result) == std::vector<int>{1});
    free_list(result);
}
