#include "../include/router.hpp"
#include<string_view>
#include<unordered_map>

std::vector<std::string> Router :: split(const std::string &topic){

    std::vector<std::string> tokens;
    std::string token;

    for(int i = 0; i <= topic.size(); i++){
        if(i == topic.size() || topic[i] == '.'){
            tokens.push_back(token);
            token.clear();
        }
        else{
            token += topic[i];
        }
    }

    return tokens;
}



void Router :: insert(std::vector<std::string> &topic, int subscriber){
    TrieNode* current = root;
    for(int i = 0; i < topic.size(); i++){
        auto it = current->children.find(topic[i]);

        if(it == current->children.end()){
            current->children[topic[i]] = new TrieNode();
        }

        current=current->children[topic[i]];
    }
    
    current->subscribers.insert(subscriber);
}

void Router :: remove(std::vector<std::string> &topic, int subscriber){
    TrieNode* current = root;
    for(int i = 0; i < topic.size(); i++){
        auto it = current->children.find(topic[i]);

        if(it == current->children.end()){
            return;
        }
        current=current->children[topic[i]];
    }
    current->subscribers.erase(subscriber);
}


void Router ::  dfs(TrieNode* node, int index, std::vector<std::string> &tokens, std::unordered_set<int> &result){
    if(node == nullptr || index >= tokens.size()){
        return;
    }
    auto it = node->children.find(tokens[index]);
    if(it != node->children.end()){
        if(index == tokens.size() - 1){
            result.insert(it->second->subscribers.begin(), it->second->subscribers.end());
        }
        else{
            dfs(it->second, index + 1, tokens, result);
        }
    }
}


void Router :: subscribe(const std::string &topic, int subscriber){
    std::vector<std::string> tokens = split(topic);
    insert(tokens, subscriber);
}
void Router :: unsubscribe(const std::string &topic, int subscriber){
    std::vector<std::string> tokens = split(topic);
    remove(tokens, subscriber);
}

std::unordered_set<int> Router :: publish(const std::string &topic){
    std::vector<std::string> tokens = split(topic);
    std::unordered_set<int> result;
    dfs(root, 0, tokens, result);
    return result;
}