#include "../../include/TcpClient.hpp"
#include "../../include/network_utils.hpp"

TcpClient::TcpClient(int client_fd, std::string ip_addr, uint16_t port)
 : client_fd(client_fd), ip_addr(ip_addr), port(port) {}

void TcpClient::print() {
    std::cout << "Client: " << this->ip_addr << ":" << this->port << std::endl;
}

TcpClient* TcpClient::create(int client_fd, sockaddr_in* client_addr) {
    char* ip_addr = network_convert_ip_n_to_p(client_addr->sin_addr.s_addr);
    uint16_t port = ntohs(client_addr->sin_port);
    return new TcpClient(client_fd, std::string(ip_addr), port);
}

int TcpClient::getFD() {
    return this->client_fd;
}

int TcpClient::send(std::string& data, int data_len) {
    return ::send(this->client_fd, data.c_str(), data_len, 0);
}

std::string TcpClient::getAddress() {
    return this->ip_addr;
}

uint16_t TcpClient::getPort() {
    return this->port;
}