// Bartlomiej Grochowski 300 951

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <netinet/ip.h>
#include <arpa/inet.h>

#include <errno.h>

#include "entry.h"
#include "util.h"

#define ROUND_TIME 5 // sec
#define PORT 54321


struct router {

    int32_t sockfd;
    int size;
    struct entry* first_entry;
    struct entry* last_entry;
};

// send / listen
void share_route_table(struct router* r, _Bool all, struct entry* e);
void actualize(struct entry* temp, struct entry* received_entry, uint32_t sender_ip, uint32_t dist_to_sender, int round);
int get_and_update(struct router* r, int round);

// update list
void insert_entry(struct router* r, struct entry* e);
void delete_entry(struct router* r, struct entry* e);
int delete_entry_check(struct router* r, struct entry* e);
void check_table(struct router* r, int round);
void print_entries(struct router* r, _Bool debug);
int read_web_from_file(struct router* r, char* filename);
void typical_check(struct entry* temp, uint32_t sender_ip, uint32_t new_dist, int round);

// misc
uint32_t get_dist_to_entry(struct router* r, struct entry* e);
uint32_t get_dist_to_ip(struct router* r, uint32_t ip);
_Bool is_self_ip(struct router* r, uint32_t ip);
_Bool is_ip_in_direct_web(struct router* r, uint32_t ip);

#endif // ROUTER_HPP