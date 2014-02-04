EXE=life
CC=g++
INCLUDES=-IGWork/gwen/include/
LDFLAGS=-LGWork/gwen/lib -LGWork/gwen/lib/macosx/ -LGWork/gwen/lib/macosx/gmake/
LIBS= -lgwen_static -lGWEN-Renderer-SDL2 -lunittest -framework Cocoa -framework IOKit -framework OpenGL -framework ForceFeedback -framework CoreAudio -framework AudioUnit -framework AudioToolbox -framework Carbon -liconv  -lSDL2 -lSDL2_image -lSDL2_ttf 
CFLAGS=-Wall --std=c++11 -stdlib=libc++ ${INCLUDES} $(sdl2-config --cflags) ${LDFLAGS} ${LIBS}

.PHONY: all debug run

all:
	g++ main.cpp ${CFLAGS} -O3 -o $(EXE)
debug:
	g++ main.cpp ${CFLAGS} -g -o $(EXE)
run:
	./$(EXE)
