// Bartlomiej Grochowski 300 951

#include "util.h"

#define CR 13
#define LF 10

// skip white characters
char* strswc(char* buffer) {
    char* buffptr = buffer;
    while(*buffptr == ' ' || *buffptr == CR || *buffptr == LF || *buffptr == '\t') buffptr++;
    return buffptr;
}

// skip until white character
char* strsunwc(char* buffer) {
    char* buffptr = buffer;
    while(!(*buffptr == ' ' || *buffptr == CR || *buffptr == LF || *buffptr == '\t' || *buffptr == '\0')) buffptr++;
    return buffptr;
}

// znajduje maximum w tablicy buff, step - co ile elementow
int find_max(int* buff, int step, int n, int cur_max) {
    for (int i=0; i<n; i++) {
        cur_max = *buff > cur_max ? *buff : cur_max;
        buff += step;
    } return cur_max;
}

// zwraca ilosc przeczytanych bajtow
int readfile(const char* filename, ssize_t filesize, uint8_t* buffer) {
    
    FILE* file = fopen(filename,"rb");
    if (file == NULL) return -1;

    int len = fread(buffer,1,filesize,file);
    
    fclose(file);
    return len;
}

// zwraca rozszerzenie (bez kropki) (np. prog.exe -> exe)
int get_extension(const char* filename, char* buffer) {
    char* ptr = strrchr(filename,'.'); // last '.'
    if (ptr == NULL) return -1;
    ptr++; int i=0;
    for (; ptr[i] != '\0'; i++) buffer[i] = ptr[i];
    return i;
}

// int -> string
char* itos(int n) {
    int len = log10((double)n) + 1;
    char* str = malloc(len+1); str[len] = '\0';
    for (int i=len-1; i>=0; i--) {
        str[i] = '0' + n%10; n/=10;
    } return str;
}

// funkcja przekopiowania z zadania: transport
_Bool is_valid_port(char* port) {
	int port_value = 0;
	for (int i=0; port[i]!='\0'; i++) {
		if (!is_digit(port[i])) return 0;
		port_value = (port_value*10) + (port[i] - '0');
	} if (port_value >= 65536) return 0; else return 1;
}

_Bool is_valid_path(char* path) {
    DIR* dir = opendir(path);
    if (dir == NULL) return 0;
    return 1;
}

_Bool valid_input(char* port_in, char* path, int* port_out, char* msg) {

    if (!is_valid_port(port_in)) { 
        sprintf(msg,"Invalid port: %s",port_in); return 0; 
    } else *port_out = atoi(port_in);

    if (!is_valid_path(path)) {
        sprintf(msg,"Invalid path: %s",path); return 0;
    } else return 1;
}
