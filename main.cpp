#include <iostream>
#include <fstream>
#include <string>
#include <array>

#include "SDL2/SDL.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_DEPTH  32

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

// The size in pixels of a single cell
const unsigned int MAP_SIZE = 8;

const unsigned int MAP_WIDTH = SCREEN_WIDTH / MAP_SIZE;
const unsigned int MAP_HEIGHT = SCREEN_HEIGHT / MAP_SIZE;

template <class T, size_t ROW, size_t COL>
using Map = std::array<std::array<T, COL>, ROW>;


/*
 * A cell has two states - alive & dead
 */
enum {
    ALIVE = 0xFF000000,
    DEAD = 0xFFFFFFFF
};

/*
 * A cell is active when it was alive the previous generation
 * It becomes inactive in the generation after it dies
 */
struct cell
{
    bool active; 
    Uint32 color;
};


/**
 * Checks every cell for neighbors and applies the following rules:
 * 
 * - Any live cell with fewer than two live neighbours dies, as if caused by under-population.
 * - Any live cell with two or three live neighbours lives on to the next generation.
 * - Any live cell with more than three live neighbours dies, as if by overcrowding.
 * - Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
 */
template <size_t ROWS, size_t COLS>
static inline void apply_rules(Map<cell, ROWS, COLS> & cells)
{
    for (size_t y = 0; y < ROWS; y++) {
        for (size_t x = 0; x < COLS; x++) {
            int neighbors = 0;

            if (y < (ROWS - 1)) {
                if (cells[y + 1][x].active)
                    neighbors++;
                if (x < (COLS - 1))
                    if (cells[y + 1][x + 1].active)
                        neighbors++;
                if (x > 0)
                    if (cells[y + 1][x - 1].active)
                        neighbors++;
            }
            if (y > 0) {
                if (cells[y - 1][x].active)
                    neighbors++;
                if (x < (COLS - 1))
                    if (cells[y - 1][x + 1].active)
                        neighbors++;
                if (x > 0)
                    if (cells[y - 1][x - 1].active)
                        neighbors++;
            }
            if (x < (COLS - 1))
                if (cells[y][x + 1].active)
                    neighbors++;
            if (x > 0)
                if (cells[y][x - 1].active)
                    neighbors++;

            if (cells[y][x].active && neighbors < 2)
                cells[y][x].color = DEAD;
            else if (cells[y][x].active && neighbors > 3)
                cells[y][x].color = DEAD;
            else if (!cells[y][x].active && neighbors == 3)
                cells[y][x].color = ALIVE;
        }
    }

    for (unsigned int y = 0; y < ROWS; y++) {
        for (unsigned int x = 0; x < COLS; x++) {
            if (cells[y][x].color == ALIVE)
                cells[y][x].active = true;
            else
                cells[y][x].active = false;
        }
    }
}


/**
 * Figures out the endianess and applies the pixel color to the surface
 */
static inline void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}


/**
 * Draws a grid to the surface by drawing horizontal and vertical lines with each
 * cell with a width and height of 'size'
 */
static inline void draw_grid(SDL_Surface *screen, Uint32 color, int size) {
    for(int y = 0; y < screen->h; y++) {
        for (int x = 0; x < screen->w; x++) {
            if (x % size == 0)
                put_pixel(screen, x, y, color);
        }     
    }

    for(int x = 0; x < screen->w; x++) {
        for (int y = 0; y < screen->h; y++) {
            if (y % size == 0)
                put_pixel(screen, x, y, color);
        }     
    }
}


int main(int argc, char *argv[]) {

    SDL_Texture *cell_tex = nullptr;
    SDL_Surface *cell_surface = nullptr;
    SDL_Texture *grid_tex = nullptr;
    SDL_Surface *grid_surface = nullptr;

    unsigned int cell_x = 0, cell_y = 0;
    unsigned long ticks = 0, delta = 0;
    unsigned long last_tick = SDL_GetTicks(); 

    bool quit = false;
    SDL_Event event;

    Map<cell, MAP_HEIGHT, MAP_WIDTH> cells;

    std::ifstream seed_file;
    std::string line;

    for (unsigned int y = 0; y < MAP_HEIGHT; y++) {
        for (unsigned int x = 0; x < MAP_WIDTH; x++) {
            cells[y][x].active = false;
            cells[y][x].color = DEAD;
        }
    }

    /*
     * Open a file called 'seed' and read O's as an active alive cell into
     * the cell grid. Ignore all other characters like '.'
     */
    seed_file.open("seed");
    if (seed_file.is_open()) {
        while ( getline (seed_file, line) ) {
            if (cell_y >= MAP_HEIGHT) {
                std::cerr << "Map out of bounds" << std::endl;
                return 1;
            }

            if (cell_x >= MAP_WIDTH) {
                std::cerr << "Map out of bounds" << std::endl;
                return 1;
            }

            for (char c : line) {
                cell_x++;
                if (c == 'O') {
                    cells[cell_y][cell_x].active = true; 
                    cells[cell_y][cell_x].color = ALIVE; 
                }
            }
            cell_y++;
            cell_x = 0;
        }
    }
    else {
        std::cerr << "Error opening file 'seed'" << std::endl;
        return 1;
    }

    seed_file.close();

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window *win = nullptr;
    win = SDL_CreateWindow("Game-o-Life", SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                           SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (win == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        return 1;

    }

    SDL_Renderer *renderer = nullptr;
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    // This is a 'streaming texture' since it gets updated frequently
    cell_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           MAP_WIDTH,
                                           MAP_HEIGHT);
    if (cell_tex == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    cell_surface = SDL_CreateRGBSurfaceFrom(nullptr, MAP_WIDTH,
                                                  MAP_HEIGHT,
                                                  32, 0,
                                                  RMASK,
                                                  GMASK,
                                                  BMASK,
                                                  AMASK);
    if (cell_surface == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    grid_surface = SDL_CreateRGBSurface(0,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        32,
                                        RMASK,
                                        GMASK,
                                        BMASK,
                                        AMASK);
    if (grid_surface == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    // Fill the grid with a transparent color and draw grid lines
    SDL_FillRect(grid_surface, nullptr,
                 SDL_MapRGBA(grid_surface->format, 0xFF, 0xFF, 0xFF, 0));
    draw_grid(grid_surface, 0xFFA9A9A9, MAP_SIZE);
    grid_tex = SDL_CreateTextureFromSurface(renderer, grid_surface);

    if (grid_tex == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        return 1;
    }

    while (!quit) {
        delta = SDL_GetTicks() - last_tick;
        last_tick = SDL_GetTicks();
        ticks += delta;

        /*
         * Poll for events, and handle the ones we care about.
         * When a user presses 'n', it handles a single step of a
         * game of life generation
         */
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
                case SDL_KEYUP:                  
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                    }
                    break;
                case SDL_KEYDOWN:                  
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_n:
                            apply_rules<MAP_HEIGHT, MAP_WIDTH>(cells);
                            break;
                    }
                    break;

                case SDL_QUIT:
                    return(0);
            }
        }

        SDL_LockTexture(cell_tex, nullptr, &cell_surface->pixels, &cell_surface->pitch);
        SDL_FillRect(cell_surface, nullptr,
                     SDL_MapRGBA(cell_surface->format, 0xFF, 0xFF, 0xFF, 0));

        for (unsigned int y = 0; y < MAP_HEIGHT; y++) {
            for (unsigned int x = 0; x < MAP_WIDTH; x++) {
                if (cells[y][x].color == ALIVE) {
                    put_pixel(cell_surface, x, y, cells[y][x].color);
                }
            }
        }

        SDL_UnlockTexture(cell_tex);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, cell_tex, nullptr, nullptr);
        SDL_RenderCopy(renderer, grid_tex, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    
    SDL_DestroyTexture(cell_tex);
    SDL_DestroyTexture(grid_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
