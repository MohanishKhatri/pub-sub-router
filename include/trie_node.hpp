#include<vector>
#include<string>
#include<unordered_map>
#include<unordered_set>

struct TrieNode{
    std::unordered_map<std::string, TrieNode*> children;
    std::unordered_set<int> subscribers;
    bool isHash{false};
};