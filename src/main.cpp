#include <iostream>
#include <string>
#include "../include/TCPserver.hpp"

static void clientCredentials(TcpServerController* tcp_server, TcpClient* client) {
    tcp_server->printServerInfo();

    if (tcp_server->getClientDBManager()->getClientList().size() == 4) {
        tcp_server->stopAcceptingNewConnections();
    }
}

static void clientDisconnects(TcpServerController* tcp_server, TcpClient* client) {
    std::cout << "Disconnected ";
    client->print();

    if (tcp_server->getClientDBManager()->getClientList().size() == 4) {
        tcp_server->startAcceptingNewConnections();
    }
}

static void clientSendData(TcpServerController* tcp_server, TcpClient* client, char* buffer, int readBytes) {
    std::string message = client->getAddress()  + ":" + std::to_string(client->getPort()) + "\t" + std::string(buffer, readBytes);
    std::cout << message << "readBytes: " << readBytes << std::endl;
    const std::list<TcpClient*>& client_list = tcp_server->getClientDBManager()->getClientList();

    if (std::string(buffer, readBytes) == "server info\n") {
        std::string info = tcp_server->printServerInfo();
        client->send(info, info.length());
        return;
    }

    if (std::string(buffer, readBytes) == "stop\n") {
        tcp_server->stop();
        tcp_server->printServerInfo();
        return;
    }

    for (auto iter = client_list.begin(); iter != client_list.end(); iter++) {
        (*iter)->send(message, message.length());
    }
}

int main() {
    TcpServerController* tcp_server = new TcpServerController("172.29.88.145", 8080, "PC-Artem");
    tcp_server->onClientConnected = clientCredentials;
    tcp_server->onCLientSendData = clientSendData;
    tcp_server->onClientDisconnected = clientDisconnects;
    tcp_server->is_keep_alive = true;
    tcp_server->start();
    // long i = 0;
    // while (i < 100000000000) i++;

    // tcp_server->stop();

    while (1);

    return 0;
}