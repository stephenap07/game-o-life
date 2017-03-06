ifndef verbose
  SILENT = @
endif

EXE=life
CXX=g++
INCLUDES=`sdl2-config --cflags`
LIBS=`sdl2-config --libs` -lSDL2_image -lSDL2_ttf
CFLAGS=-g -Wall --std=c++11 -stdlib=libc++
CFLAGS+=-msse -O2 -ffast-math

.PHONY: all debug run

all:
	$(CXX) main.cpp ${CFLAGS} ${INCLUDES} ${LIBS} -O3 -o $(EXE)
debug:
	$(CXX) main.cpp ${CFLAGS} ${INCLUDES} ${LIBS} -g -o $(EXE)

run:
	./$(EXE)
