#OBJS specifies which files to compile as part of the project
OBJS = main.cpp cpu.cpp mmu.cpp  key.cpp hasgb.cpp timer.cpp gpu.cpp audio.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
#INCLUDE_PATHS = -Ic:/usr/local/include/

#LIBRARY_PATHS specifies the additional library paths we'll need
#LIBRARY_PATHS = -Lc/usr/local/lib/

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w
#-w -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2main -lSDL2 
#-lSDL2_image -lSDL2_ttf -lSDL2_mixer

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = HasGB

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)