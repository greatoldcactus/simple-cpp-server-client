Here’s the expanded description for your repository in English:

# simple-cpp-server-client 

A simple TCP client and server written in C++ using the Linux socket API. This project demonstrates the basics of network programming and object-oriented programming principles in C++. 

## Description

This project includes the implementation of a console-based TCP client and server that communicate with each other via sockets. The client sends messages to the server at a specified interval, while the server receives these messages and logs them to a file. The project is intended for learning the fundamentals of socket programming in Linux and applying OOP principles in C++.

### What it does

- **Client**: 
  - Connects to the server using a specified name and port.
  - Sends messages in the format `[yyyy-mm-dd hh:mm:ss.ms] "client_name"` at a defined interval.
  
- **Server**: 
  - Listens on a specified port and accepts messages from clients.
  - Logs each message to a file named `log.txt`, ensuring correct access to the file when working with multiple clients.

## Running

### Client

To run the client, use the following command:

```sh
./bin/client <name> <port> <timeout>
```

Where:
- `<name>` — the text name of the client.
- `<port>` — the server's port number.
- `<timeout>` — the connection interval to the server in seconds.

### Server

To run the server, use the following command:

```sh
./bin/server <port>
```

Where:
- `<port>` — the port number on which the server will listen for incoming connections.

## Building

### All

To build the project, execute:

```sh
bash build.sh
```

### Client

To build the client, execute:

```sh
g++ ./src/client.cpp ./src/run_client.cpp -o ./bin/client
```

### Server

To build the server, execute:

```sh
g++ ./src/logger.cpp ./src/server.cpp ./src/run_server.cpp -o ./bin/server
```