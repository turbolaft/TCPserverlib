#pragma once

#include "pch.hpp"

class TcpClient;
class TcpServerController;

class ClientDBManager {
private:
    TcpServerController* tcp_server_controller;
    std::list<TcpClient*> client_list;
public:
    ClientDBManager(TcpServerController*);
    ~ClientDBManager();
    void add_client(TcpClient* client);
    void remove_client(TcpClient* client);
    TcpClient* lookUpClient(uint32_t address, uint16_t port);
    void print();
    void clear();
    const std::list<TcpClient*>& getClientList() const;
};