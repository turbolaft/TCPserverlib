#include "../../include/TcpServerBase.hpp"
#include "../../include/TcpServerController.hpp"
#include "../../include/ConnectionAcceptor.hpp"

TcpServerBase::TcpServerBase(TcpServerController* tcp_server_controller)
    : service_thread(nullptr),
      tcp_server_controller(tcp_server_controller),
      stop_flag(false) {}

void TcpServerBase::start_thread() {
    if (service_thread != nullptr) {
        delete service_thread;
    }
    std::cout << "Starting " << getName() << " thread..." << std::endl;
    stop_flag = false;
    service_thread = new std::thread(&TcpServerBase::run, this);
    service_thread->detach();
}

void TcpServerBase::stop() {
    if (stop_flag) return;
    stop_flag = true;
}

TcpServerBase::~TcpServerBase() {
    stop();
    if (service_thread != nullptr) {
        delete service_thread;
    }
}