// Bartlomiej Grochowski 300 951

#include "util.h"
#include <inttypes.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

// zamienia 4 kolejne bajty na inta
uint32_t get_data(uint8_t* buffer) {
    return (((uint32_t)buffer[0]) << 24) 
         | (((uint32_t)buffer[1]) << 16)
         | (((uint32_t)buffer[2]) << 8)
         | (((uint32_t)buffer[3]) << 0);
}

// wklada inta do kolejnych czterech bajtow
void put_data(uint8_t* buffer, uint32_t data) {
    buffer[0] = (uint8_t)(data >> 24);
    buffer[1] = (uint8_t)(data >> 16);
    buffer[2] = (uint8_t)(data >> 8);
    buffer[3] = (uint8_t)data;
}

// adres rozgloszeniowy sieci
uint32_t ip_to_broadcast(uint32_t ip, uint8_t mask) {
    if (mask == 0) return ip;
    int32_t prefix = 0x80000000;
    prefix >>= (mask - 1);
    // ~prefix -> jedynki na koncu
    return ip | ~prefix;
}

// adres sieci
uint32_t ip_to_web_address(uint32_t ip, uint8_t mask) {
    if (mask == 0) return ip;
    int32_t prefix = 0x80000000;
    prefix >>= (mask - 1);
    return ip & prefix;
}

// do wypisywania %s
char* ip_to_string(uint32_t ip) {

    // 4*3cyfry + 3 kropki + \0 
    char* str = malloc(sizeof(char)*16);
    sprintf(str,
        "%"PRIu8".%"PRIu8".%"PRIu8".%"PRIu8,
        (ip >> 24) & 0xFF,
        (ip >> 16) & 0xFF,
        (ip >> 8) & 0xFF,
        ip & 0xFF
    );

    return str;
}

// daje stringa n bajtow z bufora; np: 0A FF 4E C0
char* str_hex(uint8_t* buffer, int n) {
    char* str = (char*)malloc(3*n);
    char* p = str;
    for (int i=0; i<n; i++)
        p += sprintf(p, "%x ", buffer[i]);
    return str;
}

// wyciaga z gniazda ciag bajtow, ktory przyszedl - odczytuja ip, maske, dystans
void spread_udp_packet(char* buffer, uint32_t* ip, uint8_t* mask, uint32_t* dist) {
    *ip   =  ntohl( get_data( (uint8_t*) buffer   ) );
    *mask =                *( (uint8_t*)(buffer+4 ) );
    *dist =  ntohl( get_data( (uint8_t*)(buffer+5)) );
}
