################################################################################
# IZG project, framework compilation
################################################################################

################################################################################
# promenne prekladu

ifeq ($(OS), Windows_NT)
    INCS = -I../include -IQ:/mingw/SDL/
    LIBS = -lmingw32 -lSDLmain -lSDL -L../lib -LQ:/mingw/SDL
#    REMOVE = del
    REMOVE = rm -f
    BIN = izg_lab_03.exe
else
    INCS = -I/usr/include/SDL
    LIBS = -lSDL
#    LIBS=-m32 -lSDL
#    LIBS=-m64 -lSDL
    REMOVE = rm -f
    BIN = izg_lab_03
endif

CXX = g++

CFLAGS = -g3 -Wall $(INCS) -Wno-unused-function -fmessage-length=0
#CFLAGS = -m32 -g3 -Wall $(INCS) -Wno-unused-function -fmessage-length=0
#CFLAGS = -m64 -g3 -Wall $(INCS) -Wno-unused-function -fmessage-length=0

################################################################################
# obecny cil kompilace
all: $(BIN)

################################################################################
# linkovani vysledneho souboru
$(BIN):	obj/main.o obj/student.o obj/io.o obj/color.o
	$(CXX) obj/main.o obj/student.o obj/io.o obj/color.o -o $(BIN) $(LIBS)
#	strip $(BIN)

################################################################################
# kompilace hlavniho souboru
obj/main.o: src/main.cpp
	$(CXX) -c src/main.cpp -o obj/main.o $(CFLAGS)

################################################################################
# kompilace studentskeho souboru
obj/student.o: src/student.cpp src/student.h
	$(CXX) -c src/student.cpp -o obj/student.o $(CFLAGS)

################################################################################
# kompilace dilcich souboru
obj/io.o: src/io.cpp src/io.h
	$(CXX) -c src/io.cpp -o obj/io.o $(CFLAGS)
obj/color.o: src/color.cpp src/color.h
	$(CXX) -c src/color.cpp -o obj/color.o $(CFLAGS)

################################################################################
# cil vycisteni souboru prekladu
clean: clear
clear:
	$(REMOVE) obj/*.o bin/$(BIN)

################################################################################
################################################################################
