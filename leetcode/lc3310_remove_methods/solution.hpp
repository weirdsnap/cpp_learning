// LC 3310. 移除可疑方法 — 博客 leetcode/graph/ch45.md
#pragma once

#include <numeric>
#include <queue>
#include <vector>

namespace lc3310 {

// 解法一：BFS + 出队时标记（节点可能重复入队，有额外开销）
class SolutionV1 {
public:
    std::vector<int> remainingMethods(int n, int k,
                                      std::vector<std::vector<int>>& invocations) {
        std::vector<std::vector<int>> net(n);
        for (const auto& invocation : invocations) {
            net[invocation[0]].push_back(invocation[1]);
        }

        std::vector<bool> visted(n, false);
        std::queue<int> q;
        q.push(k);
        while (!q.empty()) {
            auto cur = q.front();
            q.pop();
            visted[cur] = true;
            for (const auto& next : net[cur]) {
                if (!visted[next]) {
                    q.push(next);
                }
            }
        }

        for (const auto& invocation : invocations) {
            if (visted[invocation[1]] && !visted[invocation[0]]) {
                std::vector<int> all(n);
                std::iota(all.begin(), all.end(), 0);
                return all;
            }
        }

        std::vector<int> result;
        for (int i = 0; i < n; i++) {
            if (!visted[i]) {
                result.push_back(i);
            }
        }
        return result;
    }
};

// 解法二：BFS + 入队时标记（标准写法，每节点恰好入队一次）
class Solution {
public:
    std::vector<int> remainingMethods(int n, int k,
                                      std::vector<std::vector<int>>& invocations) {
        std::vector<std::vector<int>> net(n);
        for (const auto& invocation : invocations) {
            net[invocation[0]].push_back(invocation[1]);
        }

        std::vector<bool> visited(n, false);
        std::queue<int> q;
        q.push(k);
        visited[k] = true;
        while (!q.empty()) {
            auto cur = q.front();
            q.pop();
            for (const auto& next : net[cur]) {
                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }

        for (const auto& invocation : invocations) {
            if (visited[invocation[1]] && !visited[invocation[0]]) {
                std::vector<int> all(n);
                std::iota(all.begin(), all.end(), 0);
                return all;
            }
        }

        std::vector<int> result;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                result.push_back(i);
            }
        }
        return result;
    }
};

} // namespace lc3310
