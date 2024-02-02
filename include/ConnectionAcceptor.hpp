#pragma once

#include "./pch.hpp"

#include "./TcpServerBase.hpp"

class ConnectionAcceptor : public TcpServerBase {
private:
    int service_fd;
    void run() override;
public:
    ConnectionAcceptor(TcpServerController* tcp_server_controller);
    std::string getName() override;
};