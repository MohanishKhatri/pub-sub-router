#include "../include/router.hpp"
#include<iostream>

int main(){
    Router* router = new Router();
    router->subscribe("sports.*", 1);
    // router->subscribe("sports.basketball", 2);
    // router->unsubscribe("sports.football", 10);
    // router->subscribe("sports.volleyball", 3);
    
    // router->subscribe("sports", 4);
    auto it = router->publish("sports.football");
    for(auto i : it){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    delete router;

    return 0;
}

