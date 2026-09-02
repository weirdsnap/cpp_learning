#pragma once
#include <cstring>
#include <string>
#include <vector>

namespace lc616 {

using std::string;
using std::vector;

// ---------- 用户提交版：Trie + 区间标记 ----------
struct TrieNode {
    TrieNode* children[128];
    bool isEnd;
    TrieNode() {
        memset(children, 0, sizeof(children));
        isEnd = false;
    }
};

class Trie {
public:
    TrieNode* root;
    Trie() { root = new TrieNode(); }

    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children[c]) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEnd = true;
    }
};

class Solution {
public:
    string addBoldTag(string s, vector<string>& words) {
        Trie trie;
        for (const string& word : words) {
            trie.insert(word);
        }

        int n = s.size();
        vector<bool> bold(n, false);

        for (int i = 0; i < n; ++i) {
            TrieNode* node = trie.root;
            int end = 0;
            for (int j = i; j < n; ++j) {
                node = node->children[s[j]];
                if (!node) break;
                if (node->isEnd) end = j + 1;
            }
            for (int k = i; k < end; ++k) {
                bold[k] = true;
            }
        }

        string res;
        for (int i = 0; i < n; ++i) {
            if (bold[i] && (i == 0 || !bold[i - 1])) {
                res += "<b>";
            }
            res += s[i];
            if (bold[i] && (i == n - 1 || !bold[i + 1])) {
                res += "</b>";
            }
        }
        return res;
    }
};

// ---------- 独立对拍：不用 Trie，逐单词朴素查找所有出现位置 ----------
inline string bruteForce(const string& s, const vector<string>& words) {
    int n = s.size();
    vector<char> bold(n, 0);
    for (const auto& w : words) {
        if (w.empty() || (int)w.size() > n) continue;
        for (int i = 0; i + (int)w.size() <= n; i++) {
            if (s.compare(i, w.size(), w) == 0) {
                for (int k = i; k < i + (int)w.size(); k++) bold[k] = 1;
            }
        }
    }
    // 用状态机拼接：跟踪"当前是否在加粗段内"
    string res;
    bool in = false;
    for (int i = 0; i < n; i++) {
        if (bold[i] && !in) { res += "<b>"; in = true; }
        if (!bold[i] && in) { res += "</b>"; in = false; }
        res += s[i];
    }
    if (in) res += "</b>";
    return res;
}

} // namespace lc616
