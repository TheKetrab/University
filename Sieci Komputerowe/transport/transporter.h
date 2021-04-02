// Bartlomiej Grochowski 300 951

#ifndef TRANSPORTER_H
#define TRANSPORTER_H

#include <stdio.h>
#include <inttypes.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define BYTES 1000 // bajty na pakiet [1,1000]
#define FRAME 350 // dlugosc okna przesuwnego
#define WAIT_TIME 1 // czas oczekiwania, timeout

struct transporter {
    uint16_t received[FRAME];
    char data[FRAME][BYTES];
    struct sockaddr_in server;
    int start, real_start;
    int sockfd;
};

struct transporter* create_transporter(int sockfd, char server_ip[], uint16_t server_port);
int send_request(struct transporter* t, uint64_t begin, uint64_t size);
uint64_t read_bytes(struct transporter* t, FILE* output);
void try_move_frame(struct transporter* t, FILE* output);
int get_file(struct transporter* t, FILE* file, uint64_t filesize, int percent_info);
void send_frame(struct transporter* t, uint64_t filesize, int total_packets);

// zamienia numer rzeczywisty na numer odpowiadajacy w tablicy
inline int num_to_index(struct transporter* t, int number) {
    return (t->start + (number - t->real_start)) % FRAME;
}


#endif // TRANSPORTER_H