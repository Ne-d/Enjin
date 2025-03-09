#ifndef CELLGRID_H
#define CELLGRID_H

#include <vector>

#include "Cell.h"


namespace Naito {

class CellGrid {
public:
    CellGrid(size_t width, size_t height);

    [[nodiscard]] Cell getCell(int x, int y) const;
    void setCell(int x, int y, Cell cell);
    void swapCells(int x, int y, int dx, int dy);

    void update();
    void updateSand(uint16_t x, uint16_t y);

    void swapBuffers();

private:
    std::vector<Cell> cells;
    std::vector<Cell> nextCells;

    unsigned int height;
    unsigned int width;
};

}

#endif //CELLGRID_H
