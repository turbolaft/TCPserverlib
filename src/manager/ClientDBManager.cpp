#include "../../include/ClientDBManager.hpp"
#include "../../include/TcpClient.hpp"
#include "../../include/TcpServerController.hpp"

ClientDBManager::ClientDBManager(TcpServerController* tcp_server_controller)
    : tcp_server_controller(tcp_server_controller)
{
    
}

ClientDBManager::~ClientDBManager() {
    for (auto iterator = client_list.begin(); iterator != client_list.end(); iterator++) {
        delete *iterator;
    }
}

void ClientDBManager::add_client(TcpClient* client) {
    this->client_list.push_back(client);
}

void ClientDBManager::print() {
    for (auto iterator = client_list.begin(); iterator != client_list.end(); iterator++) {
        (*iterator)->print();
    }
}

void ClientDBManager::remove_client(TcpClient* client) {
    for (auto iterator = client_list.begin(); iterator != client_list.end(); iterator++) {
        if (*iterator == client) {
            client_list.erase(iterator);
            delete client;
            break;
        }
    }
}

TcpClient* ClientDBManager::lookUpClient(uint32_t address, uint16_t port) {
    for (auto iter = client_list.begin(); iter != client_list.end(); iter++) {
        if (network_convert_ip_p_to_n((*iter)->getAddress().c_str()) == address && (*iter)->getPort() == port) {
            return *iter;
        }
    }

    return nullptr;
}

const std::list<TcpClient*>& ClientDBManager::getClientList() const {
    return this->client_list;
}

void ClientDBManager::clear() {
    for (auto iterator = client_list.begin(); iterator != client_list.end(); iterator++) {
        (*iterator)->disconnect();
        delete *iterator;
    }
    client_list.clear();
}