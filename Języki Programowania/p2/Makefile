# Bartłomiej Grochowski 300951
# Makefile

CC = ocamlc
OBJS =  Lambda.cmi Lambda.cmo
FILES = Lambda.mli Lambda.ml

all: $(OBJS)

$(OBJS):
	$(CC) $(FILES)

clean:
	rm -f *.cmo *.cmi