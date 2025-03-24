#include "CellGrid.h"

#include <array>
#include <imgui.h>

#include "Random.h"
#include "World.h"

#define UPDATE !clock
#define DONT_UPDATE clock


namespace Naito {
CellGrid::CellGrid(const size_t width, const size_t height) : clock(false),
                                                              cells(new std::vector(
                                                                  width * height, Cell{Element::Empty, DONT_UPDATE})),
                                                              height(height),
                                                              width(width) {
    cells->assign(width * height, Cell{Element::Empty, DONT_UPDATE});
    cells->assign(width * height, Cell{Element::Empty, DONT_UPDATE});
}

CellGrid::~CellGrid() {
    delete cells;
}

Cell CellGrid::getCell(const Uint16 x, const Uint16 y) const {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return Cell{Element::Wall, DONT_UPDATE};

    return cells->at(x + y * width);
}

void CellGrid::setCell(const Uint16 x, const Uint16 y, const Cell cell) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    cells->at(x + y * width) = cell;
}

void CellGrid::swapCells(const Uint16 x, const Uint16 y, const Uint16 dx, const Uint16 dy) {
    const auto cell = getCell(x, y);
    const auto neighbour = getCell(x + dx, y + dy);

    setCell(x + dx, y + dy, Cell{cell.element, DONT_UPDATE});
    setCell(x, y, Cell{neighbour.element, DONT_UPDATE});
}

void CellGrid::update() {
    //setCell(10, 10, Cell{Element::Sand, UPDATE});

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
}

void CellGrid::updateSand(const Uint16 x, const Uint16 y) {
    const Cell down = getCell(x, y + 1);
    if (down.isEmpty() || down.isLiquid()) {
        swapCells(x, y, 0, 1);
        return;
    }

    const int randomDir = randomDirection();
    const Cell downRand = getCell(x + randomDir, y + 1);
    if (downRand.isEmpty() || downRand.isLiquid()) {
        swapCells(x, y, randomDir, 1);
        return;
    }

    const Cell downRandMirror = getCell(x - randomDir, y + 1);
    if (downRandMirror.isEmpty() || downRandMirror.isLiquid())
        swapCells(x, y, -randomDir, 1);
}

void CellGrid::updateWater(const Uint16 x, const Uint16 y) {
    if (getCell(x, y + 1).element == Element::Empty) {
        swapCells(x, y, 0, 1);
        return;
    }

    const short randomDir = randomDirection();
    if (getCell(x + randomDir, y + 1).element == Element::Empty)
        swapCells(x, y, randomDir, 1);

    else if (getCell(x - randomDir, y + 1).element == Element::Empty)
        swapCells(x, y, -randomDir, 1);

    else if (getCell(x + randomDir, y).element == Element::Empty)
        swapCells(x, y, randomDir, 0);
}

bool CellGrid::drawGui() {
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

bool CellGrid::getClock() const {
    return clock;
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

}
