#include "../../include/TcpClient.hpp"
#include "../../include/network_utils.hpp"

TcpClient::TcpClient(int client_fd, std::string ip_addr, uint16_t port)
 : client_fd(client_fd), ip_addr(ip_addr), port(port) {}

TcpClient::~TcpClient() {
    if (this->client_fd != -1)
        close(this->client_fd);
}

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

std::string TcpClient::getAddress() const {
    return this->ip_addr;
}

uint16_t TcpClient::getPort() const {
    return this->port;
}

void TcpClient::disconnect() {
    shutdown(this->client_fd, SHUT_RDWR);
    close(this->client_fd);
    this->client_fd = -1;
}

void TcpClient::setKeepAlive() {
    int opt = 1;
    if (setsockopt(this->client_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Failed to set socket options - KEEPALIVE");
    }
}