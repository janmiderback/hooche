#CURDIR=$(shell pwd)

VPATH = src:test:obj
CC=c99
CFLAGS=-Wall -I. -I./src -I./test
#LIBS=-lm
RM=rm -f
RMDIR=rm -rf
ODIR=obj

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p obj
	$(CC) $(CFLAGS) $(DEFINES) -c -o $@ $<

DEPS = 	board.h 	 \
	chess.h 	 \
	cmd.h 		 \
	color.h		 \
	eng.h		 \
	fen.h		 \
	game.h		 \
	genmoves.h	 \
	hooche.h     \
	io.h		 \
	modinit.h	 \
	move.h		 \
	movestack.h	 \
	nodestack.h	 \
	piece.h		 \
	search.h	 \
	types.h		 \
	util.h
 
_OBJ = board.o \
	cmd.o		\
	eng.o		\
	fen.o		\
	game.o		\
	genmoves.o	\
	io.o		\
	main.o      \
	modinit.o	\
	move.o		\
	movestack.o	\
	nodestack.o	\
	piece.o		\
	search.o	\
	uci.o		\
	util.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TESTOBJ = board_test.o \
	testmain.o

TESTOBJ = $(patsubst %,$(ODIR)/%,$(_TESTOBJ))

hooche: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

hooche-test: DEFINES += -DUNITTEST
hooche-test: $(TESTOBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: test
test: hooche-test
	hooche-test

.PHONY: clean
clean:
	$(RM) hooche
	$(RM) hooche-test
	$(RMDIR) $(ODIR)
