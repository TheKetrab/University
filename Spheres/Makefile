CC = gcc

CFLAGS = -Wall			 	\
         -lm                            \
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED

FILES = src/spheres.h \
        src/game.c \
        src/main.c \
        src/system.c

Spheres: src/main.c
	$(CC) $(FILES) -o Spheres $(CFLAGS) `pkg-config gtk+-3.0 --cflags --libs`

clean: 
	rm -f *.o Spheres
