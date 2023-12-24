.PHONY: all debug release clean

RELEASE_FLAGS = -O2 - Wall - DNDEBUG
DEBUG_FLAGS = -g - O0 - Wall
RELEASE_EXEC = task1
DEBUG_EXEC = main - dbg
SOURCE = main.cpp

all : debug release

debug : $(DEBUG_EXEC)

$(DEBUG_EXEC) : main.cpp
g++ $(DEBUG_FLAGS) main.cpp - o $(DEBUG_EXEC) - lcryptopp

release : $(RELEASE_EXEC)

$(RELEASE_EXEC) : main.cpp
g++ $(RELEASE_FLAGS) main.cpp - o $(RELEASE_EXEC) - lcryptopp

clean :
rm - f $(RELEASE_EXEC) $(DEBUG_EXEC)