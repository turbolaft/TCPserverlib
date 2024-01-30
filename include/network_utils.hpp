#pragma once

#include <stdint.h>

char* network_convert_ip_n_to_p(uint32_t ip_addr);
uint32_t network_convert_ip_p_to_n(const char* ip_addr);