// Bartlomiej Grochowski 300 951

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include "entry.h"
#include "util.h"

// alokuje pamiec dla nowego wpisu
struct entry* create_entry(uint32_t web_ip, uint8_t mask, uint32_t dist) {
    struct entry* e = malloc(sizeof(struct entry));
    e->web_ip = web_ip;
    e->mask = mask;
    e->dist = dist;
    e->basic_dist = 0;
    e->via_ip = 0;
    e->next = NULL;
    e->direct = 0;
    return e;
}

void print_entry(struct entry* e) {

    if (e == NULL) {
        printf("ERROR print_entry NULL");
        return;
    }

    // IP
    printf("%s/%"PRIu8"\t",ip_to_string(ip_to_web_address(e->web_ip,e->mask)),e->mask);

    // DISTANCE
    if (get_reachable(e)) {
        printf("distance %"PRIu32" ",e->dist);
    } else {
        printf("unreachable ");
    }

    // VIA?
    if (e->direct && e->via_ip == 0) {
        printf("connected directly\n");
    } else {
        printf("via %s\n",ip_to_string(e->via_ip));
    }
}

void print_entry_debug(struct entry* e) {

    if (e == NULL) {
        printf("ERROR print_entry NULL");
        return;
    }

    printf("round %d\tnext = %p\t",e->round,e->next);
    if (e->next == NULL) printf("\t");
    print_entry(e);
}

_Bool is_ip_in_web(struct entry* e, uint32_t ip) {
    return ip_to_web_address(ip,e->mask) == ip_to_web_address(e->web_ip,e->mask);
}

char* create_udp_packet(struct entry* e) {
    uint8_t* packet = malloc(9); // 9 bajtow
    put_data(packet,htonl( ip_to_web_address(e->web_ip,e->mask) ));
    packet[4] = e->mask;
    put_data(packet+5,htonl(e->dist));
    return (char*)packet;
}

void set_unreachable(struct entry* e) { 
    e->dist = INF;
    if (e->direct) e->via_ip = 0;
}
