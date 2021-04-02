// Bartlomiej Grochowski 300 951

#include <stdio.h>
#include "util.h"

// funkcja z zadania traceroute
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

_Bool is_valid_port(char* port) {
	int port_value = 0;
	for (int i=0; port[i]!='\0'; i++) {
		if (!is_digit(port[i])) return 0;
		port_value = (port_value*10) + (port[i] - '0');
	} if (port_value >= 65536) return 0; else return 1;
}

_Bool is_valid_filesize(char* filesize) {
	int filesize_value = 0;
	for (int i=0; filesize[i]!='\0'; i++) {
		if (!is_digit(filesize[i])) return 0;
		filesize_value = (filesize_value*10) + (filesize[i] - '0');
	} return 1;
}

int validate_args(char* dest_ip, char* port, char* filesize, char* msg) {
	// jesli filename jest zle, to bedzie blad przy otwarciu pliku fopen NULL
	if (!is_valid_ip(dest_ip)) { sprintf(msg,"Invalid IP address: %s",dest_ip); return -1; }
	if (!is_valid_port(port)) { sprintf(msg,"Invalid port: %s",port); return -2; }
	if (!is_valid_filesize(filesize)) { sprintf(msg,"Invalid filesize: %s",filesize); return -3; }
	return 1;
}