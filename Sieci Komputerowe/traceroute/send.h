// Bartlomiej Grochowski 300 951

#ifndef SEND_H
#define SEND_H

#include <stdlib.h>
u_int16_t compute_icmp_checksum (const void *buff, int length);
int send_packet(int sockfd, char* ip, int ttl, int pid, int seq);

#endif // SEND_H