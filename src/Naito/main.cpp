// ReSharper disable CppInconsistentNaming
#include <iostream>

#define SDL_MAIN_USE_CALLBACKS 1

#include <thread>
#include <chrono>

#include "SDL.h"
#include "SDL_main.h"
#include "SDL_video.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "Game.h"
#include "Render.h"
#include "World.h"

using namespace Naito;
using namespace std::chrono;
using namespace std::chrono_literals;

SDL_Texture* texture;

time_point<steady_clock> start;
time_point<steady_clock> now;
time_point<steady_clock> prev;

unsigned int frameCount = 0;
double averageFrameRate = 0;

World* world;
std::thread simulationThread;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialise SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Naito", SCREEN_WIDTH, SCREEN_HEIGHT,
                                     SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // IF using Docking Branch

    io.Fonts->Build();

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                SCREEN_HEIGHT);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    world = Game::get()->getWorld();

    for (int y = 50; y < 100; ++y) {
        for (int x = 50; x < 100; ++x) {
            world->getCellGrid().setCell(x, y, Cell{Element::Water, world->getCellGrid().getClock()});
        }
    }

    start = steady_clock::now();

    simulationThread = std::thread(CellGrid::updateLoop, &world->getCellGrid());

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    ImGui_ImplSDL3_ProcessEvent(event); // Forward your event to backend

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
    // Start the Dear ImGui frame
    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui::NewFrame();

    SDL_RenderClear(renderer);

    prev = now;

    SDL_PumpEvents();
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    if (keyboardState[SDL_SCANCODE_SPACE])
        world->getCellGrid().setCell(10, 10, Cell{Element::Sand, world->getCellGrid().getClock()});

    // Update world
    world->drawCells();
    world->getCellGrid().drawGui();

    ImGui::Render();

    // Update displayed frame
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);

    frameCount++;
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    now = steady_clock::now();
    averageFrameRate = static_cast<double>(frameCount) / static_cast<double>((now - start).count()) * 1'000'000'000;
    std::cout << "Average framerate: " << averageFrameRate << " FPS\n";

    Game::get()->quit();

    simulationThread.join();

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}
