#include <cstdlib>
#include <iostream>

#include "client.h"

int main(int argc, char* argv[]) {
    if (argc !=4) {
        std::cout<<"Pass 3 arguments: name, port, timeout(seconds)"<<std::endl;
        std::cout<<"Example: client name 8080 3"<<std::endl;
        return -1;
    }
    
    int port = std::atoi(argv[2]);
    
    if (port <= 0) {
        std::cout<<"Incorrect port, must be over 0"<<std::endl;
        return -1;
    }

    int timeout = std::atoi(argv[3]);
    
    if (timeout <= 0) {
        std::cout<<"Incorrect timeout, must be over 0"<<std::endl;
        return -1;
    }

    client::Client client(argv[1], port, timeout);

    client.Connect("127.0.0.1");

    client.Run();

    std::cout<<"Done"<<std::endl;

    return 0;
}
