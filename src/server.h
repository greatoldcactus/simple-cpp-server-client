#ifndef __SERVER_H__
#define __SERVER_H__

#include <cstdint>

#include "logger.h"

#include <memory>

namespace server {

    class Server {
        int server;

        int port;

        std::shared_ptr<Logger> logger;

        void Close();
        void ServeSocket(int socket);

        public:

        Server(int port);
        Server(int port, const std::string& filename);
        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;
        ~Server();

        void ListenAndServe();
    };

}

#endif