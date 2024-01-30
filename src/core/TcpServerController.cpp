#include "../../include/TcpServerController.hpp"
#include "../../include/ClientDBManager.hpp"
#include "../../include/ClientServiceManager.hpp"
#include "../../include/ConnectionAcceptor.hpp"

TcpServerController::TcpServerController(std::string ip_addr, uint16_t port, std::string name)
 : ip_addr(network_convert_ip_p_to_n(ip_addr.c_str())), port(port), name(name)
{
    this->client_db_manager = new ClientDBManager(this);
    this->client_service_manager = new ClientServiceManager(this);
    this->connection_acceptor = new ConnectionAcceptor(this);
    this->onClientConnected = NULL;
    this->onCLientSendData = NULL;
}

void TcpServerController::start() {
    this->connection_acceptor->start_thread();
    this->client_service_manager->start_thread();
}

const ClientDBManager* TcpServerController::getClientDBManager() {
    return this->client_db_manager;
}

void TcpServerController::printServerInfo() {
    std::cout << "Server name: " << this->name << std::endl;
    std::cout << "Server IP: " << network_convert_ip_n_to_p(this->ip_addr) << std::endl;
    std::cout << "Server port: " << this->port << std::endl;
}

void TcpServerController::addNewClient(TcpClient* client) {
    this->client_db_manager->add_client(client);
    if (this->onClientConnected != NULL) this->onClientConnected(this, client);
    this->client_service_manager->addClientToFdSet();
}

void TcpServerController::stop() {
    this->connection_acceptor->stop();
    this->client_service_manager->stop();
}