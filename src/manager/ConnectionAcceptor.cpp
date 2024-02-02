#include "../../include/ConnectionAcceptor.hpp"
#include "../../include/TcpServerController.hpp"
#include "../../include/TcpClient.hpp"
#include "../../include/ClientDBManager.hpp"

#include <pthread.h>

ConnectionAcceptor::ConnectionAcceptor(TcpServerController* tcp_server_controller)
    : TcpServerBase(tcp_server_controller) {}

void ConnectionAcceptor::run() {
    try {
        if ((service_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
            std::cout << "Failed to create socket" << std::endl;
        }

        sockaddr_in server_addr;
        timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(this->tcp_server_controller->port);
        server_addr.sin_addr.s_addr = this->tcp_server_controller->ip_addr;

        int opt = 1;
        if (setsockopt(this->service_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            throw std::runtime_error("Failed to set socket options - REUSEADDR");
        }

        if (setsockopt(this->service_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
            throw std::runtime_error("Failed to set socket options - REUSEPORT");
        }

        if (setsockopt(this->service_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0) {
            throw std::runtime_error("Failed to set socket options - RCVTIMEO");
        }
        
        if (bind(this->service_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            throw std::runtime_error("Failed to bind socket");
        }

        if (listen(this->service_fd, this->tcp_server_controller->max_clients) < 0) {
            throw std::runtime_error("Failed to listen on socket");
        }

        std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";

        while (!this->stop_flag) {
            sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);
            int client_fd;

            if ((client_fd = accept(this->service_fd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    continue;
                }
            }

            if (client_fd < 0) {
                throw std::runtime_error("Failed to accept connection");
            }

            std::cout << "Accepted connection from " << network_convert_ip_n_to_p(client_addr.sin_addr.s_addr) << std::endl;

            TcpClient* client = TcpClient::create(client_fd, &client_addr);
            if (this->tcp_server_controller->is_keep_alive) {
                client->setKeepAlive();
            }
            this->tcp_server_controller->addNewClient(client);
        }
    } catch (std::runtime_error& e) {
        std::cout << "Exception in ConnectionAcceptor thread: " << e.what() << std::endl;
    }

    close(this->service_fd);
    service_fd = -1;
}

std::string ConnectionAcceptor::getName() {
    return "ConnectionAcceptor";
}