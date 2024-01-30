#include "../../include/ClientDBManager.hpp"
#include "../../include/TcpClient.hpp"

ClientDBManager::ClientDBManager(TcpServerController* tcp_server_controller)
    : tcp_server_controller(tcp_server_controller)
{
    
}

void ClientDBManager::add_client(TcpClient* client) {
    this->client_list.push_back(client);
}

void ClientDBManager::print() {
    for (auto iterator = client_list.begin(); iterator != client_list.end(); iterator++) {
        (*iterator)->print();
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