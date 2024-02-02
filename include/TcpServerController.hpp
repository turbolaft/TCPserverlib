#pragma once

class TcpServerBase;
class ClientDBManager;
class ClientServiceManager;
class ConnectionAcceptor;

#define TCP_SERVER_INITIALIZED (1)
#define TCP_SERVER_RUNNING (2)
#define TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS (4)
#define TCP_SERVER_NOT_LISTENING_CLIENTS (8)

#include "../include/TcpClient.hpp"

class TcpServerController {
private:
    ClientDBManager* client_db_manager;
    ClientServiceManager* client_service_manager;
    ConnectionAcceptor* connection_acceptor;
    uint32_t state_flags;
    void setBit(uint32_t bit);
    void unsetBit(uint32_t bit);
    bool isBitSet(uint32_t bit);
public:
    const uint32_t ip_addr;
    const uint16_t port;
    const std::string name;
    const int max_clients;
    bool is_keep_alive;
    std::mutex mutex;
    void (*onClientConnected)(TcpServerController*, TcpClient*); // Ugly function pointer
    void (*onCLientSendData)(TcpServerController*, TcpClient*, char*, int); // Ugly function pointer
    void (*onClientDisconnected)(TcpServerController*, TcpClient*); // Ugly function pointer

    TcpServerController(std::string ip_addr, uint16_t port, std::string name, int max_clients = 10);
    ~TcpServerController();
    void start();
    void startAcceptingNewConnections();
    void startListeningClients();
    std::string printServerInfo();
    void stop();
    void stopAcceptingNewConnections();
    void stopListeningClients();
    void clientShutdown(TcpClient* client);

    const ClientDBManager* getClientDBManager();

    void addNewClient(TcpClient* client);
    void removeClient(TcpClient* client);
};