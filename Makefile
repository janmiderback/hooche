CC=gcc
CFLAGS=-Wall -I.
#LIBS=-lm
RM=rm

ODIR=obj
VPATH = src:test

DEPS = 	board.h 	\
	chess.h 	\
	cmd.h 		\
	color.h		\
	eng.h		\
	fen.h		\
	game.h		\
	genmoves.h	\
	hooce.h		\
	io.h		\
	modinit.h	\
	move.h		\
	movestack.h	\
	nodestack.h	\
	piece.h		\
	search.h	\
	square.h	\
	types.h		\
	util.h
 
_OBJ = 	board.o		\
	cmd.o		\
	eng.o		\
	fen.o		\
	game.o		\
	genmoves.o	\
	io.o		\
	main.o		\
	modinit.o	\
	move.o		\
	movestack.o	\
	nodestack.o	\
	piece.o		\
	search.o	\
	square.o	\
	uci.o		\
	util.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $<

hooche: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

hooche-test:


.PHONY: test clean

test:

clean:
	$(RM) -f hooche
	$(RM) -rf $(ODIR)

