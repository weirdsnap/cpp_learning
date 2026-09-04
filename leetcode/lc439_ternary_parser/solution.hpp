#pragma once
#include <string>
#include <vector>

namespace lc439 {

using std::string;
using std::vector;

// 用户提交版：从右往左扫描 + 栈，遇到 '?' 弹出两个操作数按条件折叠
class Solution {
public:
    string parseTernary(string expression) {
        const int n = expression.size();
        vector<char> stack;
        stack.reserve(n);
        for (int i = n - 1; i >= 0; i--) {
            char c = expression[i];
            if (c == '?') {
                char cond = expression[i - 1];
                char a = stack.back(); stack.pop_back();
                char b = stack.back(); stack.pop_back();
                stack.push_back(cond == 'T' ? a : b);
                i--;
            } else if (c != ':') {
                stack.push_back(c);
            }
        }
        return string(1, stack.back());
    }
};

// 独立对拍：递归下降语法解析（从左往右），expr := atom | atom '?' expr ':' expr
inline string recursiveParse(const string& s, int& i) {
    char atom = s[i++];  // 原子：T/F/数字
    if (i < (int)s.size() && s[i] == '?') {
        i++;  // 跳过 '?'
        string t = recursiveParse(s, i);
        i++;  // 跳过 ':'
        string f = recursiveParse(s, i);
        return atom == 'T' ? t : f;
    }
    return string(1, atom);
}

inline string oracle(const string& s) {
    int i = 0;
    return recursiveParse(s, i);
}

} // namespace lc439
