#pragma once

#include "./pch.hpp"

class TcpClient {
private:
    int client_fd;
    std::string ip_addr;
    uint16_t port;
public:
    TcpClient(int client_fd, std::string ip_addr, uint16_t port);
    void print();
    static TcpClient* create(int client_fd, sockaddr_in* client_addr);
    int getFD();
    int send(std::string& data, int data_len);
    std::string getAddress();
    uint16_t getPort();
};