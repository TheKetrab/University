// Bartlomiej Grochowski 300 951

#ifndef RESPONSE_H
#define RESPONSE_H

#include "util.h"
#include "server.h"

#define RESPONSE_MAX 1000000

char* rescode_description(int rescode);
char* mime_type(char* extension);
int send_response(struct server* s, int sockfd, struct request* r);
int host_is_valid(const char* path, const char* host_name);
int answer(int sockfd, int rescode, char* headers, uint8_t* data, ssize_t data_len, int keep_conn);

#endif // RESPONSE_H