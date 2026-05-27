CC=g++
CXXFLAGS=-g -c -Wall $(INCLUDES)
LDFLAGS=$(shell pkg-config --libs sdl2) -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpng

#-I/usr/include/SDL3 \
#-I/usr/include/SDL3/SDL3_image \
#-I/usr/include/SDL3/SDL3_mixer \
#-I/usr/include/SDL3/SDL3_ttf

INCLUDES=-Iinclude
SRC_DIR=src
BUILD_DIR=build

OBJS = $(BUILD_DIR)/main.o \
       $(BUILD_DIR)/render.o \
       $(BUILD_DIR)/util.o \
       $(BUILD_DIR)/menu.o \
       $(BUILD_DIR)/input.o \
       $(BUILD_DIR)/sprite.o \
       $(BUILD_DIR)/character.o \
       $(BUILD_DIR)/font.o

ifeq ($(strip $(LDFLAGS)),)
	# fallback if pkg-config is not available or does not provide flags
	LDFLAGS=-lSDL2 # provide alternative linker flags
endif

run: $(BUILD_DIR) all leanclean
	./game

all: $(BUILD_DIR) $(OBJS)
	$(CC) $(OBJS) -o game $(LDFLAGS)

## -- compile rules --
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $(CXXFLAGS) $(SRC_DIR)/main.cpp $(shell pkg-config --cflags sdl2) -o $@

$(BUILD_DIR)/render.o: $(SRC_DIR)/render.cpp
	$(CC) $(CXXFLAGS) $(SRC_DIR)/render.cpp $(shell pkg-config --cflags sdl2) -o $@

$(BUILD_DIR)/util.o: $(SRC_DIR)/util.cpp
	$(CC) $(CXXFLAGS) $(SRC_DIR)/util.cpp $(shell pkg-config --cflags sdl2) -o $@

$(BUILD_DIR)/menu.o: $(SRC_DIR)/menu.cpp
	$(CC) $(CXXFLAGS) $(SRC_DIR)/menu.cpp $(shell pkg-config --cflags sdl2) -o $@

$(BUILD_DIR)/input.o: $(SRC_DIR)/input.cpp
	$(CC) $(CXXFLAGS) $(SRC_DIR)/input.cpp $(shell pkg-config --cflags sdl2) -o $@

$(BUILD_DIR)/sprite.o: $(SRC_DIR)/sprite.cpp
	$(CC) $(CXXFLAGS) $(SRC_DIR)/sprite.cpp $(shell pkg-config --cflags sdl2) -o $@

$(BUILD_DIR)/character.o: $(SRC_DIR)/character.cpp
	$(CC) $(CXXFLAGS) $(SRC_DIR)/character.cpp $(shell pkg-config --cflags sdl2) -o $@

$(BUILD_DIR)/font.o: $(SRC_DIR)/font.cpp
	$(CC) $(CXXFLAGS) $(SRC_DIR)/font.cpp $(shell pkg-config --cflags sdl2) -o $@

## -- setup build dir --
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

## -- utilities --
setup:
	bash setup.sh

clean: 
	@echo "Cleaning everything..."
	rm -rf $(BUILD_DIR)
	rm -f game

leanclean:
	@echo "Cleaning objects only..."
	rm -rf $(BUILD_DIR)

install:
	mkdir -p /tmp/first-sdl-game/
	cp game /tmp/first-sdl-game/game

uninstall:
	rm -rfv /tmp/first-sdl-game/
