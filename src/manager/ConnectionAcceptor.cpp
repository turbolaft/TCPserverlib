#include "../../include/ConnectionAcceptor.hpp"
#include "../../include/TcpServerController.hpp"
#include "../../include/TcpClient.hpp"
#include "../../include/ClientDBManager.hpp"

ConnectionAcceptor::ConnectionAcceptor(TcpServerController* tcp_server_controller)
    : TcpServerBase(tcp_server_controller)
{
    if ((service_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cout << "Failed to create socket" << std::endl;
    }
}

ConnectionAcceptor::~ConnectionAcceptor() {
    // delete cas_thread;
    // close(cas_fd);
}

void ConnectionAcceptor::run() {
    try {
        sockaddr_in server_addr;
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
        
        if (bind(this->service_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            throw std::runtime_error("Failed to bind socket");
        }

        if (listen(this->service_fd, 10) < 0) {
            throw std::runtime_error("Failed to listen on socket");
        }

        while (true) {
            sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);

            std::cout << "Waiting for connection..." << std::endl;
            int client_fd = accept(this->service_fd, (struct sockaddr*)&client_addr, &client_addr_len);

            if (client_fd < 0) {
                throw std::runtime_error("Failed to accept connection");
            }

            std::cout << "Accepted connection from " << network_convert_ip_n_to_p(client_addr.sin_addr.s_addr) << std::endl;

            send(client_fd, "Hello, world!\n", 14, 0);
            
            TcpClient* client = TcpClient::create(client_fd, &client_addr);
            this->tcp_server_controller->addNewClient(client);
        }
    } catch (std::runtime_error& e) {
        std::cout << "Exception in ConnectionAcceptor thread: " << e.what() << std::endl;
    }
}

std::string ConnectionAcceptor::getName() {
    return "ConnectionAcceptor";
}

void ConnectionAcceptor::stop() {
    
}