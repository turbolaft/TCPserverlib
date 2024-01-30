#include <iostream>
#include <string>
#include "../include/TcpClient.hpp"
#include "../include/TcpServerController.hpp"
#include "../include/ClientDBManager.hpp"

static void clientCredentials(TcpServerController* tcp_server, TcpClient* client) {
    tcp_server->printServerInfo();
    client->print();
}

static void clientSendData(TcpServerController* tcp_server, TcpClient* client, char* buffer, int readBytes) {
    std::string message = client->getAddress()  + ":" + std::to_string(client->getPort()) + "\t" + std::string(buffer, readBytes);
    std::cout << message << "readBytes: " << readBytes << std::endl;

    const std::list<TcpClient*>& client_list = tcp_server->getClientDBManager()->getClientList();

    for (auto iter = client_list.begin(); iter != client_list.end(); iter++) {
        (*iter)->send(message, message.length());
    }
}

int main() {
    TcpServerController* tcp_server = new TcpServerController("172.29.88.145", 8080, "PC-Artem");
    tcp_server->onClientConnected = clientCredentials;
    tcp_server->onCLientSendData = clientSendData;
    tcp_server->start();
    long i = 0;
    while (i < 100000000000) i++;

    tcp_server->stop();

    while (1);

    return 0;
}
