#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <memory>
#include <vector>

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
        uint32_t length;
        while(recv(socket, &length, sizeof(length), 0) > 0) {
            length = ntohl(length);

            if (length > 256'000'000) {
                std::cout<<"Message too big: "<<length<<std::endl;
                return;
            }

            std::vector<char> buffer(length + 1);

            ssize_t totalBytesRead = 0;
            while (totalBytesRead < length) {
            ssize_t bytesRead = recv(socket, buffer.data() + totalBytesRead, length - totalBytesRead, 0);
            if (bytesRead <= 0) {
                std::cout << "Client disconnected" << std::endl;
                return;
            }
            totalBytesRead += bytesRead;
        }

            buffer[length] = '\0';

            std::string message(buffer.data());

            logger->Write(message);
        }

        std::cout<<"Client disconnected"<<std::endl;
    
    }

}