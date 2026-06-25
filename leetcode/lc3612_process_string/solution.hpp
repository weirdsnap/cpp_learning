#pragma once

#include <algorithm>
#include <deque>
#include <string>

namespace lc3612 {

// 直接模拟
class SolutionSimulation {
public:
    std::string processStr(std::string s) {
        std::string result;
        for (char c : s) {
            if (c == '*') {
                if (!result.empty()) {
                    result.pop_back();
                }
            } else if (c == '#') {
                result += result;
            } else if (c == '%') {
                std::reverse(result.begin(), result.end());
            } else {
                result.push_back(c);
            }
        }
        return result;
    }
};

// 逻辑反转标志 + deque
class SolutionDeque {
public:
    std::string processStr(std::string s) {
        std::deque<char> dq;
        bool reversed = false;

        for (char c : s) {
            if (c == '*') {
                if (!dq.empty()) {
                    if (reversed) dq.pop_front();
                    else dq.pop_back();
                }
            } else if (c == '#') {
                std::deque<char> copy = dq;
                if (reversed) {
                    dq.insert(dq.end(), copy.rbegin(), copy.rend());
                } else {
                    dq.insert(dq.end(), copy.begin(), copy.end());
                }
            } else if (c == '%') {
                reversed = !reversed;
            } else {
                if (reversed) dq.push_front(c);
                else dq.push_back(c);
            }
        }

        std::string result;
        if (reversed) {
            result.assign(dq.rbegin(), dq.rend());
        } else {
            result.assign(dq.begin(), dq.end());
        }
        return result;
    }
};

} // namespace lc3612
