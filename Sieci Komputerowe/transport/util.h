// Bartlomiej Grochowski 300 951

#ifndef UTIL_H
#define UTIL_H

_Bool is_valid_ip(char* ip);
inline _Bool is_digit(char c) {	return c >= 48 && c <= 57; }
inline _Bool is_dot(char c) { return c == 46; }
int validate_args(char* dest_ip, char* port, char* filesize, char* msg);

_Bool is_valid_port(char* port);
_Bool is_valid_filesize(char* filesize);

#endif // UTIL_H