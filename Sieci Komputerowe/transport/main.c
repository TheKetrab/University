// Bartlomiej Grochowski 300 951

/**
 * TRANSPORT
 * 
 * Struktura transporter przetrzymuje okno przesuwne.
 * Sa to dwie tablice, jedna z przeslanymi znakami, druga z info o tym,
 * ktore pakiety z ramki (z przyszlosci) juz przyszly. Przyklad:
 * 
 * real_start = 732
 * start = 7                      v
 * index     0  1  2  3  4  5  6  7  8  9  10 11
 * received  T  F  F  F  T  F  F  F  T  T  F  F
 * dane     [D][A][T][A][D][A][T][A][D][A][T][A]
 * 
 * Czyli w tej sytuacji czekamy na pakiet pod indeksem 732, chociaz w naszej tablicy
 * ma to numer 7. Wszystkie dane z wczesniejszych pakietow sa juz zapisane w pliku wynikowym.
 * Tutaj tez widzimy, ze dostalismy np. pakiet 741 (indeks 4).
 * Gdy dostaniemy pakiet 732, przesuwamy ramke do pierwszego pakietu, ktorego nie dostalismy.
 * W tej sytuacji bedzie to 735 (indeks 10). Pakiety 732, 733, 734 wpisujemy do pliku wyjsciowego.
 * Teraz ramka obejmuje pakiety: 735-746, tj indeksy 10,11,0,1,2,...,9, a indeksy received 7,8,9 = F
 * 
 * Uwaga: tablica received to w rzeczywistosci nie T/F, tylko ilosc otrzymanych bajtow
 * W tej implementacji niekazdy pakiet ma swoj wlasny timeout, dlatego serwer jest troche zasypywany requestami...
 */

/**
 * SREDNIA Z PIECIU POMIAROW
 * 
 * MBI: /usr/bin/time -v /usr/local/bin/transport-client-fast ADDRESS PORT file1 FILESIZE
 * BGR: /usr/bin/time -v ./transporter ADDRESS PORT file2 FILESIZE
 * 
 *           |         MBI          |          BGR          | Stosunek BGR/MBI
 *  FILESIZE |  TIME         MEM    |  TIME          MEM    |  TIME      MEM
 * --------- | -------------------- | --------------------- | -----------------
 *    15 000 | 1,276 s  , 1510.4 KB | 1.812 s   , 1512.8 KB |  1.42      1.00 
 *           |                      |                       |
 * 1 000 000 | 5.754 s  , 1868 KB   | 12.014 s  , 1798.4 KB |  2.09      0.96
 *           |                      |                       |
 * 9 000 000 | 34.212 s , 2994.4 KB | 118.074 s , 1800 KB   |  3.45      0.60
 * 
 * Czyli program miesci sie w 4t+5 i nie przekracza 5MB.
 */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "transporter.h"
#include "util.h"
#define PERCENT_INFO 1

int main(int argc, char* argv[]) {

    if (argc != 5) {
		    fprintf(stderr, "usage: ./transport destIP port filename filesize\n"); 
		    return EXIT_FAILURE;
    }

    char msg[100];
    if (validate_args(argv[1],argv[2],argv[4],&msg[0]) < 0) {
        fprintf(stderr, "arguments are incorrect: %s\n", msg);
        return EXIT_FAILURE;
    }

    char* dest_ip = argv[1];
    int port = atoi(argv[2]);
    char* filename = argv[3];
    int filesize = atoi(argv[4]);

  	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (sockfd < 0) {
        fprintf(stderr, "socket error: %s\n", strerror(errno)); 
        return EXIT_FAILURE;
    }

    FILE* file = fopen(filename,"wb+");
    if (file == NULL) {
		  fprintf(stderr, "fail to open file: %s\n", filename);
	    return EXIT_FAILURE;
    }

    struct transporter* t = create_transporter(sockfd,dest_ip,port);
    if (get_file(t,file,filesize,PERCENT_INFO) < 0) return EXIT_FAILURE;
    fclose(file);

    return 0;
}
