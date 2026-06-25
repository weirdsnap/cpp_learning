#pragma once

#include <queue>
#include <vector>

namespace lc3558 {

class Solution {
public:
    int assignEdgeWeights(std::vector<std::vector<int>>& edges) {
        constexpr int MOD = 1'000'000'007;
        const int n = static_cast<int>(edges.size()) + 1;
        if (n == 1) return 0;

        std::vector<std::vector<int>> adj(n + 1);
        for (const auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        std::vector<bool> visited(n + 1, false);
        std::queue<int> q;
        q.push(1);
        visited[1] = true;
        int max_depth = -1;

        while (!q.empty()) {
            int size = static_cast<int>(q.size());
            ++max_depth;
            while (size--) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }

        if (max_depth == 0) return 0;

        long long ans = 1;
        long long base = 2;
        int exp = max_depth - 1;
        while (exp > 0) {
            if (exp & 1) ans = (ans * base) % MOD;
            base = (base * base) % MOD;
            exp >>= 1;
        }
        return static_cast<int>(ans);
    }
};

} // namespace lc3558
