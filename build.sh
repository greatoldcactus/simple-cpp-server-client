mkdir bin

g++ ./src/client.cpp ./src/run_client.cpp -o ./bin/client
g++ ./src/logger.cpp ./src/server.cpp ./src/run_server.cpp -o ./bin/client