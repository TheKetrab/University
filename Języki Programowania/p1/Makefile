# Bartłomiej Grochowski 300951
# Makefile

PROG = prog
CC = ocamlc
EXTERNALS = str.cma
OBJS =  Util.cmi Util.cmo Lambda.cmi Lambda.cmo DeBruijn.cmi DeBruijn.cmo Parser.cmo Main.cmo
FILES = Util.mli Util.ml  Lambda.mli Lambda.ml  DeBruijn.mli DeBruijn.ml  Parser.ml  Main.ml

all: $(OBJS)

$(OBJS):
	$(CC) $(EXTERNALS) $(FILES) -o $(PROG)

clean:
	rm -f *.cmo *.cmi $(PROG)