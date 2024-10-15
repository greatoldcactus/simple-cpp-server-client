#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <memory>
#include <string>

    namespace client {

    constexpr int BUFFER_SIZE = 1024;

    class Client {
        std::string name;
        int port;
        int timeout;
        int sock{-1};

        public:
        Client(const std::string& name, int port, int timeout);
        ~Client();

        void Send() const;
        void Run() const;
        void Connect(const std::string& ip);
    };

}

#endif