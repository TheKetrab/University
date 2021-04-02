// Bartlomiej Grochowski 300 951

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <math.h>

#include "transporter.h"

#define ERROR(str,ret) { fprintf(stderr, "%s: %s\n", str, strerror(errno)); return ret; }
#define INF 0xFFFFFFFFFFFFFFFF

struct transporter* create_transporter(int sockfd, char server_ip[], uint16_t server_port) {

    struct transporter* t = malloc(sizeof(struct transporter));
    t->start = 0; t->real_start=0; t->sockfd = sockfd;
    for (int i=0; i<FRAME; i++) t->received[i] = 0;

    bzero (&t->server, sizeof(t->server));
	t->server.sin_family      = AF_INET;
	t->server.sin_port        = htons(server_port);
	inet_pton(AF_INET, server_ip, &t->server.sin_addr);

    return t;
}

int send_request(struct transporter* t, uint64_t begin, uint64_t size) {

    char msg[25]; sprintf(msg,"GET %lu %lu\n",begin,size);
    ssize_t msg_len = strlen(msg);
    int bytes_sent = sendto( t->sockfd, msg, msg_len, 0,
        (struct sockaddr*) &t->server, sizeof(t->server)
    ); if (bytes_sent != msg_len) ERROR("sendto error",-1)

    return 1;
}

// zwraca ilosc dopisanych bajtow do pliku output, -1 jesli blad
uint64_t read_bytes(struct transporter* t, FILE* output) {

    struct sockaddr_in  sender;	
    socklen_t           sender_len = sizeof(sender);
    u_int8_t            buffer[IP_MAXPACKET+1];

    ssize_t len = recvfrom(t->sockfd, buffer, IP_MAXPACKET, 0,
        (struct sockaddr*)&sender, &sender_len
    ); if (len < 0) ERROR("recvfrom error",INF);

    // czy to server przyslal?
    if (sender.sin_addr.s_addr != t->server.sin_addr.s_addr
     || sender.sin_port != t->server.sin_port) return 0;

    // otrzymano: DATA start dlugosc\nABCD...
    uint64_t start = 0; uint16_t size = 0; int i = 5;
    for ( ; buffer[i]!=' '; i++) start = start*10 + buffer[i]-'0';
    for (++i; buffer[i]!='\n'; i++) size = size*10 + buffer[i]-'0';

    int number = start / (BYTES); // numer pakietu
    if (number < t->real_start || number > t->real_start+(FRAME)) return 0;
    uint16_t j=0; int k = num_to_index(t,number);
    if (t->received[k]) return 0; // jesli juz otrzymane, to nie nadpisuj
    for (++i; j<size; j++) t->data[k][j] = buffer[i+j];
    t->received[k] = size;
    
    // proba przesuniecia ramki
    uint64_t bytes_written = 0;
    for (int idx=t->start; t->received[idx]; ) {
        fwrite(t->data[idx],sizeof(char),t->received[idx],output);
        bytes_written += t->received[idx]; t->received[idx] = 0;        
        idx = (idx+1)%(FRAME); t->start = idx; t->real_start++;
    }    

    return bytes_written;
}

// procedura pobierania pliku z serwera
int get_file(struct transporter* t, FILE* file, uint64_t filesize, int percent_info) {

    int total_packets = ceil(filesize / (double)(BYTES));

    if (percent_info) printf("START\n");
    int prev = 0;

    uint64_t got_bytes = 0;
    while(got_bytes < filesize) {

        send_frame(t,filesize,total_packets);

        // select properties
        fd_set descriptors; FD_ZERO (&descriptors); FD_SET (t->sockfd, &descriptors);
        struct timeval tv; tv.tv_sec = WAIT_TIME; tv.tv_usec = 0;

        int ready = select(t->sockfd+1, &descriptors, NULL, NULL, &tv);
        if (ready < 0) ERROR("select from socket",-1)
        else if (ready == 0) { continue; } // timeout

        uint64_t res = read_bytes(t,file);
        if (res != INF) got_bytes += res;
        
        if (percent_info) {
            int percent = (float)got_bytes / (float)filesize * 100;
            if (percent > prev) { prev = percent; printf("%d%%\n",percent); }
        }
    }

    if (percent_info) printf("DONE\n");

    return 1;

}

// wysyla requesty po wszystkie pakiety, ktore sa w ramce i sa jeszcze nieotrzymane
void send_frame(struct transporter* t, uint64_t filesize, int total_packets) {

    // i: indeks w tablicy, j: iterator n razy, k: ktory to pakiet? 

    int j = 0, k = t->real_start;
    for (int i=t->start; j<FRAME; i = (i+1)%(FRAME)) {

        // ostatni pakiet
        if (k == total_packets-1) {
            uint64_t bytes_needed = filesize - k*(BYTES);
            send_request(t,k*(BYTES),bytes_needed);
            break;
        }
     
        // wysylaj requesty tylko do tych, ktorych nie masz
        if (!t->received[i]) {
            send_request(t,k*(BYTES),BYTES);
        }

        j++, k++;
    }
}

