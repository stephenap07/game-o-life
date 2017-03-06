#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Endianess check for SDL RGBA surfaces
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#endif

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_DEPTH 32

// The size in pixels of a single cell
const unsigned int MAP_SIZE = 8;
const unsigned int MAP_WIDTH = SCREEN_WIDTH / MAP_SIZE;
const unsigned int MAP_HEIGHT = SCREEN_HEIGHT / MAP_SIZE;

Uint8 *getFirstByteFromSurfacePixels(SDL_Surface *surface, int x, int y) {
  return (Uint8 *)surface->pixels + y * surface->pitch +
         x * surface->format->BytesPerPixel;
}

void assign16BitPixelToSurfacePixelByByteOrder(Uint8 *p, Uint32 pixel) {
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
    p[0] = (pixel >> 16) & 0xff;
    p[1] = (pixel >> 8) & 0xff;
    p[2] = pixel & 0xff;
  } else {
    p[0] = pixel & 0xff;
    p[1] = (pixel >> 8) & 0xff;
    p[2] = (pixel >> 16) & 0xff;
  }
}

void assignPixelToSurface(SDL_Surface *surface, int x, int y, Uint32 pixel) {
  Uint8 *p = getFirstByteFromSurfacePixels(surface, x, y);

  switch (surface->format->BytesPerPixel) {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(Uint16 *)p = pixel;
    break;
  case 3:
    assign16BitPixelToSurfacePixelByByteOrder(p, pixel);
    break;
  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}

class GridDrawer {
public:
  GridDrawer(SDL_Surface *s, Uint32 c, int sz)
      : screen(s), color(c), size(sz) {}

  SDL_Texture *drawToTexture(SDL_Renderer *renderer) {
    drawWhiteBackground();
    drawRows();
    drawColumns();
    return SDL_CreateTextureFromSurface(renderer, screen);
  }

private:
  SDL_Surface *screen;
  Uint32 color;
  int size;

  void drawWhiteBackground() {
    SDL_FillRect(screen, nullptr,
                 SDL_MapRGBA(screen->format, 0xFF, 0xFF, 0xFF, 0));
  }

  void drawRows() {
    for (int y = 0; y < screen->h; y++) {
      for (int x = 0; x < screen->w; x++) {
        if (x % size == 0)
          assignPixelToSurface(screen, x, y, color);
      }
    }
  }

  void drawColumns() {
    for (int x = 0; x < screen->w; x++) {
      for (int y = 0; y < screen->h; y++) {
        if (y % size == 0)
          assignPixelToSurface(screen, x, y, color);
      }
    }
  }
};

class CellMap {
private:
  struct Cell {

    enum { ALIVE = 0xFF000000, DEAD = 0xFFFFFFFF };

    bool active;
    Uint32 color;

    void setState(Uint32 state) { color = state; }

    void setToActiveBasedOnState() { active = color == ALIVE ? true : false; }

    void makeAlive() {
      active = true;
      color = ALIVE;
    }

    void makeDead() {
      active = false;
      color = DEAD;
    }
  };

  std::size_t width;
  std::size_t height;
  std::vector<Cell> cells;

  Cell &getCell(int x, int y) { return cells[y * width + x]; }
  const Cell &getCell(int x, int y) const { return cells[y * width + x]; }

public:
  CellMap(int w, int h) : width(w), height(h), cells(w * h) {}
  CellMap() : width(MAP_WIDTH), height(MAP_HEIGHT), cells(MAP_WIDTH * MAP_HEIGHT) {}

  void applyRules() {
    performOneGeneration();
    updateLiveCellsToActiveState();
  }

  bool clearCellsAndReadCellsFromFile(const std::string &fileName) {
    deactivateCellsAndSetAllToDead();
    if (!readCellsFromFile(fileName))
      return false;
    return true;
  }

  SDL_Texture* createCellTexture(SDL_Renderer *renderer) {
    return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                             SDL_TEXTUREACCESS_STREAMING, width, height);
  }

  SDL_Surface *createCellSurface() {
    return SDL_CreateRGBSurfaceFrom(nullptr, width, height, 32, 0, RMASK, GMASK,
                                    BMASK, AMASK);
  }

  void renderCellSurfaceToTexture(SDL_Texture *cellTex,
                                  SDL_Surface *cellSurface) {
    SDL_LockTexture(cellTex, nullptr, &cellSurface->pixels,
                    &cellSurface->pitch);
    SDL_FillRect(cellSurface, nullptr,
                 SDL_MapRGBA(cellSurface->format, 0xFF, 0xFF, 0xFF, 0));
    assignPixelToSurfaceForEveryLiveCell(cellSurface);
    SDL_UnlockTexture(cellTex);
  }

private:

  void assignPixelToSurfaceForEveryLiveCell(SDL_Surface *surface) {
    for (unsigned int y = 0; y < height; y++) {
      for (unsigned int x = 0; x < width; x++) {
        if (getCell(x, y).color == Cell::ALIVE) {
          assignPixelToSurface(surface, x, y, getCell(x, y).color);
        }
      }
    }
  }

  /// Checks every cell for neighbors and applies the following rules:
  /// - Any live cell with fewer than two live neighbours dies, as if caused by
  /// under-population.
  /// - Any live cell with two or three live neighbours lives on to the next
  /// generation.
  /// - Any live cell with more than three live neighbours dies, as if by
  /// overcrowding.
  /// - Any dead cell with exactly three live neighbours becomes a live cell, as
  /// if by reproduction.
  void performOneGeneration() {
    for (size_t y = 0; y < height; y++) {
      for (size_t x = 0; x < width; x++) {
        getCell(x, y).setState(
            determineCellState(x, y, getActiveNeighbors(x, y)));
      }
    }
  }

  void updateLiveCellsToActiveState() {
    for (unsigned int y = 0; y < height; y++) {
      for (unsigned int x = 0; x < width; x++) {
        getCell(x, y).setToActiveBasedOnState();
      }
    }
  }

  int getBottomRowActiveNeighbors(int x, int y) const {
    int neighbors(0);
    if (y < (height - 1)) {
      if (getCell(x, y + 1).active)
        neighbors++;
      if (x < (width- 1))
        if (getCell(x + 1, y + 1).active)
          neighbors++;
      if (x > 0)
        if (getCell(x - 1, y + 1).active)
          neighbors++;
    }
    return neighbors;
  }

  int getTopRowActiveNeighbors(int x, int y) const {
    int neighbors(0);
    if (y > 0) {
      if (getCell(x, y - 1).active)
        neighbors++;
      if (x < (width- 1))
        if (getCell(x + 1, y - 1).active)
          neighbors++;
      if (x > 0)
        if (getCell(x - 1, y - 1).active)
          neighbors++;
    }
    return neighbors;
  }

  int getMiddleRowActiveNeighbors(int x, int y) const {
    int neighbors(0);
    if (x < (width - 1))
      if (getCell(x + 1, y).active)
        neighbors++;
    if (x > 0)
      if (getCell(x - 1, y).active)
        neighbors++;
    return neighbors;
  }

  int getActiveNeighbors(int x, int y) const {
    int neighbors = 0;
    neighbors += getTopRowActiveNeighbors(x, y);
    neighbors += getMiddleRowActiveNeighbors(x, y);
    neighbors += getBottomRowActiveNeighbors(x, y);
    return neighbors;
  }

  Uint32 determineCellState(int x, int y, int neighbors) const {
    if (getCell(x, y).active && neighbors < 2)
      return Cell::DEAD;
    else if (getCell(x, y).active && neighbors > 3)
      return Cell::DEAD;
    else if (!getCell(x, y).active && neighbors == 3)
      return Cell::ALIVE;
    return getCell(x, y).color;
  }

  bool readCellsFromFile(const std::string &fileName) {
    std::ifstream seedFile;
    seedFile.open(fileName);
    if (seedFile.is_open()) {
      parseCellsFromStream(seedFile);
    } else {
      std::cerr << "Error opening file " << fileName << '\n';
      return false;
    }
    seedFile.close();
    return true;
  }

  void deactivateCellsAndSetAllToDead() {
    for (unsigned int y = 0; y < height; y++) {
      for (unsigned int x = 0; x < width; x++) {
        getCell(x, y).makeDead();
      }
    }
  }

  bool isXYCoordinatesOutOfMapBoundary(int x, int y) const {
    if (y >= height) {
      std::cerr << "Map out of bounds" << std::endl;
      return false;
    }
    if (x >= width) {
      std::cerr << "Map out of bounds" << std::endl;
      return false;
    }
    return true;
  }

  bool parseCellRowFromString(const std::string &line, int row) {
    size_t column = 0;
    for (char c : line) {
      if (!isXYCoordinatesOutOfMapBoundary(row, column))
        return false;
      if (c == 'O')
        getCell(column, row).makeAlive();
      column++;
    }
    return true;
  }

  bool parseCellsFromStream(std::ifstream &seedFile) {
    unsigned int row = 0;
    std::string line;
    while (getline(seedFile, line)) {
      if (!parseCellRowFromString(line, row++))
        return false;
    }
    return true;
  }
};

class CellRenderer {
private:
  SDL_Texture *cellTex = nullptr;
  SDL_Surface *cellSurface = nullptr;

public:
  ~CellRenderer() {
    SDL_DestroyTexture(cellTex);
    SDL_FreeSurface(cellSurface);
  }

  void render(SDL_Renderer *renderer, CellMap &cellMap) {
    renderCellSurfaceToTexture(cellMap);
    SDL_RenderCopy(renderer, cellTex, nullptr, nullptr);
  }

  bool createSurfaceAndTexture(SDL_Renderer *renderer, CellMap &cellMap) {
    if (!(cellSurface = cellMap.createCellSurface()))
      return false;
    if (!(cellTex = cellMap.createCellTexture(renderer)))
      return false;
    return true;
  }

private:
  void renderCellSurfaceToTexture(CellMap &cellMap) {
    cellMap.renderCellSurfaceToTexture(cellTex, cellSurface);
  }
};

class GridRenderer {
private:
  SDL_Texture *gridTex = nullptr;
  SDL_Surface *gridSurface = nullptr;

public:
  ~GridRenderer() {
    SDL_DestroyTexture(gridTex);
    SDL_FreeSurface(gridSurface);
  }

  void render(SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, gridTex, nullptr, nullptr);
  }

  bool createSurfaceAndTexture(SDL_Renderer *renderer, std::size_t width,
                               std::size_t height) {
    if (!createGridSurface(width, height))
      return false;
    if (!createGridTexture(renderer))
      return false;
    return true;
  }

private:
  bool createGridSurface(std::size_t width, std::size_t height) {
    gridSurface =
        SDL_CreateRGBSurface(0, width, height, 32, RMASK, GMASK, BMASK, AMASK);
    if (!gridSurface) {
      std::cerr << SDL_GetError() << std::endl;
      return false;
    }
    return true;
  }

  bool createGridTexture(SDL_Renderer *renderer) {
    GridDrawer gridDrawer(gridSurface, 0xFFA9A9A9, MAP_SIZE);
    gridTex = gridDrawer.drawToTexture(renderer);
    if (!gridTex) {
      std::cerr << SDL_GetError() << std::endl;
      return false;
    }
    return true;
  }
};

class CellAndGridRenderer {
private:
  CellRenderer cellRenderer;
  GridRenderer gridRenderer;

public:
  bool createSurfacesAndTexturesForCellsAndGrid(SDL_Renderer *renderer,
                                                CellMap &cellMap) {
    if (!cellRenderer.createSurfaceAndTexture(renderer, cellMap))
      return false;
    if (!gridRenderer.createSurfaceAndTexture(renderer, SCREEN_WIDTH,
                                              SCREEN_HEIGHT))
      return false;
    return true;
  }

  void drawCellsAndGrid(SDL_Renderer *renderer, CellMap &cellMap) {
    cellRenderer.render(renderer, cellMap);
    gridRenderer.render(renderer);
  }
};

class GameOfLife {
private:
  CellMap cellMap;
  CellAndGridRenderer cellAndGridRenderer;

  SDL_Window *win = nullptr;
  SDL_Renderer *renderer = nullptr;

  unsigned long ticks = 0, delta = 0;
  unsigned long lastTick = 0;

  bool quit = false;
  SDL_Event event;

  bool initializeSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
      std::cerr << SDL_GetError() << std::endl;
      SDL_Quit();
      return false;
    }
    if (TTF_Init() != 0) {
      std::cerr << SDL_GetError() << std::endl;
      SDL_Quit();
      return false;
    }
    return true;
  }

  bool initializeWindow() {
    win = SDL_CreateWindow("Game-o-Life", SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                           SDL_WINDOW_SHOWN);
    if (win == nullptr) {
      std::cerr << SDL_GetError() << std::endl;
      return false;
    }

    return true;
  }

  bool initializeRenderer() {
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |
                                               SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
      std::cerr << SDL_GetError() << std::endl;
      return false;
    }
    return true;
  }

  bool initializeRenderSystem() {
    if (!initializeSDL())
      return false;
    if (!initializeWindow())
      return false;
    if (!initializeRenderer())
      return false;
    return true;
  }

  bool initializeCells() { return true; }

  void createDeltaAndIncrementTicks() {
    delta = SDL_GetTicks() - lastTick;
    lastTick = SDL_GetTicks();
    ticks += delta;
  }

  void pollForEvents() {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          quit = true;
          break;
        }
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_n:
          cellMap.applyRules();
          break;
        }
        break;
      case SDL_QUIT:
        quit = true;
      }
    }
  }

  bool initialize() {
    if (!initializeRenderSystem())
      return false;
    if (!cellMap.clearCellsAndReadCellsFromFile("seed"))
      return false;
    cellAndGridRenderer.createSurfacesAndTexturesForCellsAndGrid(renderer,
                                                                 cellMap);
    return true;
  }

  void mainLoop() {
    while (!quit) {
      pollForEvents();
      createDeltaAndIncrementTicks();
      SDL_RenderClear(renderer);
      cellAndGridRenderer.drawCellsAndGrid(renderer, cellMap);
      SDL_RenderPresent(renderer);
    }
  }

  void destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
  }

public:

  GameOfLife() : cellMap(MAP_WIDTH, MAP_HEIGHT) {}

  int runGame() {
    if (!initialize())
      return 1;
    mainLoop();
    destroy();
    return 0;
  }
};

int main(int argc, char *argv[]) { return GameOfLife().runGame(); }
