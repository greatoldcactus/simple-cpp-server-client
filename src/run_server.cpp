#include <iostream>

#include "server.h"

int main(int argc, char* argv[]) {
    if (argc !=2) {
        std::cout<<"Pass 1 argument: port"<<std::endl;
        std::cout<<"Example: server 8080"<<std::endl;
        return -1;
    }
    
    int port = std::atoi(argv[1]);
    
    if (port <= 0) {
        std::cout<<"Incorrect port, must be over 0"<<std::endl;
        return -1;
    }
    server::Server server(port);

    server.ListenAndServe();
}