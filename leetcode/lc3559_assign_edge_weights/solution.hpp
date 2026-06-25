#pragma once

#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>

namespace lc3559 {

class Solution {
public:
    std::vector<int> assignEdgeWeights(std::vector<std::vector<int>>& edges,
                                       std::vector<std::vector<int>>& queries) {
        constexpr int MOD = 1'000'000'007;
        const int n = static_cast<int>(edges.size()) + 1;
        const int LOG = std::max(1, static_cast<int>(std::ceil(std::log2(n + 1))));

        std::vector<std::vector<int>> tree(n + 1);
        for (const auto& e : edges) {
            tree[e[0]].push_back(e[1]);
            tree[e[1]].push_back(e[0]);
        }

        std::vector<int> depth(n + 1, -1);
        std::vector<std::vector<int>> parent(LOG, std::vector<int>(n + 1, -1));
        std::queue<int> q;
        q.push(1);
        depth[1] = 0;

        while (!q.empty()) {
            int cur = q.front(); q.pop();
            for (int node : tree[cur]) {
                if (depth[node] == -1) {
                    depth[node] = depth[cur] + 1;
                    parent[0][node] = cur;
                    q.push(node);
                }
            }
        }

        for (int j = 1; j < LOG; ++j) {
            for (int k = 1; k <= n; ++k) {
                if (parent[j - 1][k] != -1) {
                    parent[j][k] = parent[j - 1][parent[j - 1][k]];
                }
            }
        }

        std::vector<long long> pow2(n + 1);
        pow2[0] = 1;
        for (int i = 1; i <= n; ++i) {
            pow2[i] = (pow2[i - 1] * 2) % MOD;
        }

        auto lca = [&](int u, int v) {
            if (depth[u] < depth[v]) std::swap(u, v);
            int diff = depth[u] - depth[v];
            for (int j = 0; j < LOG; ++j) {
                if (diff & (1 << j)) {
                    u = parent[j][u];
                }
            }
            if (u == v) return u;
            for (int j = LOG - 1; j >= 0; --j) {
                if (parent[j][u] != parent[j][v]) {
                    u = parent[j][u];
                    v = parent[j][v];
                }
            }
            return parent[0][u];
        };

        std::vector<int> result;
        result.reserve(queries.size());
        for (const auto& query : queries) {
            int u = query[0];
            int v = query[1];
            if (u == v) {
                result.push_back(0);
                continue;
            }
            int w = lca(u, v);
            int path_edges = depth[u] + depth[v] - 2 * depth[w];
            result.push_back(static_cast<int>(pow2[path_edges - 1]));
        }
        return result;
    }
};

} // namespace lc3559
