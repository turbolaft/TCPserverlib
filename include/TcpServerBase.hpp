#pragma once

#include "./pch.hpp"

class TcpServerController;

class TcpServerBase {
protected:
    TcpServerController* tcp_server_controller;
    std::thread* service_thread;
    std::atomic<bool> stop_flag;
    virtual void run() = 0;
public:
    TcpServerBase(TcpServerController* tcp_server_controller);
    ~TcpServerBase();
    virtual void start_thread();
    virtual std::string getName() = 0;
    virtual void stop();
};