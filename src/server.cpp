#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <memory>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.h"

namespace server {

    Server::Server(int port):Server(port, "log.txt"){};

    Server::Server(int port, const std::string& filename): port(port), server(-1) {
        logger = std::make_shared<Logger>(filename);
    }

    Server::~Server() {
        Close();
    }

    void Server::Close() {
        if (server >= 0) {
            close(server);
            server = -1;
        }
    }

    void Server::ListenAndServe() {
        int opt = 1;
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        
        if ((server = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            throw std::runtime_error("socket failed");
        }

        if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            throw std::runtime_error("setsockopt failed");
        }

        address.sin_family = AF_INET; // IPv4
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);


        // Bind the socket to the address
        if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
            throw std::runtime_error("bind failed");
        }

            // Start listening for connections
        if (listen(server, 3) < 0) {
            throw std::runtime_error("listen failed");
        }

        std::cout << "Server is listening on port " << port << std::endl;

        while (true) {
            int socket;
            
            if ((socket = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                throw std::runtime_error("connection accept failed");
                continue;
            }

            constexpr auto closeSocket = [](int*s){
                close(*s);
            };

            std::unique_ptr<std::remove_pointer_t<int>, decltype(closeSocket)> socketPtr(new int(socket), closeSocket);


            std::thread thread([this, socketPtr = std::move(socketPtr)](){
                std::cout<<"Client connected"<<std::endl;
                this->ServeSocket(*socketPtr);
            });

            thread.detach();
        }

        Close();
    }

    void Server::ServeSocket(int socket){
        while(true) {
            char buffer[BUFFER_SIZE] = {0};

            // Read data from the client
            int valread = read(socket, buffer, BUFFER_SIZE);
            if (valread == 0) {
                std::cout<<"Client disconnected"<<std::endl;
                return;
            }
            if (valread < 0) {
                std::cerr << "Error when reading from socket" << std::endl;
                return;
            }

            std::string message = buffer;

            logger->Write(message);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    
    }

}