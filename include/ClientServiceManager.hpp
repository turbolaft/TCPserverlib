#pragma once

#include "./pch.hpp"

class TcpServerController;

#include "./TcpServerBase.hpp"

#define BUFFER_SIZE 1024

class ClientServiceManager : public TcpServerBase {
private:
    fd_set fdset;
    fd_set backup_set;
    int max_fd;
    void run() override;
public:
    ClientServiceManager(TcpServerController* tcp_server_controller);
    int getMaxFd();
    void copyClientsToFdSet();
    std::string getName() override;
};