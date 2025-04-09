#ifndef CELLGRID_H
#define CELLGRID_H

#include <mutex>
#include <vector>

#include "Cell.h"


namespace Naito {

class CellGrid {
public:
    CellGrid(size_t width, size_t height);

    [[nodiscard]] Cell getCell(Uint16 x, Uint16 y) const;
    void setCell(Uint16 x, Uint16 y, Cell cell);
    void swapCells(Uint16 x, Uint16 y, Uint16 dx, Uint16 dy);

    void update();
    void updateSand(Uint16 x, Uint16 y);
    void updateWater(Uint16 x, Uint16 y);

    [[nodiscard]] bool getClock() const;
    [[nodiscard]] std::mutex& getMutex();

    void copyToFrontbuffer();

    bool drawGui() const;

private:
    [[nodiscard]] std::array<unsigned int, static_cast<int>(Element::Count)> countCells() const;

    unsigned long long generation;
    bool clock;

    std::vector<Cell> backbuffer;
    std::vector<Cell> frontbuffer;

    std::mutex mutex;

    Uint16 height;
    Uint16 width;
};


int cellGridUpdateLoop(CellGrid* cellGrid);

}

#endif //CELLGRID_H
