// Bartlomiej Grochowski 300 951

/**
 * TRACEROUTE
 * Program parsuje podane IP, tj. sprawdza, czy ma 4 części
 * oddzielone kropka mieszczące się na jednym bajcie (<255).
 * Nastepnie 30 razy wykonuje kroki:
 *  - wyslij 3 zadania
 *  - czekaj na odpowiedzi
 * Mierzy czas do ich powrotu, wysyla z coraz wiekszym TTL,
 * a kazdy echo request ma inny numer seryjny (zmienna total).
 * Rezultaty przechowujemy w strukturach response.
 * Na podstawie trzech otrzymanych odpowiedzi w każdej
 * iteracji zbieramy wynik i wypisujemy go.
 */

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <regex.h>
#include <netinet/ip_icmp.h>

#include "receive.h"
#include "send.h"


// czekaj na odpowiedz od routerow 1s
#define WAIT_TIME 1

_Bool is_valid_ip(char* ip);
inline _Bool is_digit(char c) {	return c >= 48 && c <= 57; }
inline _Bool is_dot(char c) { return c == 46; }

int main(int argc, char* argv[]) {

	if (argc != 2) {
		printf("Run program with a.b.c.d argument.\n");
		return EXIT_FAILURE;
	}

	if (!is_valid_ip(argv[1])) {
		fprintf(stderr, "wrong ip: %s\n", argv[1]); 
		return EXIT_FAILURE;
	}

	// tworzy gniazdo sieciowe
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0) {
		fprintf(stderr,"Run program as administrator (sudo ./traceroute IP).\n");
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

	int pid = getpid();
	int total = 0; 			// wszystkie wyslane pakiety
	_Bool dest = 0; 		// czy doszedl do celu?

	for (int i=1; i<=30 && !dest; i++) {

		printf("%d. ", i );

		// czas wyslania pakietow
		struct timeval stop, start;
		gettimeofday(&start, NULL);

		// wyslij 3 pakiety
		for (int j=0; j<3; j++)
			if (send_packet(sockfd,argv[1],i,pid,total++) < 0)
				return EXIT_FAILURE;

		// select properties
		fd_set descriptors;
		FD_ZERO (&descriptors);
		FD_SET (sockfd, &descriptors);
		struct timeval tv; tv.tv_sec = WAIT_TIME; tv.tv_usec = 0;
		
		struct response responses[3];
		int res_cnt = 0;
		
		// skoncz czekac, gdy otrzymasz 3 DOBRE odpowiedzi
		while (res_cnt < 3) {

			int ready = select( sockfd + 1, &descriptors, NULL, NULL, &tv );
			if (ready < 0) { fprintf(stderr,"error in fun 'select'\n"); return EXIT_FAILURE; }
			else if (ready == 0) break; // timeout

			// jest co zabrac z gniazda
			else {
				
				struct response res;
				if (receive(sockfd,&res) < 0)
					return EXIT_FAILURE;

				if (valid_response(res,i)) {			// ignoruj stare pakiety (ttl)
					gettimeofday(&stop, NULL);
					
					res.us_ping = 
						(stop.tv_sec - start.tv_sec) * 1000000 
						+ (stop.tv_usec - start.tv_usec);
					
					responses[res_cnt++] = res; 		// dodaj i zwieksz wskaznik

					// jesli odebrano echo reply to
					// jest to punkt docelowy czyli koniec
					if (res.type == ICMP_ECHOREPLY) dest = 1;
				} 
			}
			
		}

		// wypisz info na podstawie zebranych odpowiedzi
		print_info(responses,res_cnt);

	}

	close(sockfd);

    return 0;
}

_Bool is_valid_ip(char* ip) {

	int num = 0;             // wartosc liczbowa partu
	int digits_in_part = 0; // ile cyfr w parcie?
	_Bool begin_zero = 0;  // jesli 0 jest pierwsza cyfra, to nic po nim nie moze byc
	int parts = 1;        // ilosc partow (powinna wynosic 4)

	for (int i=0; ip[i]!='\0'; i++) {

		if (is_digit(ip[i])) {
			if (begin_zero) return 0; // zaczelo sie od zera a tu jakas kolejan cyfra
			if (ip[i] == '0' && digits_in_part == 0) begin_zero = 1; // pierwsza cyfra to zero
			digits_in_part++;
			num = num*10 + (ip[i]-'0');
		}
		
		else if (is_dot(ip[i])) {
			if (digits_in_part == 0) return 0; // kropka zanim jakas cyfra
			if (digits_in_part > 3) return 0; // za dlugie
			if (num > 255) return 0; // liczba, ktora jest bajtem > 255
			digits_in_part = 0;
			num = 0;
			begin_zero = 0;
			parts++;
		}
		
		else {
			return 0; // inny znak niz cyfra albo kropka
		}
	}

	// sprawdzamy ostatni (czwarty!) part
	if (digits_in_part == 0) return 0; // pusty bajt 4
	if (digits_in_part > 3) return 0; // za dlugie
	if (num > 255) return 0;         // liczba, ktora jest bajtem > 255
	if (parts != 4) return 0;       // ip ma miec 4 party

	return 1;

}
