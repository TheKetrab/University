// Bartlomiej Grochowski 300 951

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

char* strswc(char* buffer);
char* strsunwc(char* buffer);
int find_max(int* buff, int step, int n, int cur_max);
int readfile(const char* filename, ssize_t filesize, uint8_t* buffer);
int get_extension(const char* filename, char* buffer);
char* itos(int n);

inline _Bool is_digit(char c) {	return c >= 48 && c <= 57; }
_Bool valid_input(char* port_in, char* path, int* port_out, char* msg);
_Bool is_valid_port(char* port);
_Bool is_valid_path(char* path);


#endif // UTIL_H
