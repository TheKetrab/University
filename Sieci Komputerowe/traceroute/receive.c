// Bartlomiej Grochowski 300 951

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/ip_icmp.h>
#include "receive.h"

// sprawdza, czy to response pochodzi z obecnego ttl 
_Bool valid_response(struct response res, int ttl) {
	return ttl == res.seq / 3 + 1;
}

// odbiera z gniazda to co tam przyszło i wstrzykuje do res;
// zwraca -1 jesli blad
int receive(int sockfd, struct response* res) {

	struct sockaddr_in sender;
	socklen_t sender_len = sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET];

	ssize_t packet_len = recvfrom(
		sockfd,
		buffer,
		IP_MAXPACKET,
		0,
		(struct sockaddr*)&sender,
		&sender_len
	);

	if (packet_len < 0) {
		fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
		return -1;
	}

	char sender_ip_str[20]; 
	inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));

	// [iphdr][icmphdr][data]
	struct iphdr* ip_header = (struct iphdr*) buffer;
	ssize_t ip_header_len = 4 * ip_header->ihl;	
	u_int8_t* icmp_packet = buffer + ip_header_len; // icmp jest po ip
	struct icmphdr* icmp_header = (struct icmphdr*) icmp_packet;

	// pakiet dotarl, nie zostal zabity
	if (icmp_header->type == ICMP_ECHOREPLY) {
		// wtedy icmp ma takie same dane
		res->id = icmp_header->un.echo.id;
		res->seq = icmp_header->un.echo.sequence;
		res->type = ICMP_ECHOREPLY;
		strcpy(res->ip_from,sender_ip_str);
	}


	// https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol#Time_exceeded
	// pakiet zostal zabity przez TTL
	else if (icmp_header->type == ICMP_TIME_EXCEEDED) {
		// wtedy w odpowiedzi dostajemy w DATA icmp z requestu

		// przeskocz iphdr, a potem przeskocz icmpheader (+8)
		u_int8_t* rec_data = buffer + 4*ip_header->ihl + 8;
		struct iphdr* rec_ip_header = (struct iphdr*) rec_data; // przeskocz jeszcze o naglowek odeslanego ip
		struct icmphdr* rec_icmp_header = (struct icmphdr*) (rec_data + 4*rec_ip_header->ihl);

		res->id = rec_icmp_header->un.echo.id;
		res->seq = rec_icmp_header->un.echo.sequence;
		res->type = ICMP_TIME_EXCEEDED;
		strcpy(res->ip_from,sender_ip_str);

	}

    return 0;

}

// wypisuje informacje o pakietach
void print_info(struct response res[3], int n) {

	// nikt nie odpowiedzial
	if (n == 0) {
		printf("*\n");
		return;
	}

	// jakie routery odpowiedzialy?
	printf("%s ",res[0].ip_from);
	if (n>=2 && strcmp(res[0].ip_from,res[1].ip_from) != 0)
		printf("%s ",res[1].ip_from);
	if (n>=3 && strcmp(res[0].ip_from,res[2].ip_from) != 0
     && strcmp(res[1].ip_from,res[2].ip_from) != 0)
		printf("%s ",res[2].ip_from);

	// sredni czas
	if (n < 3) {
		printf("???\n");
		return;
	}

	int sum=0;
	for (int i=0; i<n; i++) {
		sum+=res[i].us_ping;
	}

	printf("%.2f ms\n", (float)(sum/3.0 / 1000));

}