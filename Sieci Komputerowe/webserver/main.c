// Bartlomiej Grochowski 300 951

// WEBSERVER
// w /etc/hosts dopisac: localhost 127.0.0.1
// w folderze, ktory wskazujemy jako sciezke powinien byc folder localhost
// w folderze localhost index.html (lub inne pliki)

// program na konsole wypisuje:
// 1. Nawiazanie polaczenia
// 2. Zamkniecie polaczenia
// 3. Sparsowany http_request wyslany do serwera
// 4. Odeslany http_response (naglowki)

// dzialanie:
// * serwer slucha
// * jesli ktos zacznie nawiazywac polaczenie, to przydzielone mu przez system
//   gniazdo zostaje dodane do listy polaczonych klientow
// * polaczenie jest zamykane, gdy:
//     - klient sobie tego zazyczy (Connection != keep-alive)
//     - wystapi jakis blad
//     - wystapi timeout (przez X sekund nie przyjdzie nic do gniazda tego polaczenia)
// * kazdy request jest obslugiwany w funkcji handle_request
//     - odczytywany z gniazda (nalezacego do danego klienta) bufor bajtow jest parsowany
//       na strukture struct request, a nastepnie podawany do modulu 'response'
//     - send_response ma zadecydowac co odeslac na podstawie struktury request
//     - odsylany jest answer z odpowiednim kodem HTML, naglowkami i danymi
//     - funkcje te zwracaja informacje o tym, czy utrzymywac polaczenie - jesli nie,
//       to w funkcji server_listen zostanie to odnotowane i klient zostanie rozlaczony

#include "util.h"
#include "server.h"

int main(int argc, char* argv[]) {

    // EXAMPLE USAGE:
    // ./webserver 8888 webpages/

    if (argc != 3) {
		printf("Run program: ./webserver PORT PATH_TO_HOSTS\n");
		return EXIT_FAILURE;
	}

    int port; char msg[1000] = {0};
    if (!valid_input(argv[1],argv[2],&port,msg)) {
		printf("Invalid input arguments: %s\n",msg);
		return EXIT_FAILURE;
    }

    int sockfd = create_socket(argv[1]);
	if (sockfd < 0) return EXIT_FAILURE;

    struct server* s = create_server(sockfd,port);
    s->respath = argv[2];

    while(1) server_listen(s);
    
	close (sockfd);
    return EXIT_SUCCESS;
}
