CC= c99
LD_FLAGS= $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf)
CFLAGS= -g -O0 -Wall -Wextra -pedantic
CPPFLAGS= $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)

ONAME= historpg

OBJ= interface.o board.o

all: build

interface.o: interface.h board.h
board.o: board.h


build: $(OBJ)
	$(CC) $(OBJ) -o $(ONAME) $(LD_FLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -rf *.o $(ONAME)
