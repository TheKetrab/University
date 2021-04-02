# Bartlomiej Grochowski 300 951

PROG    = webserver
OBJ     = util.o server.o response.o main.o
CC      = gcc
CFLAGS  = -std=gnu99 -Wall -Wextra -Wshadow -O2
LDFLAGS = -lm

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(PROG) $(LDFLAGS)

clean:
	rm -rf *.o

distclean:
	rm -rf *.o webserver .*.swp

zip:
	tar --xz -cf prog.tar.xz main.c response.c response.h server.c server.h util.c util.h Makefile