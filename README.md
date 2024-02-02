# TCP Server Library

The TCP Server Library is a powerful and flexible library that allows you to easily create and manage TCP servers in your applications. It provides a simple and intuitive API for handling incoming connections, sending and receiving data, and managing server resources.

## Table of Contents
- [Introduction](#introduction)
- [Key Features](#key-features)
- [Installation](#installation)
- [Usage](#usage)
- [Examples](#examples)

## Introduction

The TCP Server Library is designed to simplify the process of building TCP server applications. It abstracts away the complexities of low-level socket programming, allowing you to focus on implementing your server logic.

## Key Features

- Easy setup and configuration of TCP servers
- Support for handling multiple client connections
- Efficient data transmission and reception
- Customizable event-driven architecture
- Error handling and graceful shutdown

## Installation

To install the TCP Server Library, you can use the following command:

```bash
git clone https://github.com/turbolaft/TCPserverlib
cd TCPserverlib
make
./bin/main
```
## Usage

To use the TCP Server Library in your application, follow these steps:

1. Install the library by cloning the repository and building it using the provided Makefile.
2. Include the library's header file in your source code: `#include "../include/TCPserver.hpp"`.
3. Create an instance of the `TcpServerController` class, this class is responsible for handling all the other classes in the library.
4. Configure the server by setting the desired options, such as the listening port and maximum number of connections.
5. Implement the necessary event handlers for handling incoming connections `onClientConnected`, receiving data `onCLientSendData`, and client disconnection `onClientDisconnected`.
6. Start the server by calling the `start()` method. This will start two main threads, one for accepting incoming connections, the other one for handling those connections using multiplexing technologies.
7. Gracefully shut down the server when it is no longer needed by calling the `stop()` method.

Here's a simple example of how to use the TCP Server Library:

## Examples

```cpp
#include "../include/TCPserver.hpp"

// Event handler for when a client connects
void onClientConnected(TcpServerController* tcp_server, TcpClient* client) {
    // Handle the new client connection
    // ...
}

// Event handler for when a client sends data
void onClientSendData(TcpServerController* tcp_server, TcpClient* client, char* buffer, int readBytes) {
    // Handle the received data from the client
    // ...
}

// Event handler for when a client disconnects
void onClientDisconnected(TcpServerController* tcp_server, TcpClient* client) {
    // Handle the client disconnection
    // ...
}

int main() {
    // Initialize the server, param. - ip addr., port, name of server, max back_log
    TcpServerController* tcp_server = new TcpServerController("172.29.88.145", 8080, "PC-Artem");
    // set appropriate callbacks
    tcp_server->onClientConnected = onClientConnected;
    tcp_server->onCLientSendData = onClientSendData;
    tcp_server->onClientDisconnected = onClientDisconnected;
    // is keep alive
    tcp_server->is_keep_alive = true;
    // start the server, at this point ur server is able to accept and handle new connections
    tcp_server->start();

    // Wait for user input to stop the server
    std::cin.ignore();

    // Stop the server
    server.stop();

    return 0;
}
```
