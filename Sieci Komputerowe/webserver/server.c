// Bartlomiej Grochowski 300 951

#include "util.h"
#include "server.h"
#include "response.h"

#define ERROR(str) { fprintf(stderr, "%s: %s\n", str, strerror(errno)); return -1; }
#define PARROR(msg,str) { sprintf(msg,str); return -1; }
#define MAX(x,y) (x>y ? x : y)

struct server* create_server(int sockfd, int port) {
    struct server* s = malloc(sizeof(struct server));
    s->sockfd = sockfd; s->port = port;
    s->max_sockfd = sockfd;
    for (int i=0; i<MAX_CLIENTS; i++)
        s->clients[i] = (struct client){ 0,0 };
    return s;
}

// serwer nasluchuje w swoim gniezdzie i wszystkich klientow
int server_listen(struct server* s) {

    // szukamy wszystkie gniazda, na ktorych nasluchujemy funckja select
    // dodajemy gniazda do zbioru, jesli sa tacy uzytkownicy (sockfd > 0)

    fd_set descriptors; FD_ZERO (&descriptors);
    FD_SET (s->sockfd, &descriptors);
    struct timeval tv, now;
    tv.tv_sec = TIMEOUT; tv.tv_usec = 0;

    for (int i=0; i<MAX_CLIENTS; i++)
        if (s->clients[i].sockfd > 0)
            FD_SET(s->clients[i].sockfd, &descriptors);

    int ready = select( s->max_sockfd + 1, &descriptors, NULL, NULL, &tv );
    
    if (ready < 0) ERROR("select error")
    else if (ready == 0) disconnect_all(s); // timeout
    else { // got sth!

        // gniazdo serwera cos dostalo
        if (FD_ISSET(s->sockfd, &descriptors))
            new_connection(s);

        // moze gnizado klienta cos dostalo?
        for (int i=0; i<MAX_CLIENTS; i++) {

            if (FD_ISSET(s->clients[i].sockfd, &descriptors)) {
                gettimeofday(&now,NULL);
                s->clients[i].time = now.tv_sec;
                if (handle_request(s,s->clients[i].sockfd) <= 0)
                    disconnect(s,i);
            }

            else { // jesli timeout, to rozlacz to gniazdo
                gettimeofday(&now,NULL);
                if (s->clients[i].sockfd > 0
                    && s->clients[i].time > 0
                    && now.tv_sec - s->clients[i].time > TIMEOUT
                ) disconnect(s,i);
            } 
        }
    }

    return 1;
}

// odczytuje z gniazda sockfd request i go przetwarza
// zwraca info, czy utrzymywac polaczenie
int handle_request(struct server* s, int sockfd) {

    // przegladarka nawiazuje polaczenie po najechaniu na linka
    // jednak nie wysyla zadnego requesta (po 5s). powoduje to bledy
    // dlatego jesli przyszedl jakis request do tego gniazda,
    // ale pusty (len=0) to rozlacz to polaczenie

	char buffer[IP_MAXPACKET];
	ssize_t len = read(sockfd ,buffer, IP_MAXPACKET);

	if (len < 0) ERROR("recvfrom error");
    if (len == 0) return 0; // force disconnect!

    struct request req; char msg[STANDARD_SIZE];
    if (parse_request((char*)buffer,&req,msg) < 0)
        return answer(sockfd,501,"",(uint8_t*)msg,strlen(msg),1);

    printf("Request: %s %s %s\n",req.path,req.host,req.conn);
    return send_response(s,sockfd,&req);
}


int parse_request(char* buffer, struct request *req, char* msg) {

    // strstr - wskaznik na pierwsze wystapienie stringa
    // strchr - wskaznik na pierwsze wystapienie znaku
    // strswc - skip white characters
    // strsunwc - skip until not white character

    char* buffbeg; char* buffend; int n;
    
    // ----- path -----
    char path[STANDARD_SIZE] = {0};
    buffbeg = strstr(buffer,"GET");
    if (buffbeg == NULL) PARROR(msg,"GET not found");
    buffbeg = strchr(buffbeg,' ');
    buffbeg = strswc(buffbeg);
    buffend = strchr(buffbeg,' ');
    if (buffbeg == NULL || buffend == NULL)
        PARROR(msg,"Unknown error during parsing.");

    n = buffend - buffbeg;
    strncpy(path,buffbeg, n);
    req->path = path;
    
    // ----- host -----
    char host[STANDARD_SIZE] = {0};
    buffbeg = strstr(buffer,"Host:");
    if (buffbeg == NULL) PARROR(msg,"Host not found");
    buffbeg = strchr(buffbeg,' ');
    buffbeg = strswc(buffbeg);
    buffend = strchr(buffbeg,':'); // host:port
    if (buffbeg == NULL || buffend == NULL)
        PARROR(msg,"Unknown error during parsing.");

    n = buffend - buffbeg;
    strncpy(host,buffbeg, n);
    req->host = host;

    // ----- connection -----
    char conn[STANDARD_SIZE] = {0};
    buffbeg = strstr(buffer,"Connection:");
    if (buffbeg != NULL) {
        buffbeg = strchr(buffbeg,' ');
        buffbeg = strswc(buffbeg);
        buffend = strsunwc(buffbeg);
        if (buffbeg == NULL || buffend == NULL)
            PARROR(msg,"Unknown error during parsing.");
        n = buffend - buffbeg;
        strncpy(conn,buffbeg, n);
        req->conn = conn;
    } else conn[0] = '\0';

    return 1; // success

}

void disconnect_all(struct server* s) {
    for (int i=0; i<MAX_CLIENTS; i++) disconnect(s,i);
}

int disconnect(struct server* s, int i) {
    
    int sockfd = s->clients[i].sockfd;
    if (sockfd <= 0) return 0; // gniazdo nie nalezalo do nikogo
    
    s->clients[i] = (struct client){0,0};
    if (sockfd == s->max_sockfd)
        s->max_sockfd = find_max(
            (int*)s->clients,2,MAX_CLIENTS,s->sockfd);

    struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	
    // znajdz wlasciciela gniazda, jesli sie nie uda to 
    int know_source = getpeername(sockfd, (struct sockaddr*)&client_addr , &len);
    if (know_source < 0) printf("Disconnection[%d] > Unknown\n",i);
    else {
        printf("Disconnection[%d] > ip: %s, port: %d\n",
            i,inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port)
        );
    }

    // close socket
    if (close(sockfd) < 0)
        ERROR("close error");

    return 1;
}

// tworzy gniazdo powiazane z konkretnym portem
int create_socket(char* port) {
	
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) ERROR("socket error");

    int true = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
        &true, sizeof(true)) < 0) ERROR("setsockopt error");

	struct sockaddr_in server_address;
	bzero (&server_address, sizeof(server_address));
	server_address.sin_family      = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port        = htons(atoi(port));
	
    if (bind(sockfd, (struct sockaddr*)(&server_address),
        sizeof(server_address)) < 0
    ) ERROR("bind error");

    if (listen(sockfd, 64) < 0)
		ERROR("listen error");
	
    return sockfd;
}


int new_connection(struct server* s) {
    
    struct sockaddr_in client_address;
    socklen_t len = sizeof(client_address);
    int new_sockfd = accept(s->sockfd, (struct sockaddr*)&client_address, &len);
    if (new_sockfd < 0) ERROR("accept error");

    char ip_address[20];
    inet_ntop (AF_INET, &client_address.sin_addr, ip_address, sizeof(ip_address));

    _Bool found_free_slot = 0;
    for (int i=0; i<MAX_CLIENTS; i++)
        if (s->clients[i].sockfd == 0) {
            s->clients[i] = (struct client){new_sockfd,0};
            s->max_sockfd = MAX(s->max_sockfd,new_sockfd);
            printf("Connection[%d] > ip: %s port:%d\n",
                i, ip_address, ntohs(client_address.sin_port)
            ); found_free_slot = 1; break;
        }

    if (!found_free_slot) {
        printf("No free slot for > ip: %s port:%d\n",
            ip_address, ntohs(client_address.sin_port)
        );
        
        answer(new_sockfd,503,
            "Content-Type: text/html\n",
            (uint8_t*)"Server is busy.",15,
            0
        );

        close(new_sockfd);
        return 0;
    }

    return 1;
}