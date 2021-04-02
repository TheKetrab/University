# Bartlomiej Grochowski 300 951

PROG    = transporter
OBJ     = util.o transporter.o main.o
CC      = gcc
CFLAGS  = -std=gnu99 -Wall -Wextra -Wshadow -O2

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(PROG)

clean:
	rm -rf *.o

distclean:
	rm -rf *.o transporter .*.swp

zip:
	tar --xz -cf prog.tar.xz transporter.h transporter.c main.c util.h util.c Makefile