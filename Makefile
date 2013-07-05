#CURDIR=$(shell pwd)

CC=c99
CFLAGS=-Wall -I. -I./src -I./test
#LIBS=-lm
RM=rm -f
RMDIR=rm -rf

ODIR=obj
VPATH = src:test

.PHONY: test mkobjdir clean

DEPS = 	board.h 	 \
	chess.h 	 \
	cmd.h 		 \
	color.h		 \
	eng.h		 \
	fen.h		 \
	game.h		 \
	genmoves.h	 \
	hooche.h         \
	io.h		 \
	modinit.h	 \
	move.h		 \
	movestack.h	 \
	nodestack.h	 \
	piece.h		 \
	search.h	 \
	square.h	 \
	types.h		 \
	util.h
 
_OBJ = 	board.o         \
	cmd.o		\
	eng.o		\
	fen.o		\
	game.o		\
	genmoves.o	\
	io.o		\
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

_TESTOBJ = board_test.o

TESTOBJ = $(patsubst %,$(ODIR)/%,$(_TESTOBJ))


$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p obj
	$(CC) $(CFLAGS) $(DEFINES) -c -o $@ $<


hooche: main.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

hooche-test: DEFINES += -DUNITTEST
hooche-test: testmain.o $(TESTOBJ)
	$(CC) $(CFLAGS) -o $@ $^


.PHONY: test clean

test: hooche-test
	hooche-test

clean:
	$(RM) hooche
	$(RM) hooche-test
	$(RMDIR) $(ODIR)
