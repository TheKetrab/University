// Bartlomiej Grochowski 300 951

#ifndef UTIL_H
#define UTIL_H

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


inline uint32_t make_ip(uint8_t i1, uint8_t i2, uint8_t i3, uint8_t i4) {
    return (((uint32_t)i1)<<24) | (((uint32_t)i2)<<16) | (((uint32_t)i3)<<8) | ((uint32_t)i4);
}

char* str_hex(uint8_t* buffer, int n);
char* ip_to_string(uint32_t ip);
uint32_t ip_to_broadcast(uint32_t ip, uint8_t mask);
uint32_t ip_to_web_address(uint32_t ip, uint8_t mask);

void spread_udp_packet(char* buffer, uint32_t* ip, uint8_t* mask, uint32_t* dist);
uint32_t get_data(uint8_t* buffer);
void put_data(uint8_t* buffer, uint32_t data);

#endif // UTIL_H