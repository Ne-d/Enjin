// ReSharper disable CppInconsistentNaming
#include <iostream>

#include <thread>
#include <chrono>

#include "SDL.h"
#include "SDL_video.h"
#include "SDL_render.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "Game.h"
#include "Render.h"
#include "World.h"

using namespace Naito;
using namespace std::chrono;
using namespace std::chrono_literals;

time_point<steady_clock> start;
time_point<steady_clock> now;
time_point<steady_clock> prev;

unsigned int frameCount = 0;
unsigned int lastFrameCount = 0;
double averageFrameRate = 0;

bool quit = false;

World* world;
std::thread simulationThread;

int main() {
    // =================== INITIALIZATION ===================
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialise SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Naito", 1920, 1080, SDL_WINDOW_RESIZABLE, &globalWindow, &globalRenderer)) {
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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

    io.Fonts->Build();

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(globalWindow, globalRenderer);
    ImGui_ImplSDLRenderer3_Init(globalRenderer);

    world = Game::get()->getWorld();

    start = steady_clock::now();

    // The CellGrid (cellular automaton) simulation runs on a different thread.
    simulationThread = std::thread(&CellGrid::updateLoop, &world->getCellGrid());

    world->loadFromFile("defaultLevel.txt");

    // =================== Main Loop ===================

    while (!quit) {
        // Timing for frameTime
        time_point<high_resolution_clock> frameStart = high_resolution_clock::now();

        // SDL event processing
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            ImGui_ImplSDL3_ProcessEvent(&event);

            switch (event.type) {
            case SDL_EVENT_QUIT:
                quit = true;

            case SDL_EVENT_KEY_DOWN:
                if (event.key.scancode == SDL_SCANCODE_ESCAPE)
                    quit = true;

            default:
                break;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui::NewFrame();

        SDL_RenderClear(globalRenderer);

        prev = now;

        // Update world
        world->update();

        // Draw
        world->draw();

        // Gui
        Game::get()->drawGui();

        // Render
        ImGui::Render();
        ImGui::UpdatePlatformWindows();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), globalRenderer);
        SDL_RenderPresent(globalRenderer);

        // Frametime calculation
        frameCount++;
        auto frameEnd = high_resolution_clock::now();
        auto frameTime = frameEnd - frameStart;
        Game::get()->setFrameTime(static_cast<float>(duration_cast<nanoseconds>(frameTime).count()) / 1'000'000'000.0f);
    }

    // Average framerate calculation
    now = steady_clock::now();
    averageFrameRate = static_cast<double>(frameCount) / static_cast<double>((now - start).count()) * 1'000'000'000;
    std::cout << "Average framerate: " << averageFrameRate << " FPS\n";

    Game::get()->quit();

    simulationThread.join();

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_Quit();

    return EXIT_SUCCESS;
}
