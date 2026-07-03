#include<string>
#include<vector>
#include<unordered_set>
#include<trie_node.hpp>

class Router{
    public:
        Router(){
            root = new TrieNode();
        }
        void subscribe(const std::string &topic, int subscriber);
        void unsubscribe(const std::string &topic, int subscriber);
        std::unordered_set<int> publish(const std::string &topic);
    
    private:
        TrieNode* root;
        std::vector<std::string> split(const std::string &topic);
        void insert(const std::string &topic, int subscriber);
        void remove(const std::string &topic, int subscriber);
        void dfs(TrieNode* node, int index, std::vector<std::string> &tokens, std::unordered_set<int> &result);
};