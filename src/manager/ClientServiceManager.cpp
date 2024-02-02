#include "../../include/ClientServiceManager.hpp"
#include "../../include/TcpServerController.hpp"
#include "../../include/ClientDBManager.hpp"
#include "../../include/TcpClient.hpp"

ClientServiceManager::ClientServiceManager(TcpServerController* tcp_server_controller)
    : TcpServerBase(tcp_server_controller)
{
    FD_ZERO(&fdset);
    FD_ZERO(&backup_set);
}

int ClientServiceManager::getMaxFd() {
    const std::list<TcpClient*>& client_list = tcp_server_controller->getClientDBManager()->getClientList();

    if (client_list.empty()) {
        return -1;
    }

    int max_fd = client_list.front()->getFD();

    for (auto client : client_list) {
        if (client->getFD() > max_fd) {
            max_fd = client->getFD();
        }
    }

    return max_fd;
}

void ClientServiceManager::copyClientsToFdSet() {
    FD_ZERO(&backup_set);

    const std::list<TcpClient*>& client_list = tcp_server_controller->getClientDBManager()->getClientList();
    for (std::list<TcpClient*>::const_iterator it = client_list.begin(); it != client_list.end(); it++) {
        FD_SET((*it)->getFD(), &backup_set);
    }

    this->max_fd = getMaxFd();
}

void ClientServiceManager::run() {
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    std::list<TcpClient*>::const_iterator it;
    int readBytes;
    char* buffer = new char[BUFFER_SIZE];
    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    this->max_fd = getMaxFd();

    while (!stop_flag) {
        if (max_fd == -1) {
            continue; 
        }

        tcp_server_controller->mutex.lock();
        memcpy(&fdset, &backup_set, sizeof(fd_set));
        if (select(max_fd + 1, &fdset, NULL, NULL, &timeout) == -1) {
            perror("select");
            return;
        }
        tcp_server_controller->mutex.unlock();

        const std::list<TcpClient*>& client_list = tcp_server_controller->getClientDBManager()->getClientList();
        for (it = client_list.begin(); it != client_list.end(); it++) {
            if (stop_flag) {
                break;
            }
            if (FD_ISSET((*it)->getFD(), &fdset)) {

                if ((readBytes = recvfrom((*it)->getFD(), buffer, BUFFER_SIZE, 0, (sockaddr*)&client_addr, &client_addr_len)) == -1) {
            	    perror("recvfrom");
                    return;
                }

                if (readBytes == 0) {
                    tcp_server_controller->removeClient(*it);
                    break;
                }

                if (tcp_server_controller->onCLientSendData != nullptr && readBytes > 0) {
                    tcp_server_controller->onCLientSendData(tcp_server_controller, *it, buffer, readBytes);
                }
            }
        }
    }

    delete[] buffer;
    std::cout << "ClientServiceManager stopped" << std::endl;
}

std::string ClientServiceManager::getName() {
    return "ClientServiceManager";
}
