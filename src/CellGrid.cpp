#include "CellGrid.h"

#include "Random.h"
#include "World.h"


namespace Naito {
CellGrid::CellGrid(const size_t width, const size_t height) : cells(std::vector<Cell>(width * height)),
                                                              nextCells(std::vector<Cell>(width * height)),
                                                              height(height),
                                                              width(width) {
    cells.assign(width * height, Cell{Element::Empty});
    cells.assign(width * height, Cell{Element::Empty});
}

Cell CellGrid::getCell(const int x, const int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return Cell{Element::Wall};
    }

    return cells.at(x + y * width);
}

void CellGrid::setCell(const int x, const int y, const Cell cell) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    nextCells.at(x + y * width) = cell;
}

void CellGrid::swapCells(int x, int y, int dx, int dy) {
    const auto cell = getCell(x, y);
    const auto neighbour = getCell(x + dx, y + dy);

    setCell(x + dx, y + dy, cell);
    setCell(x, y, neighbour);
}

void CellGrid::update() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Cell cell = getCell(x, y);

            switch (cell.element) {
            case Element::Sand:
                updateSand(x, y);
                break;

            default:
                break;
            }
        }
    }
    swapBuffers();
}

void CellGrid::updateSand(const uint16_t x, const uint16_t y) {

    if (getCell(x, y + 1).element == Element::Empty) {
        swapCells(x, y, 0, 1);
        return;
    }

    short randomDir = randomDirection();
    if (getCell(x + randomDir, y + 1).element == Element::Empty)
        swapCells(x, y, randomDir, 1);

    else if (getCell(x - randomDir, y + 1).element == Element::Empty)
        swapCells(x, y, -randomDir, 1);

    else
        setCell(x, y, Cell{Element::Sand});
}

void CellGrid::swapBuffers() {
    std::swap(cells, nextCells);
    nextCells.assign(width * height, Cell{Element::Empty});
}

}
