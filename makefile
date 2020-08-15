
OBJS = main.cpp asm.h asm.cpp cpu.cpp cpu.h configparser.cpp configparser.h


CC = g++


COMPILER_FLAGS = -w -I. 
#`pkg-config --cflags --libs sdl2`

#LINKER_FLAGS = -lSDL2

OBJ_NAME = kiwi_vm

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)
	#$(LINKER_FLAGS)
