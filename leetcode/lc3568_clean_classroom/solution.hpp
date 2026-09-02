#pragma once
#include <queue>
#include <string>
#include <vector>

namespace lc3568 {

using std::queue;
using std::string;
using std::vector;

// 用户提交版：BFS + bestEnergy[x][y][mask] 支配剪枝（只保留更高能量的同位同掩码状态）
class Solution {
public:
    int minMoves(vector<string>& classroom, int energy) {
        int m = classroom.size(), n = classroom[0].size();

        vector<vector<int>> id(m, vector<int>(n, 0));
        int sx = 0, sy = 0, cnt = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (classroom[i][j] == 'S') {
                    sx = i;
                    sy = j;
                } else if (classroom[i][j] == 'L') {
                    id[i][j] = 1 << cnt;
                    ++cnt;
                }
            }
        }

        int full = 1 << cnt;

        vector<vector<vector<int>>> bestEnergy(
            m, vector<vector<int>>(n, vector<int>(full, -1)));
        bestEnergy[sx][sy][0] = energy;

        struct State {
            int x, y, mask, e, steps;
        };

        queue<State> q;
        q.push({sx, sy, 0, energy, 0});

        const int dx[4] = {0, 1, 0, -1};
        const int dy[4] = {1, 0, -1, 0};

        while (!q.empty()) {
            State t = q.front();
            q.pop();

            if (t.mask == full - 1) {
                return t.steps;
            }

            if (t.e == 0) {
                continue;
            }

            for (int k = 0; k < 4; ++k) {
                int nx = t.x + dx[k];
                int ny = t.y + dy[k];

                if (nx < 0 || nx >= m || ny < 0 || ny >= n ||
                    classroom[nx][ny] == 'X') {
                    continue;
                }

                int ne = (classroom[nx][ny] == 'R') ? energy : t.e - 1;
                int nmask = t.mask | id[nx][ny];

                if (ne > bestEnergy[nx][ny][nmask]) {
                    bestEnergy[nx][ny][nmask] = ne;
                    q.push({nx, ny, nmask, ne, t.steps + 1});
                }
            }
        }

        return -1;
    }
};

// 独立对拍：完整状态 (x, y, mask, e) 的朴素 BFS，visited 精确去重，
// 不做任何支配剪枝。状态数 m*n*2^cnt*(energy+1)，仅用于小网格。
inline int bruteForce(const vector<string>& classroom, int energy) {
    int m = classroom.size(), n = classroom[0].size();
    vector<vector<int>> id(m, vector<int>(n, 0));
    int sx = 0, sy = 0, cnt = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) {
            if (classroom[i][j] == 'S') { sx = i; sy = j; }
            else if (classroom[i][j] == 'L') { id[i][j] = 1 << cnt; ++cnt; }
        }
    int full = (1 << cnt) - 1;

    // visited[x][y][mask][e]
    vector<vector<vector<vector<char>>>> vis(
        m, vector<vector<vector<char>>>(
               n, vector<vector<char>>(1 << cnt, vector<char>(energy + 1, 0))));

    struct State { int x, y, mask, e, steps; };
    queue<State> q;
    q.push({sx, sy, 0, energy, 0});
    vis[sx][sy][0][energy] = 1;

    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {1, 0, -1, 0};

    while (!q.empty()) {
        State t = q.front();
        q.pop();
        if (t.mask == full) return t.steps;
        if (t.e == 0) continue;  // 能量 0 且不在 R 上（在 R 上时入队即回满，不会出现 e=0）
        for (int k = 0; k < 4; ++k) {
            int nx = t.x + dx[k], ny = t.y + dy[k];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n ||
                classroom[nx][ny] == 'X')
                continue;
            int ne = (classroom[nx][ny] == 'R') ? energy : t.e - 1;
            int nmask = t.mask | id[nx][ny];
            if (!vis[nx][ny][nmask][ne]) {
                vis[nx][ny][nmask][ne] = 1;
                q.push({nx, ny, nmask, ne, t.steps + 1});
            }
        }
    }
    return -1;
}

} // namespace lc3568
