#include "CellGrid.h"

#include <array>
#include <imgui.h>
#include <thread>
#include <cmath>
#include <iostream>

#include "Game.h"
#include "World.h"

// These macros are made to avoid having to enter clock values manually because they're confusing.
#define UPDATE !this->clock
#define DONT_UPDATE this->clock


namespace Naito {
CellGrid::CellGrid(const size_t width, const size_t height) :
    clock(false),
    backbuffer(std::vector(width * height, Cell{Element::Empty, DONT_UPDATE})),
    frontbuffer(std::vector(width * height, Cell{Element::Empty, DONT_UPDATE})),
    height(height),
    width(width),
    desiredTickrate(100),
    tickInterval(std::chrono::round<std::chrono::nanoseconds>(std::chrono::duration<float>(1.0f / desiredTickrate))),
    actualTickrate(0),
    actualTickDuration(0), activeTickDuration(0) {
    backbuffer.assign(width * height, Cell{Element::Empty, DONT_UPDATE});
    backbuffer.assign(width * height, Cell{Element::Empty, DONT_UPDATE});
}

Cell CellGrid::getCell(const Uint16 x, const Uint16 y) const {
    // If trying to access a cell outside the grid, pretend there is a wall.
    if (x < 0 || x >= width || y < 0 || y >= height)
        return Cell{Element::Wall, DONT_UPDATE};

    return backbuffer.at(x + y * width);
}

void CellGrid::setCell(const Uint16 x, const Uint16 y, const Cell cell) {
    // If trying to set a cell outside the grid, it is safe to do nothing.
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    backbuffer.at(x + y * width) = cell;
}

void CellGrid::swapCells(const Uint16 x, const Uint16 y, const Uint16 dx, const Uint16 dy) {
    const auto cell = getCell(x, y);
    const auto neighbour = getCell(x + dx, y + dy);

    setCell(x + dx, y + dy, Cell{cell.element, cell.value, cell.fuel, DONT_UPDATE});
    setCell(x, y, Cell{neighbour.element, neighbour.value, neighbour.fuel, DONT_UPDATE});
}

void CellGrid::paint(const int x, const int y, const int brushSize) {
    for (int i = x - brushSize / 2; i < x + brushSize; i++)
        for (int j = y - brushSize / 2; j < y + brushSize; j++)
            setCell(std::floor(i), std::floor(j), Cell{Game::get()->getSelectedElement(), UPDATE});
}

void CellGrid::update() {
    // Timing
    //=======
    using namespace std::chrono;
    const high_resolution_clock::time_point start = high_resolution_clock::now();
    actualTickDuration = start - lastUpdate;

    // Paint Cells
    // ===========
    SDL_MouseButtonFlags mouseButtonFlags;
    float mouseX, mouseY;
    mouseButtonFlags = SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseButtonFlags & SDL_BUTTON_LEFT && !ImGui::GetIO().WantCaptureMouse) {
        float floatX, floatY;
        Game::get()->getWorld()->windowToWorldCoordinates(mouseX, mouseY, &floatX, &floatY);
        const int x = std::floor(floatX);
        const int y = std::floor(floatY);
        const int brushSize = Game::get()->getBrushSize();

        paint(x, y, brushSize);
    }

    // Update cells
    // ============

    // Update goes from the lowest row to the highest, then from left to right.
    // In this type of cellular automaton, the update order changes the behaviour of the cells,
    // so updating from bottom to top is necessary.
    for (unsigned int y = height - 1; y > 0; --y) {
        for (unsigned int x = 0; x < width; ++x) {

            const Cell cell = getCell(x, y);
            if (cell.getClock() == UPDATE) {

                // Run the corresponding update function for each cell that needs to be updated.
                switch (cell.element) {
                case Element::Sand:
                    updateSand(x, y);
                    break;

                case Element::Water:
                    updateWater(x, y);
                    break;

                case Element::Dirt:
                    updateDirt(x, y);
                    break;

                case Element::Grass:
                    updateGrass(x, y);
                    break;

                case Element::Fire:
                    updateFire(x, y);
                    break;

                case Element::Wood:
                    updateWood(x, y);
                    break;

                default:
                    break;
                }
            }
        }
    }

    // Flip the clock. All cells that have been updated,
    // and were marked as "no longer need to update", now need to be updated again
    clock = !clock;

    copyToFrontbuffer();

    // Timing
    // ======
    actualTickrate = 1'000'000'000.0f / static_cast<float>(duration_cast<nanoseconds>(actualTickDuration).count());

    lastUpdate = start;
    activeTickDuration = high_resolution_clock::now() - start;
}

bool CellGrid::getClock() const {
    return clock;
}

std::mutex& CellGrid::getMutex() {
    return mutex;
}

void CellGrid::copyToFrontbuffer() {
    std::lock_guard lock(getMutex());
    frontbuffer = std::vector(backbuffer);
}

bool CellGrid::drawGui() {
    using namespace std::chrono;

    if (ImGui::CollapsingHeader("Simulation", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SeparatorText("Desired speed");
        bool desiredTickrateChanged = ImGui::InputFloat("Desired Tickrate", &desiredTickrate, 1, 0);
        if (desiredTickrateChanged) {
            tickInterval = round<nanoseconds>(duration<float>(1.0f / desiredTickrate));
        }

        ImGui::Value("Desired tick interval",
                     static_cast<float>(duration_cast<nanoseconds>(tickInterval).count()) / 1'000'000.0f);

        ImGui::SeparatorText("Measured speed");
        ImGui::Value("Tickrate", actualTickrate);
        ImGui::Value("Tick duration", static_cast<float>(
                         static_cast<double>(duration_cast<nanoseconds>(actualTickDuration).count()) / 1'000'000.0));

        ImGui::Value("Active Tick Duration", duration_cast<nanoseconds>(activeTickDuration).count() / 1'000'000.0f);

        ImGui::Value("Tick budget used",
                     static_cast<float>(duration_cast<nanoseconds>(activeTickDuration).count()) /
                     static_cast<float>(duration_cast<nanoseconds>(tickInterval).count()));
    }

#ifndef NDEBUG // This only runs in debug builds because it's useless for a player, and eats up a lot of resources.
    if (ImGui::CollapsingHeader("Element cell count")) {
        const auto elementCounts = countCells(); // Pretty expensive

        for (unsigned int i = 0; i < static_cast<int>(Element::Count); ++i) {
            std::string name = elementName(static_cast<Element>(i));
            ImGui::Value(name.c_str(), elementCounts.at(i));
        }
    }
#endif

    return true;
}

std::array<unsigned int, static_cast<int>(Element::Count)> CellGrid::countCells() const {
    std::array<unsigned int, static_cast<int>(Element::Count)> counts{};
    counts.fill(0);

    for (Uint16 y = 0; y < height; ++y) {
        for (Uint16 x = 0; x < width; ++x) {
            const Cell cell = getCell(x, y);
            counts.at(static_cast<int>(cell.element))++;
        }
    }

    return counts;
}

void CellGrid::updateLoop() {
    while (Game::get()->isRunning()) {
        update();
        std::this_thread::sleep_until(lastUpdate + tickInterval);
    }
}

}
