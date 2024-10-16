#include <iostream>

#include "client.h"

#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::tm now_tm = *std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();

    return oss.str();
}


namespace client {

    Client::Client(const std::string& name, int port, int timeout): name(name), port(port), timeout(timeout){};
    Client::~Client(){
        if (sock>=0) {
            close(sock);
            sock = 0;
        }
    };

    void Client::Send() const{
        if (sock < 0) {
            throw std::runtime_error("not connected");
        }

        std::ostringstream oss;
        oss << "[" << getCurrentTime() << "]" << " \"" << name<<"\"";

        std::string message = oss.str();

        uint32_t length = htonl(message.size());
        if (send(sock, &length, sizeof(length), 0) < 0) {
            std::cerr << "send failed" << std::endl;
            return;
        }

        // Send message to server
        if (send(sock, message.c_str(), message.size(), 0) < 0) {
            std::cerr << "send failed" << std::endl;
        }
    };

    void Client::Run() const {
        if (sock < 0) {
            throw std::runtime_error("not connected");
        }

        while(true) {
            Send();
            std::cout<<"Message sent to server"<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(timeout));
        }
    }

    void Client::Connect(const std::string& ip) {
        int new_socket = -1;
        // Create socket
        if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw std::runtime_error("Socket creation error");
            return;
        }

        struct sockaddr_in serv_addr;


        serv_addr.sin_family = AF_INET; // IPv4
        serv_addr.sin_port = htons(port); // Port

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
            throw std::runtime_error("Invalid address/ Address not supported");
        }

        // Connect to the server
        if (connect(new_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            throw std::runtime_error("Connection failed");
        }

        sock = new_socket;
    }

}