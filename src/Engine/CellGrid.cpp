#include "CellGrid.h"

#include <array>
#include <imgui.h>

#include "Game.h"
#include "World.h"

#define UPDATE !clock
#define DONT_UPDATE clock


namespace Naito {
CellGrid::CellGrid(const size_t width, const size_t height) :
    clock(false),
    backbuffer(std::vector(width * height, Cell{Element::Empty, DONT_UPDATE})),
    frontbuffer(std::vector(width * height, Cell{Element::Empty, DONT_UPDATE})),
    height(height),
    width(width) {
    backbuffer.assign(width * height, Cell{Element::Empty, DONT_UPDATE});
    backbuffer.assign(width * height, Cell{Element::Empty, DONT_UPDATE});
}

Cell CellGrid::getCell(const Uint16 x, const Uint16 y) const {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return Cell{Element::Wall, DONT_UPDATE};

    return backbuffer.at(x + y * width);
}

void CellGrid::setCell(const Uint16 x, const Uint16 y, const Cell cell) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    backbuffer.at(x + y * width) = cell;
}

void CellGrid::swapCells(const Uint16 x, const Uint16 y, const Uint16 dx, const Uint16 dy) {
    const auto cell = getCell(x, y);
    const auto neighbour = getCell(x + dx, y + dy);

    setCell(x + dx, y + dy, Cell{cell.element, DONT_UPDATE});
    setCell(x, y, Cell{neighbour.element, DONT_UPDATE});
}

void CellGrid::update() {
    if (getCell(10, 10).isEmpty())
        setCell(10, 10, Cell{Element::Sand, UPDATE});

    for (unsigned int y = height - 1; y > 0; --y) {
        for (unsigned int x = 0; x < width; ++x) {
            const Cell cell = getCell(x, y);
            if (cell.getClock() == UPDATE) {
                switch (cell.element) {
                case Element::Sand:
                    updateSand(x, y);
                    break;

                case Element::Water:
                    updateWater(x, y);
                    break;

                default:
                    break;
                }
            }
        }
    }

    clock = !clock;
    copyToFrontbuffer();
}

bool CellGrid::getClock() const {
    return clock;
}

void CellGrid::copyToFrontbuffer() {
    // TODO: Lock mutex
    frontbuffer = std::vector(backbuffer);
    // TODO: Unlock mutex
}

bool CellGrid::drawGui() const {
    ImGui::Begin("Cell Grid");

#ifndef NDEBUG // Pretty expensive
    const auto elementCounts = countCells();

    for (unsigned int i = 0; i < static_cast<int>(Element::Count); ++i) {
        std::string name = elementName(static_cast<Element>(i));
        ImGui::Value(name.c_str(), elementCounts.at(i));
    }
#else
    ImGui::Text("Element cell count disabled for performance");
#endif

    ImGui::End();

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

int cellGridUpdateLoop(CellGrid* cellGrid) {
    while (Game::get()->isRunning()) {
        cellGrid->update();
        SDL_Delay(16);
    }

    return 0;
}

}
