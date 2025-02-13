#include <iostream>
#include <vector>

#define SDL_MAIN_USE_CALLBACKS 1
#define SDL_HINT_RENDER_VSYNC 1

#include <thread>
#include <chrono>

#include "SDL.h"
#include "SDL_main.h"
#include "SDL_video.h"

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

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialise SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Naito", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

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
        return SDL_APP_CONTINUE;
    }

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void* appstate) {
    time_point<steady_clock> frameStart;
    prev = now;
    // Update displayed frame
    //SDL_SetRenderDrawColorFloat(renderer, 1, 0, 1, SDL_ALPHA_OPAQUE_FLOAT);

    SDL_LockTextureToSurface(texture, nullptr, &surface);

    auto* pixels = (Uint32*)surface->pixels;
    const auto formatDetails = SDL_GetPixelFormatDetails(surface->format);

    for (int x = 0; x < SCREEN_WIDTH; ++x) {
        for (int y = SCREEN_HEIGHT - 1; y >= 0; --y) {
            const int offset = x + surface->pitch / sizeof(Uint32) * y;
            *(pixels + offset) = SDL_MapRGBA(formatDetails, nullptr, 20, 40, 60, SDL_ALPHA_OPAQUE);
        }
    }

    SDL_UnlockTexture(texture);


    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);

    SDL_RenderPresent(renderer);

    now = steady_clock::now();
    duration frameTime = now - prev;
    double frameRate = 1'000'000'000.0 / duration_cast<nanoseconds>(frameTime).count();
    averageFrameRate = (double)frameCount / (double)(now - start).count() * 1'000'000'000;

    //std::cout << "Framerate: " << frameRate << " FPS, average: " << averageFrameRate << " FPS\n";

    frameCount++;
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    std::cout << "Average framerate: " << averageFrameRate << " FPS\n";
}