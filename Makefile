EXE=life
CC=g++
CFLAGS=-Wall --std=c++11 `sdl2-config --cflags --libs`
all:
	g++ main.cpp ${CFLAGS} -O3 -o $(EXE)
debug:
	g++ main.cpp ${CFLAGS} -g -o $(EXE)
run:
	./$(EXE)
