// Bartlomiej Grochowski 300 951

#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <inttypes.h>
#include "util.h"

#define INF 0xFFFFFFFF // nieskonczonosc
#define MAX 16 // max powyzej, ktorego uwazamy, ze wystapilo zliczanie do nieskonczonosci

// web_ip - adres sieci, do ktorej jest ten wpis prowadzi

struct entry {
    struct entry* next;
    uint32_t web_ip;     // wpis bezposredni ? adres swojej karty : adres sieci
    uint32_t dist;       // odleglosc do sieci
    uint32_t basic_dist; // odleglosc do sieci jesli jest direct (ta podana na wejsciu) - potrzebna jesli odzyska polaczenie z bazowa siecia
    uint32_t via_ip;     // pierwszy router na trasie
    int32_t round;       // z ktorej tury info?
    uint8_t mask;        // maska sieci
    _Bool direct;        // czy polaczony bezposrednio
};

inline _Bool get_reachable(struct entry* e) { return e->dist != INF; }
inline _Bool cmp_entries(struct entry* e1, struct entry* e2) { 
    return ip_to_web_address(e1->web_ip,e1->mask) == ip_to_web_address(e2->web_ip,e2->mask);
}

void set_unreachable(struct entry* e);
struct entry* create_entry(uint32_t web_ip, uint8_t mask, uint32_t dist);
void print_entry(struct entry* e);
void print_entry_debug(struct entry* e);
char* create_udp_packet(struct entry* e);
_Bool is_ip_in_web(struct entry* e, uint32_t ip);

#endif // ENTRY_HPP