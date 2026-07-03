#include "../include/router.hpp"
#include<iostream>

int main(){
    Router* router = new Router();
    router->subscribe("sports.football", 1);
    router->subscribe("sports.basketball", 2);
    router->subscribe("sports.football", 3);
    router->subscribe("sports", 4);
    auto it = router->publish("sports");
    for(auto i : it){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}