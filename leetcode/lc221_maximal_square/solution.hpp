#pragma once

#include <algorithm>
#include <vector>

class Solution {
public:
    int maximalSquare(std::vector<std::vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) {
            return 0;
        }

        const int N = static_cast<int>(matrix[0].size());
        int max_side = 0;
        std::vector<int> dp(N, 0);

        for (int i = 0; i < N; ++i) {
            if (matrix[0][i] == '1') {
                dp[i] = 1;
                max_side = 1;
            }
        }

        for (int i = 1; i < static_cast<int>(matrix.size()); ++i) {
            int last = dp[0];
            dp[0] = matrix[i][0] == '1' ? 1 : 0;
            max_side = std::max(max_side, dp[0]);

            for (int j = 1; j < N; ++j) {
                if (matrix[i][j] == '0') {
                    last = dp[j];
                    dp[j] = 0;
                    continue;
                }
                int temp = std::min({dp[j], dp[j - 1], last}) + 1;
                last = dp[j];
                dp[j] = temp;
                max_side = std::max(max_side, dp[j]);
            }
        }
        return max_side * max_side;
    }
};
