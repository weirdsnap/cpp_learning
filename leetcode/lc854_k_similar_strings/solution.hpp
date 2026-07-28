// LC 854. 相似度为 K 的字符串 — 博客 leetcode/graph/ch35.md
#pragma once

#include <algorithm>
#include <functional>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace lc854 {

// 解法一：剪枝 BFS（三条剪枝：只修最左错位 / j 能修对 i / j 不破坏已正确位置）
class SolutionBFS {
public:
    int kSimilarity(std::string s1, std::string s2) {
        if (s1 == s2) return 0;
        std::queue<std::pair<std::string, int>> q;
        std::unordered_set<std::string> vis;
        q.push({s1, 0});
        vis.insert(s1);
        while (!q.empty()) {
            auto [cur, step] = q.front();
            q.pop();
            int i = 0;
            while (i < (int)cur.size() && cur[i] == s2[i]) i++;
            for (int j = i + 1; j < (int)cur.size(); j++) {
                if (cur[j] != s2[i] || cur[j] == s2[j]) continue;
                std::swap(cur[i], cur[j]);
                if (cur == s2) return step + 1;
                if (vis.count(cur) == 0) {
                    vis.insert(cur);
                    q.push({cur, step + 1});
                }
                std::swap(cur[i], cur[j]);
            }
        }
        return -1;
    }
};

// 解法二：A*，h = 不匹配位置数 / 2 上取整
class SolutionAStar {
public:
    int kSimilarity(std::string s1, std::string s2) {
        std::priority_queue<
            std::pair<int, std::string>,
            std::vector<std::pair<int, std::string>>,
            std::greater<std::pair<int, std::string>>> pq;
        std::unordered_map<std::string, int> dist;
        dist[s1] = 0;
        pq.push({h(s1, s2), s1});
        while (!pq.empty()) {
            auto top = pq.top();
            pq.pop();
            std::string s = top.second;
            int g = dist[s];
            if (s == s2) return g;
            int i = 0;
            while (i < (int)s.size() && s[i] == s2[i]) i++;
            for (int j = i + 1; j < (int)s.size(); j++) {
                if (s[j] != s2[i] || s[j] == s2[j]) continue;
                std::string next_s = s;
                std::swap(next_s[i], next_s[j]);
                int new_g = g + 1;
                if (!dist.count(next_s) || new_g < dist[next_s]) {
                    dist[next_s] = new_g;
                    pq.push({new_g + h(next_s, s2), next_s});
                }
            }
        }
        return -1;
    }

private:
    int h(std::string& s, std::string& target) {
        int diff = 0;
        for (int i = 0; i < (int)s.size(); i++)
            if (s[i] != target[i]) diff++;
        return (diff + 1) / 2;
    }
};

// 解法三：IDA*（迭代加深，内存只占递归栈）
class SolutionIDAStar {
public:
    int kSimilarity(std::string s1, std::string s2) {
        target = s2;
        n = s1.size();
        int bound = h(s1);
        while (true) {
            int t = dfs(s1, 0, bound, 0);
            if (t == -1) return bound;
            if (t == INF) return -1;
            bound = t;
        }
    }

private:
    std::string target;
    int n = 0;
    const int INF = 1e9;

    // 返回：-1 找到解，其他值为下一轮的最小 bound
    int dfs(std::string& s, int g, int bound, int pos) {
        int f = g + h(s);
        if (f > bound) return f;
        if (s == target) return -1;
        int minNext = INF;
        int i = pos;
        while (i < n && s[i] == target[i]) i++;
        for (int j = i + 1; j < n; j++) {
            if (s[j] != target[i] || s[j] == target[j]) continue;
            if (s[i] == target[j]) {  // 双向匹配：一换修两个位置
                std::swap(s[i], s[j]);
                int t = dfs(s, g + 1, bound, i + 1);
                std::swap(s[i], s[j]);
                if (t == -1) return -1;
                minNext = std::min(minNext, t);
                break;
            }
        }
        if (minNext == INF) {  // 单向匹配
            for (int j = i + 1; j < n; j++) {
                if (s[j] != target[i] || s[j] == target[j]) continue;
                std::swap(s[i], s[j]);
                int t = dfs(s, g + 1, bound, i + 1);
                std::swap(s[i], s[j]);
                if (t == -1) return -1;
                minNext = std::min(minNext, t);
            }
        }
        return minNext;
    }

    int h(std::string& s) {
        int diff = 0;
        for (int i = 0; i < n; i++)
            if (s[i] != target[i]) diff++;
        return (diff + 1) / 2;
    }
};

} // namespace lc854
