################################################################################
# IZG project, framework compilation
################################################################################

################################################################################
# promenne prekladu

ifeq ($(OS), Windows_NT)
    INCS = -I../SDL-1.2.15/include -IQ:/mingw/SDL/SDL
    LIBS = -lmingw32 -lSDLmain -lSDL -L../SDL-1.2.15/lib -LQ:/mingw/SDL
#    REMOVE = del
    REMOVE = rm -f
    BIN = izg_lab_02.exe
else
    INCS = -I/usr/include/SDL
    LIBS = -lSDL
#    LIBS=-m32 -lSDL
#    LIBS=-m64 -lSDL
    REMOVE = rm -f
    BIN = izg_lab_02
endif

CC = g++

CFLAGS = -g3 -Wall $(INCS) -Wno-unused-function -fmessage-length=0
#CFLAGS = -m32 -g3 -Wall $(INCS) -Wno-unused-function -fmessage-length=0
#CFLAGS = -m64 -g3 -Wall $(INCS) -Wno-unused-function -fmessage-length=0

################################################################################
# obecny cil kompilace
all: $(BIN)

################################################################################
# linkovani vysledneho souboru
$(BIN):	main.o student.o io.o color.o
	$(CC) main.o student.o io.o color.o -o $(BIN) $(LIBS)
#	strip $(BIN)

################################################################################
# kompilace hlavniho souboru
main.o: main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)

################################################################################
# kompilace studentskeho souboru
student.o: student.cpp student.h
	$(CC) -c student.cpp -o student.o $(CFLAGS)

################################################################################
# kompilace dilcich souboru
io.o: io.cpp io.h
	$(CC) -c io.cpp -o io.o $(CFLAGS)
color.o: color.cpp color.h
	$(CC) -c color.cpp -o color.o $(CFLAGS)

################################################################################
# cil vycisteni souboru prekladu
clean: clear
clear:
	$(REMOVE) *.o $(BIN)

################################################################################
################################################################################
