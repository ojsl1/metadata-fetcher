CC=g++
CXXFLAGS=-g -c -Wall $(INCLUDES)
LDFLAGS=$(shell pkg-config --libs sdl2) -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpng

INCLUDES=-Iinclude

ifeq ($(strip $(LDFLAGS)),)
	# fallback if pkg-config is not available or does not provide flags
	LDFLAGS=-lSDL2 # provide alternative linker flags
endif

run: game leanclean
	./game
	rm -f game

game: main.o render.o input.o sprite.o character.o font.o
	$(CC) main.o render.o input.o sprite.o character.o font.o -o game $(LDFLAGS)

main.o: main.cpp main.h
	$(CC) $(CXXFLAGS) main.cpp $(shell pkg-config --cflags sdl2)
   
render.o: render.cpp render.h
	$(CC) $(CXXFLAGS) render.cpp $(shell pkg-config --cflags sdl2)

input.o: input.cpp input.h
	$(CC) $(CXXFLAGS) input.cpp $(shell pkg-config --cflags sdl2)

sprite.o: sprite.cpp sprite.h
	$(CC) $(CXXFLAGS) sprite.cpp $(shell pkg-config --cflags sdl2)

character.o: character.cpp character.h
	$(CC) $(CXXFLAGS) character.cpp $(shell pkg-config --cflags sdl2)

font.o: font.cpp font.h
	$(CC) $(CXXFLAGS) font.cpp $(shell pkg-config --cflags sdl2)

setup:
	bash setup.sh

clean: 
	@echo "Cleaning everything..."
	rm -f game
	rm -f *.o
	rm -f *.gch

leanclean:
	@echo "Cleaning..."
	rm -f *.o
	rm -f *.gch

install:
	mkdir /tmp/first-sdl-game/
	cp game /tmp/first-sdl-game/game

uninstall:
	rm -rfv /tmp/first-sdl-game/
