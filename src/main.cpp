// ReSharper disable CppInconsistentNaming
#include <iostream>
#include <vector>

#define SDL_MAIN_USE_CALLBACKS 1

#include <thread>
#include <chrono>

#include "SDL.h"
#include "SDL_main.h"
#include "SDL_video.h"
#include "World.h"

using namespace Naito;
using namespace std::chrono;
using namespace std::chrono_literals;

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* texture;

static SDL_Surface* surface;

static constexpr unsigned int SCREEN_WIDTH = 256;
static constexpr unsigned int SCREEN_HEIGHT = 192;

time_point<steady_clock> start;
time_point<steady_clock> now;
time_point<steady_clock> prev;

unsigned int frameCount = 0;
double averageFrameRate = 0;

World* world;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialise SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Naito", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                SCREEN_HEIGHT);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    world = new World(SCREEN_WIDTH, SCREEN_HEIGHT);
    world->getCellGrid().setCell(10, 10, Cell{Element::Sand});

    start = steady_clock::now();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
        if (event->key.scancode == SDL_SCANCODE_ESCAPE)
            return SDL_APP_SUCCESS;

    default:
        break;
    }

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void* appstate) {
    time_point<steady_clock> frameStart;
    prev = now;

    SDL_PumpEvents();
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    if (keyboardState[SDL_SCANCODE_SPACE])
        world->getCellGrid().setCell(10, 10, Cell{Element::Sand});

    // Update world
    world->getCellGrid().update();
    world->drawCells(surface, texture, renderer);

    // Update displayed frame
    SDL_RenderPresent(renderer);

    frameCount++;

    std::this_thread::sleep_for(16.6ms);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    now = steady_clock::now();
    averageFrameRate = static_cast<double>(frameCount) / static_cast<double>((now - start).count()) * 1'000'000'000;
    std::cout << "Average framerate: " << averageFrameRate << " FPS\n";
}
