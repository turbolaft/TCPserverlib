#pragma once

#include "./pch.hpp"

class TcpServerBase;
class ClientDBManager;
class ClientServiceManager;
class ConnectionAcceptor;

#include "../include/TcpClient.hpp"

class TcpServerController {
private:
    ClientDBManager* client_db_manager;
    ClientServiceManager* client_service_manager;
    ConnectionAcceptor* connection_acceptor;
public:
    const uint32_t ip_addr;
    const uint16_t port;
    const std::string name;
    void (*onClientConnected)(TcpServerController*, TcpClient*); // Ugly function pointer
    void (*onCLientSendData)(TcpServerController*, TcpClient*, char*, int); // Ugly function pointer

    TcpServerController(std::string ip_addr, uint16_t port, std::string name);
    void start();
    void printServerInfo();
    void stop();

    const ClientDBManager* getClientDBManager();

    void addNewClient(TcpClient* client);
};