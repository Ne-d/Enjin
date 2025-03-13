#ifndef CELLGRID_H
#define CELLGRID_H

#include <vector>

#include "Cell.h"


namespace Naito {

class CellGrid {
public:
    CellGrid(size_t width, size_t height);
    ~CellGrid();

    [[nodiscard]] Cell getCell(Uint16 x, Uint16 y) const;
    void setCell(Uint16 x, Uint16 y, Cell cell);
    void swapCells(Uint16 x, Uint16 y, Uint16 dx, Uint16 dy);

    void update();
    void updateSand(Uint16 x, Uint16 y);
    void updateWater(Uint16 x, Uint16 y);

    [[nodiscard]] bool getClock() const;

private:
    bool clock;

    std::vector<Cell>* cells;

    Uint16 height;
    Uint16 width;
};

}

#endif //CELLGRID_H
