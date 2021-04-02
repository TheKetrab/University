// Bartlomiej Grochowski 300 951

#ifndef RECEIVE_H
#define RECEIVE_H

struct response {
    int16_t seq;
    int16_t id;
    char ip_from[20];
    uint8_t type; // echo reply / time exceeded
    int32_t us_ping; // ping w mikrosekundach
};

int receive(int sockfd, struct response* res);
_Bool valid_response(struct response res, int ttl);
void print_info(struct response res[3], int n);

#endif // RECEIVE_H