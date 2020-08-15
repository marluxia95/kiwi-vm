#OBJS specifies which files to compile as part of the project
OBJS = main.cpp asm.h asm.cpp cpu.cpp cpu.h configparser.cpp configparser.h

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -I. 
#`pkg-config --cflags --libs sdl2`

#LINKER_FLAGS specifies the libraries we're linking against
#LINKER_FLAGS = -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = kiwi_vm

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)
	#$(LINKER_FLAGS)
