SRC_PATH=./src/
CC=gcc
LL=g++
COMPILER_FLAGS = -g -c
#LINKER_FLAGS = `sdl2-config --cflags --libs` -llua -lm
#LINKER_FLAGS = `sdl2-config --cflags --libs` -lGL -lm -lGLU -lGLEW
LINKER_FLAGS = -lm -lmanifoldc -lmanifold -lClipper2
#INCLUDE_PATHS = -I. -I./src/ -I/usr/include/SDL2
INCLUDE_PATHS = -I. -I./src/ -I./include
#LIBRARY_PATHS = -L/usr/lib/ -L.
LIBRARY_PATHS = -L/usr/lib/ -L. -L./lib

EXE=teste_manifold.exe

SRC=$(wildcard $(SRC_PATH)*.c)
OBJ=$(subst ./src, ./obj, $(SRC:.c=.o))

all: $(SRC) $(EXE)

$(EXE): $(OBJ)
	$(LL) $(LIBRARY_PATHS) $(LINKER_FLAGS) $(OBJ) $(LINKER_FLAGS) -o $@

./obj/%.o: ./src/%.c
	$(CC) $(INCLUDE_PATHS) $(COMPILER_FLAGS) -o $@ $<

clean:
	rm -rf run $(OBJ)
