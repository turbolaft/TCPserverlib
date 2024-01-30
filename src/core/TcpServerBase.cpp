#include "../../include/TcpServerBase.hpp"
#include "../../include/TcpServerController.hpp"

TcpServerBase::TcpServerBase(TcpServerController* tcp_server_controller)
    : service_thread(nullptr),
      tcp_server_controller(tcp_server_controller),
      stop_flag(false) {}

void TcpServerBase::start_thread() {
    std::cout << "Starting " << getName() << " thread..." << std::endl;
    service_thread = new std::thread(&TcpServerBase::run, this);
}