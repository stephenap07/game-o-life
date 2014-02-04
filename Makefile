ifndef verbose
  SILENT = @
endif

EXE=life
CC=g++
CXX=g++
INCLUDES=-IGWork/gwen/include/ -Iinclude
LDFLAGS=-LGWork/gwen/lib -LGWork/gwen/lib/macosx/ -LGWork/gwen/lib/macosx/gmake/ -L./
LIBS= -lgwen_static -lGWEN-Renderer-SDL2 -framework Cocoa -framework IOKit -framework OpenGL -framework ForceFeedback -framework CoreAudio -framework AudioUnit -framework AudioToolbox -framework Carbon -liconv  -lSDL2 -lSDL2_image -lSDL2_ttf -llifegui
CFLAGS=-Wall --std=c++11 -stdlib=libc++ ${INCLUDES} $(sdl2-config --cflags) ${LDFLAGS} ${LIBS}
CFLAGS += -msse -O2 -ffast-math

.PHONY: all debug run gui

all: gui
	g++ main.cpp ${CFLAGS} -O3 -o $(EXE)
debug: gui
	g++ main.cpp ${CFLAGS} -g -o $(EXE)

gui: ${OBJECTS}
	@echo "==== Building Life Gui library ===="
	make --no-print-directory -C . -f LifeGUI.make

run:
	./$(EXE)
