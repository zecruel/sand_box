SRC_PATH = ./harfbuzz_core/
EXT_SRC_PATH = ./ext_src/
INCLUDE_PATHS = $(SRC_PATH) $(EXT_SRC_PATH) $(dir $(wildcard $(EXT_SRC_PATH)*/) $(wildcard $(EXT_SRC_PATH)*/include/))
CC = gcc
CPP = g++
LINKER = ar
#COMPILER_FLAGS = -g -c
CPP_COMPILER_FLAGS = -c -DMANIFOLD_PAR=-1 -DMANIFOLD_CROSS_SECTION  -DHB_NO_DRAW -DHB_NO_COLOR -DHB_NO_OUTLINE -DHB_NO_PAINT
C_COMPILER_FLAGS = -g -c -MD -DSQLITE_THREADSAFE=0 -DSQLITE_OMIT_LOAD_EXTENSION -DGLEW_NO_GLU -DGLEW_STATIC
#LINKER_FLAGS = -static-libgcc -static-libstdc++ -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lopengl32 -lm
LINKER_FLAGS = rcs
INCLUDE = $(patsubst %,-I%,. /ucrt64/include/ /ucrt64/include/SDL2/ $(INCLUDE_PATHS))
LIBRARY_PATHS = -L/ucrt64/lib/ -L.
EXE=lib_harf.a

SRC_C=$(wildcard $(SRC_PATH)*.c) $(wildcard $(EXT_SRC_PATH)*/*.c)
SRC_CPP=$(wildcard $(SRC_PATH)*.cc) $(wildcard $(EXT_SRC_PATH)*/*.cc)
OBJ_C=$(SRC_C:.c=.o)
OBJ_CPP=$(SRC_CPP:.cc=.obj)
DEP_C=$(OBJ_C:.o=.d)

all: $(SRC_C) $(SRC_CPP) $(EXE)
	echo "test"

$(EXE): $(OBJ_CPP) $(OBJ_C)
	#$(LINKER) $(LIBRARY_PATHS) $(LINKER_FLAGS) $(OBJ_C) $(OBJ_CPP) $(LINKER_FLAGS) cz.res -o $@
	$(LINKER) $(LINKER_FLAGS) $(EXE) $(OBJ_C) $(OBJ_CPP)

$(OBJ_C):
	$(CC) $(INCLUDE) $(C_COMPILER_FLAGS) -o $@ $(@:.o=.c)

$(OBJ_CPP):
	$(CPP) $(INCLUDE) $(CPP_COMPILER_FLAGS) -o $@ $(@:.obj=.cc)

clean:
	rm -rf run $(OBJ_C)
	rm -rf run $(OBJ_CPP)
	rm -rf run $(DEP_C)

-include $(DEP_C)
