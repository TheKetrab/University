// Bartlomiej Grochowski 300 951

/**
 * ROUTER
 * 
 * Program przetrzymuje tablice routingu i wymienia ja z sasiadami.
 * Argument podany na standardowe wejscie to plik z konfiguracja bezposrednich sieci.
 * Na poczatek inicjujemy gniazdo na porcie PORT (54321), potem wysylamy swoja tablice routingu sasiadom.
 * Czekamy (zasypiajac - select) na wpisy od innych przez czas ROUND_TIME (5s).
 * Jesli pojawi sie problem zliczania do nieskonczonosci, to wpis uwazamy za nieosiagalny jesli dist > MAX_DIST (16).
 * 
 * Jesli wpis nie byl uaktualniany przez 3 rundy, to uwazamy siec za nieosiagalna
 * > jesli przyjdzie wpis od kogos innego, to aktualizujemy wpis
 * > jesli nikt nic nie przysle o tej nieosiagalnej sieci przez kolejne
 *   3 tury, to wpis o sieci zostaje usuniety z tablicy routingu
 * 
 * Jesli ktoras z maszyn padnie, to trasy nie beda przez nia przechodzic, ale sieci beda dalej osiagalne
 * np: A--B--C, B pada, siec AB dalej dostepna dla A, bo A wysyla broadcast
 * do sieci AB i dostaje odpowiedz sam od siebie - tak samo C i siec BC
 * 
 * Jesli padnie karta sieciowa A prowadzaca do AB, to broadcast wyslany do sieci AB
 * nie przyjdzie z powrotem do A -> po 3 turach wpis bedzie unreachable i jesli mamy polaczenie np:
 * Ax--B---C---A (x = zerwane), to C przysle wpis o sieci AB i bedzie AB reachable via C,
 * ale nadal A bedzie wysylalo broadcasty do AB, bo byc moze odzyska polaczenie.
 * 
 * Jesli padnie karta sieciowa A do AB i B do AB, to mamy np: Ax-xB---C---A
 * to wtedy siec AB jest nieosiagalna dla wszystkich (C po pewnym czasie usunie wpis o tej sieci)
 * A i B beda mialy wpisy 'AB unreachable connected directly'
 * 
 */

#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

#include "router.h"


int main(int argc, char* argv[]) {

    if (argc != 2) {
		printf("Run program with FILE argument. (eg. ./router configA)\n");
		return EXIT_FAILURE;
	}

    // input
    struct router* r = malloc(sizeof(struct router));
    r->first_entry = NULL;
    r->last_entry = NULL;

    if (read_web_from_file(r,argv[1]) < 0) {
        return EXIT_FAILURE;
    }

    // konfigureacja gniazda
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

    int broadcast = 1;
    if (setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast)) < 0) {
        fprintf(stderr, "socket error (opt BROADCAST): %s\n", strerror(errno)); 
		return EXIT_FAILURE;
    }

    r->sockfd = sockfd;

	struct sockaddr_in router_address;
	bzero (&router_address, sizeof(router_address));
	router_address.sin_family      = AF_INET;
	router_address.sin_port        = htons(PORT);
	router_address.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind (sockfd, (struct sockaddr*)&router_address, sizeof(router_address)) < 0) {
		fprintf(stderr, "bind error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}

    // program
    for (int i=0 ; ; i++) {

        // sprawdz, czy nie ma starych wpisow
        check_table(r,i);

        // wypisz info
        printf("\n ----- ----- ----- ----- \n");
        printf("Round %d:\n",i);
        print_entries(r,0);

        // wyslij tablice routingu
        share_route_table(r,1,NULL);

		// select properties
		fd_set descriptors;
		FD_ZERO (&descriptors);
		FD_SET (sockfd, &descriptors);
		struct timeval tv; tv.tv_sec = ROUND_TIME; tv.tv_usec = 0;

        // nasluchuj inne routery na ich tablice
        while(1) {
            int ready = select( sockfd + 1, &descriptors, NULL, NULL, &tv );
			if (ready < 0) { fprintf(stderr,"error in fun 'select'\n"); return EXIT_FAILURE; }
			else if (ready == 0) break; // timeout
            else get_and_update(r,i);
        }

    }

	close (sockfd);

    return EXIT_SUCCESS;

}

