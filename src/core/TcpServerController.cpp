#include "../../include/TcpServerController.hpp"
#include "../../include/ClientDBManager.hpp"
#include "../../include/ClientServiceManager.hpp"
#include "../../include/ConnectionAcceptor.hpp"

TcpServerController::TcpServerController(std::string ip_addr, uint16_t port, std::string name, int max_clients)
 : ip_addr(network_convert_ip_p_to_n(ip_addr.c_str())), port(port), name(name), max_clients(max_clients)
{
    this->client_db_manager = new ClientDBManager(this);
    this->client_service_manager = new ClientServiceManager(this);
    this->connection_acceptor = new ConnectionAcceptor(this);
    this->onClientConnected = NULL;
    this->onCLientSendData = NULL;
    this->state_flags = 0;
    this->is_keep_alive = false;
    setBit(TCP_SERVER_INITIALIZED);
}

TcpServerController::~TcpServerController() {
    delete this->client_db_manager;
    delete this->client_service_manager;
    delete this->connection_acceptor;
}

void TcpServerController::start() {
    this->startAcceptingNewConnections();
    this->startListeningClients();
    setBit(TCP_SERVER_RUNNING);
}

void TcpServerController::startAcceptingNewConnections() {
    this->connection_acceptor->start_thread();
    unsetBit(TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS);
}

void TcpServerController::startListeningClients() {
    this->client_service_manager->start_thread();
    unsetBit(TCP_SERVER_NOT_LISTENING_CLIENTS);
}

const ClientDBManager* TcpServerController::getClientDBManager() {
    return this->client_db_manager;
}

std::string TcpServerController::printServerInfo() {
    std::cout << "Server info [" << network_convert_ip_n_to_p(this->ip_addr)
     << " : " << this->port << "]" << std::endl;
    std::cout << "State flags: ";
    std::cout << (isBitSet(TCP_SERVER_INITIALIZED) ? "INIT " : "");
    std::cout << (isBitSet(TCP_SERVER_RUNNING) ? "RUN " : "");
    std::cout << (isBitSet(TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS) ? "NOT_ACCEPT " : "");
    std::cout << (isBitSet(TCP_SERVER_NOT_LISTENING_CLIENTS) ? "NOT_LISTEN " : "");
    std::cout << std::endl;
    client_db_manager->print();

    std::string message = "Server info [" + std::string(network_convert_ip_n_to_p(this->ip_addr)) + " : " + std::to_string(this->port) + "]\n";
    message += "State flags: ";
    message += (isBitSet(TCP_SERVER_INITIALIZED) ? "INIT " : "");
    message += (isBitSet(TCP_SERVER_RUNNING) ? "RUN " : "");
    message += (isBitSet(TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS) ? "NOT_ACCEPT " : "");
    message += (isBitSet(TCP_SERVER_NOT_LISTENING_CLIENTS) ? "NOT_LISTEN " : "");
    message += "\n";

    return message;
}

void TcpServerController::addNewClient(TcpClient* client) {
    mutex.lock();
    this->client_db_manager->add_client(client);
    this->client_service_manager->copyClientsToFdSet();
    mutex.unlock();
    if (this->onClientConnected != NULL) this->onClientConnected(this, client);
}

void TcpServerController::setBit(uint32_t bit) {
    this->state_flags |= bit;
}

void TcpServerController::unsetBit(uint32_t bit) {
    this->state_flags &= ~bit;
}

bool TcpServerController::isBitSet(uint32_t bit) {
    return (this->state_flags & bit) == bit;
}

void TcpServerController::stop() {
    this->stopAcceptingNewConnections();
    this->stopListeningClients();
    this->client_db_manager->clear();
}

void TcpServerController::stopAcceptingNewConnections() {
    this->connection_acceptor->stop();
    setBit(TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS);
}

void TcpServerController::stopListeningClients() {
    this->client_service_manager->stop();
    setBit(TCP_SERVER_NOT_LISTENING_CLIENTS);
}

void TcpServerController::removeClient(TcpClient* client) {
    if (this->onClientDisconnected != NULL) this->onClientDisconnected(this, client);
    mutex.lock();
    this->client_db_manager->remove_client(client);
    this->client_service_manager->copyClientsToFdSet();
    mutex.unlock();
}

void TcpServerController::clientShutdown(TcpClient* client) {
    client->disconnect();
    this->removeClient(client);
}