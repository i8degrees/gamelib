WORKING_DIR := $(shell pwd)
CC := /usr/bin/clang++
EMCC = ~/local/src/emscripten/emcc
EMCC_CFLAGS = --jcache

CFLAGS = -std=c++11 -stdlib=libc++
DEBUG_CFLAGS := $(CFLAGS) -gfull -O0 -Wall
RELEASE_CFLAGS := $(CFLAGS) -O2
LIB_CFLAGS = -fPIC
SDL_CFLAGS := $(shell pkg-config --cflags sdl SDL_image SDL_ttf SDL_mixer)

LIB_LDFLAGS = -shared
SDL_LDFLAGS := $(shell pkg-config --libs sdl SDL_image SDL_ttf SDL_mixer)

SRC = src/audio.cpp src/font.cpp src/gfx.cpp src/sprite.cpp src/timer.cpp \
src/fps.cpp

OBJ = build/audio.o build/font.o build/gfx.o build/sprite.o build/timer.o \
build/fps.o

TARGET_OBJ = libgamelib.bc
TARGET_LIB = libgamelib.dylib
TARGET_DIR = gamelib
PREFIX = /usr/local

all: libgamelib web

libgamelib: $(OBJ)
	$(CC) $(CFLAGS) $(LIB_CFLAGS) $(SDL_CFLAGS) $(LIB_LDFLAGS) $(SDL_LDFLAGS) $(OBJ) -o $(TARGET_LIB)

bin: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS) $(OBJ) -o $(TARGET)
web:
	$(EMCC) $(EMCC_CFLAGS) $(DEBUG_CFLAGS) $(SRC) -o $(TARGET_OBJ)

build/audio.o: src/audio.cpp src/audio.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/audio.cpp -o build/audio.o

build/font.o: src/font.cpp src/font.h src/gfx.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/font.cpp -o build/font.o

build/gfx.o: src/gfx.cpp src/gfx.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/gfx.cpp -o build/gfx.o

build/sprite.o: src/sprite.cpp src/sprite.h src/gfx.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/sprite.cpp -o build/sprite.o

build/timer.o: src/timer.cpp src/timer.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/timer.cpp -o build/timer.o

build/fps.o: src/timer.cpp src/timer.h src/fps.cpp src/fps.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/fps.cpp -o build/fps.o

clean:
	/bin/rm -rf *.o *.dylib build/* $(TARGET_OBJ) $(TARGET_LIB)

reinstall: install
install: install-lib
deinstall: uninstall

install-lib:
	/bin/ln -sf $(WORKING_DIR)/$(TARGET_LIB) $(PREFIX)/lib; /bin/mkdir -p $(PREFIX)/include/$(TARGET_DIR);

	/bin/ln -sf $(WORKING_DIR)/src/audio.h $(PREFIX)/include/$(TARGET_DIR)/audio.h;
	/bin/ln -sf $(WORKING_DIR)/src/font.h $(PREFIX)/include/$(TARGET_DIR)/font.h;
	/bin/ln -sf $(WORKING_DIR)/src/fps.h $(PREFIX)/include/$(TARGET_DIR)/fps.h;
	/bin/ln -sf $(WORKING_DIR)/src/gamelib.h $(PREFIX)/include/$(TARGET_DIR)/gamelib.h;
	/bin/ln -sf $(WORKING_DIR)/src/gfx.h $(PREFIX)/include/$(TARGET_DIR)/gfx.h;
	/bin/ln -sf $(WORKING_DIR)/src/sprite.h $(PREFIX)/include/$(TARGET_DIR)/sprite.h;
	/bin/ln -sf $(WORKING_DIR)/src/timer.h $(PREFIX)/include/$(TARGET_DIR)/timer.h;

uninstall:
	/bin/rm $(PREFIX)/lib/$(TARGET_LIB);

analyze:
	scan-build -k --use-c++=/usr/bin/clang++ /usr/bin/clang++ -c $(CFLAGS) $(SDL_CFLAGS) $(SRC)

sublime-cpp:
	subl src/sprite.cpp src/gfx.cpp src/font.cpp src/audio.cpp src/timer.cpp src/fps.cpp
sublime-h:
	subl src/sprite.h src/gfx.h src/font.h src/audio.h src/timer.h src/fps.h

# DO NOT DELETE
