#pragma once

#include <string>
#include <vector>

namespace lc14 {

class Solution {
public:
    std::string longestCommonPrefix(std::vector<std::string>& strs) {
        if (strs.empty()) return "";

        std::string result;
        int i = 0;
        while (i < static_cast<int>(strs[0].size())) {
            char current_char = strs[0][i];
            for (int j = 1; j < static_cast<int>(strs.size()); ++j) {
                if (i >= static_cast<int>(strs[j].size()) || strs[j][i] != current_char) {
                    return result;
                }
            }
            result.push_back(current_char);
            ++i;
        }
        return result;
    }
};

} // namespace lc14
