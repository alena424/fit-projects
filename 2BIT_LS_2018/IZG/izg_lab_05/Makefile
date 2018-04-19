################################################################################
# IZG lab #05, framework compilation
################################################################################

################################################################################
# build variables

ifeq ($(OS), Windows_NT)
  INCS = -I./SDL-1.2.15/include -IQ:/mingw/SDL/SDL
  LIBS = -lmingw32 -lSDLmain -lSDL -L./SDL-1.2.15/lib -LQ:/mingw/SDL #-mwindows
  REMOVE = del
  BIN = izg_lab_05.exe
else
  INCS = -I/usr/include/SDL
  LIBS=-lSDL
  REMOVE = rm -f
  BIN = izg_lab_05
endif

CPP  = g++
CC   = gcc

CXXINCS =
CXXFLAGS = $(INCS) -g3 -Wall $(CXXINCS)
CFLAGS = $(INCS)  -fmessage-length=0

################################################################################
# build target
all: $(BIN)

################################################################################
# linking target file
$(BIN):	obj/main.o obj/student.o obj/transform.o obj/model.o
	$(CPP) obj/main.o obj/student.o obj/transform.o obj/model.o -o $(BIN) $(LIBS)
#	strip $(BIN)

################################################################################
# building main file
obj/main.o: src/main.cpp src/main.h
	$(CPP) -c src/main.cpp -o obj/main.o $(CXXFLAGS)

################################################################################
# building students file
obj/student.o: src/student.cpp src/main.h
	$(CPP) -c src/student.cpp -o obj/student.o $(CXXFLAGS)

################################################################################
# building other files
obj/transform.o: src/transform.cpp src/transform.h
	$(CPP) -c src/transform.cpp -o obj/transform.o $(CXXFLAGS)
obj/model.o: src/model.cpp src/model.h
	$(CPP) -c src/model.cpp -o obj/model.o $(CXXFLAGS)

################################################################################
# clean built files
clean: clear
clear:
ifeq ($(OS), Windows_NT)
	$(REMOVE) obj\*.o $(BIN)
else
	$(REMOVE) obj/*.o $(BIN)
endif

################################################################################
################################################################################

