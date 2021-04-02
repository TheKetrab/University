# Bartlomiej Grochowski 300 951

PROG    = traceroute
OBJ     = receive.o send.o main.o
CC      = gcc
CFLAGS  = -std=gnu99 -Wall -Wextra -Wshadow -O2

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(PROG)

clean:
	rm -rf *.o

distclean:
	rm -rf *.o traceroute .*.swp