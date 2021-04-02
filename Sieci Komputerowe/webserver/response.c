// Bartlomiej Grochowski 300 951

#include "response.h"
#include "util.h"

#define ERROR(str) { fprintf(stderr, "%s: %s\n", str, strerror(errno)); return -1; }

char* rescode_description(int rescode) {
    switch (rescode) {
        case 200: return "OK";
        case 301: return "Moved Permanently";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 501: return "Not Implemented";
        case 503: return "Service Unavailable";
        default: return "Unknown";
    }
}

// txt, html, css, jpg, jpeg, png, pdf
char* mime_type(char* extension) {
    if (!strcmp(extension,"txt"))  return "text/html";
    if (!strcmp(extension,"html")) return "text/html";
    if (!strcmp(extension,"css"))  return "text/css";
    if (!strcmp(extension,"jpg"))  return "image/jpg";
    if (!strcmp(extension,"jpeg")) return "image/jpeg";
    if (!strcmp(extension,"png"))  return "image/png";
    if (!strcmp(extension,"pdf"))  return "application/pdf";
    return "application/octet-stream";
}

// sprawdza czy na serwerze instnieje takie host, czyli FOLDER z taka nazwa
int host_is_valid(const char* path, const char* host_name) {
    DIR* dir = opendir(path);
    for (struct dirent* entry = readdir(dir); entry; entry = readdir(dir)) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name,host_name) == 0)
             { closedir(dir); return 1; }
    } closedir(dir); return 0;
}

// wysyla przez dane gniazdo konkretna odpowiedz
int answer(int sockfd, int rescode, char* headers, uint8_t* data, ssize_t data_len, int keep_conn) {

    char* description = rescode_description(rescode);
    char* response = malloc(RESPONSE_MAX); int pos = 0;
    pos = sprintf(response + pos, "HTTP/1.1 %d %s\n",rescode,description);
    pos = sprintf(response + pos,"%s\n",headers); // 1 linia odstepu po naglowkach

    printf("Response: \n---\n%s",response);
    if (data_len < 50) printf("%s",data);
    else printf("CONTENT (%ld bytes)",data_len);
    printf("\n---\n");


    int response_len = strlen(response);
    int result_len = response_len + data_len;
    uint8_t result[result_len];
    memcpy(result,response,response_len);
    memcpy(result+response_len,data,data_len);

    if (send(sockfd,result,result_len,0) < 0)
        ERROR("send error");

    return keep_conn;
}

// funkcja podejmuje decyzje, jaka odpowiedz HTTP wyslac,
// zwraca 1, jesli utrzymujemy polaczenie,
// 0 jesli zamykamy,
// -1 jesli blad
int send_response(struct server* s, int sockfd, struct request* r) {

    int keep_conn = 0;
    if (r->conn != NULL && !strcmp(r->conn,"keep-alive"))
        keep_conn = 1;

    if (!host_is_valid(s->respath,r->host))
        return answer(sockfd,403,
            "Content-Type: text/html\n",
            (uint8_t*)"Invalid host name.",18,
            keep_conn
        );

    char filepath[1000];
    strcpy(filepath,s->respath);
    strcat(filepath,r->host);
    strcat(filepath,r->path);

	// czy plik istnieje?
	struct stat file_stat;
    if (stat(filepath, &file_stat) < 0)
        return answer(sockfd,404,
            "Content-Type: text/html\n",
            (uint8_t*)"File not found!",15,
            keep_conn
        );

    // czy w zapytaniu jest '../'?
	if (strstr(filepath,"../") != NULL)
        return answer(sockfd,403,
            "Content-Type: text/html\n",
            (uint8_t*)"Forbidden operation.",20,
            keep_conn
        );

    // czy zadany zasob to folder?
    if (S_ISDIR(file_stat.st_mode)) {
        // -> przekierowanie do strony index.html
        char headers[RESPONSE_MAX]; sprintf(headers,
            "Location: http://%s:%s%s/index.html\n",r->host,itos(s->port),r->path
        ); return answer(sockfd,301,headers,NULL,0,keep_conn);
    }

    // czy zadany zasob to plik?
    if (S_ISREG(file_stat.st_mode)) {

        uint8_t filestream[RESPONSE_MAX] = {0};
        int len = readfile(filepath,file_stat.st_size,filestream);
        if (len < 0) return answer(sockfd,501,
            "Content-Type: text/html\n",
            (uint8_t*)"File failed to load.",20,
            keep_conn
        );

        char extension[100] = {0};
        get_extension(filepath,extension);
        
        char headers[RESPONSE_MAX]; sprintf(headers,
            "Content-Type: %s\nContent-Length: %d\n",
            mime_type(extension),len
        );

        return answer(sockfd,200,headers,filestream,len,keep_conn);
    }

    // ELSE
    return answer(sockfd,501,
        "Content-Type: text/html\n",
        (uint8_t*)"Unknown operation.",18,
        keep_conn
    );

}
