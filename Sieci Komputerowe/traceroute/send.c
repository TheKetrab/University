// Bartlomiej Grochowski 300 951

#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <assert.h>
#include <strings.h>

u_int16_t compute_icmp_checksum (const void *buff, int length) {

	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

// wysyla pakiet; zwraca -1 jesli blad
int send_packet(int sockfd, char* ip, int ttl, int pid, int seq) {

    // tworzenie danych do wysylki
    struct icmphdr header;
    header.type = ICMP_ECHO;
    header.code = 0;
    header.un.echo.id = pid;
    header.un.echo.sequence = seq;
    header.checksum = 0;
    header.checksum = compute_icmp_checksum (
        (u_int16_t*)&header, sizeof(header));

    //adresowanie
    struct sockaddr_in recipient;
    bzero (&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &recipient.sin_addr);
    setsockopt (sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));

    // wysylanie przez gniazdo
    ssize_t bytes_sent = sendto (  
        sockfd,  
        &header,   
        sizeof(header),  
        0,   
        (struct sockaddr*) &recipient,   
        sizeof(recipient)
    );

    if (bytes_sent <= 0) {
        fprintf(stderr, "sendto error: %s\n", strerror(errno)); 
		return -1;
    }

    return 0;

}