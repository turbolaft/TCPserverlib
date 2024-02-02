#pragma once

#include "./pch.hpp"

class TcpClient {
private:
    int client_fd;
    std::string ip_addr;
    uint16_t port;
public:
    TcpClient(int client_fd, std::string ip_addr, uint16_t port);
    ~TcpClient();
    void print();
    static TcpClient* create(int client_fd, sockaddr_in* client_addr);
    int getFD();
    int send(std::string& data, int data_len);
    void disconnect();
    void setKeepAlive();
    std::string getAddress() const;
    uint16_t getPort() const;
};