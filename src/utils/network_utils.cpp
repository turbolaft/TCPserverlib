#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>

char* network_convert_ip_n_to_p(uint32_t ip_addr) {
    return inet_ntoa(*(in_addr*)&ip_addr);
}

uint32_t network_convert_ip_p_to_n(const char* ip_addr) {
    return inet_addr(ip_addr);
}