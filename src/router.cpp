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
    if(node == nullptr){
        return;
    }
    if(index >= tokens.size()){
        for(const auto &subscriber : node->subscribers){
            result.insert(subscriber);
        }
        return;
    }

    auto exact_match_iterator = node->children.find(tokens[index]);
    if(exact_match_iterator != node->children.end()){
        dfs(exact_match_iterator->second, index + 1, tokens, result);
    }
    auto wildcard_match_iterator = node->children.find("*");
    if(wildcard_match_iterator != node->children.end()){
        dfs(wildcard_match_iterator->second, index + 1, tokens, result);
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