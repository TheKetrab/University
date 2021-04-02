// Bartlomiej Grochowski 300 951

#ifndef SERVER_H
#define SERVER_H

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include "util.h"

#define MAX_CLIENTS 5
#define TIMEOUT 2 // jak dlugo utrzymuje polaczenie
#define STANDARD_SIZE 1000 // standardowa dlugosc bufora znakowego

struct client {
    int sockfd;
    int time;
};

struct server {
    struct client clients[MAX_CLIENTS];
    int max_sockfd;
    int sockfd;
    int port;
    const char* respath; // sciezka fizyczna do zasobow
};


struct request {
    const char* path; // sciezka wzgledna
    const char* host;
    const char* conn; // keep_alive?
};

int create_socket(char* port);
int disconnect(struct server* s, int i);
void disconnect_all(struct server* s);
int new_connection(struct server* s);
int server_listen(struct server* s);
int handle_request(struct server* s, int sockfd);
int parse_request(char* buffer, struct request* req, char* msg);
struct server* create_server(int sockfd, int port);

#endif // SERVER_H