#include "../include/router.hpp"
#include<string_view>

/*
A helper function which takes topic string and splits in tokens based on '.' delimiter
sports.football.ipl => ["sports", "football", "ipl"]
*/
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

/*
Inserts a subscriber's subscription into trie
if there is no node for a token we create one
*/
void Router :: insert(std::vector<std::string> &topic, int subscriber){
    TrieNode* current = root;
    for(int i = 0; i < topic.size(); i++){
        auto it = current->children.find(topic[i]);

        if(it == current->children.end()){
            current->children[topic[i]] = new TrieNode();
        }
        
        current=current->children[topic[i]];
        if(topic[i] == "#"){
            current->isHash = true;
        }
    }
    
    current->subscribers.insert(subscriber);
}

/*
Remove a node from trie if some one unsubscribes
!TODO : implement recursive removal of nodes with memory cleanup
*/
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


/*
Main brain of router it does dfs ie when someone publishes we traverse and deliver it to respective subscribers.
normal node traversal along with '*' which is a substitute for exactly one node/level
'#' on other hand can be substitute for zero or more than 1 multiple nodes
*/
void Router ::  dfs(TrieNode* node, int index, std::vector<std::string> &tokens, std::unordered_set<int> &result){
    if(node == nullptr){
        return;
    }

    if(index == tokens.size()){
        for(const auto &subscriber : node->subscribers){
            result.insert(subscriber);
        }

        // this part takes care of case where topic is short like sports and we have a subscription like sports.# then that subscriber should get the message as well
        // also cases where we skip some levels due to isHash thing so if we reach end process them 
        auto multi_level_wildcard_iterator = node->children.find("#");
        if(multi_level_wildcard_iterator != node->children.end()){
            for(const auto &subscriber : multi_level_wildcard_iterator->second->subscribers){
                result.insert(subscriber);
            }
        }
        return;
    }

    // skipping some levels 
    if(node->isHash){
        dfs(node, index + 1, tokens, result); 
    }

    // this also handles the case where we skip some nodes due to hash and then a acutal node matches so we come out of that recursive thing
    auto exact_match_iterator = node->children.find(tokens[index]);
    if(exact_match_iterator != node->children.end()){
        dfs(exact_match_iterator->second, index + 1, tokens, result);
    }

    auto wildcard_match_iterator = node->children.find("*");
    if(wildcard_match_iterator != node->children.end()){
        dfs(wildcard_match_iterator->second, index + 1, tokens, result);
    }

    // if its a # we pass with same index and above isHash code will handle it 
    // we dont go to next index coz # means no next node as well
    // like sports.#  is a subscribed topic and if we publish just sports then that subscriber should get this message as well
    auto multi_level_wildcard_iterator = node->children.find("#");
    if(multi_level_wildcard_iterator != node->children.end()){
        dfs(multi_level_wildcard_iterator->second, index, tokens, result);
    }
}


/*
Below are abstract functions hiding undelrying implementation
*/

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