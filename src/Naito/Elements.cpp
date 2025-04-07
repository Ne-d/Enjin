#include "CellGrid.h"
#include "Random.h"


namespace Naito {

// I don't like sand. It's coarse and rough, and irritating. And it gets everywhere.
void CellGrid::updateSand(const Uint16 x, const Uint16 y) {
    const Cell down = getCell(x, y + 1);
    if (down.isEmpty() || down.isLiquid()) {
        swapCells(x, y, 0, 1);
        return;
    }

    const int randomDir = randomDirection();
    const Cell downRand = getCell(x + randomDir, y + 1);
    if (downRand.isEmpty() || downRand.isLiquid())
        swapCells(x, y, randomDir, 1);
}


void CellGrid::updateWater(const Uint16 x, const Uint16 y) {
    if (getCell(x, y + 1).element == Element::Empty) {
        swapCells(x, y, 0, 1);
        return;
    }

    const short randomDir = randomDirection();
    if (getCell(x + randomDir, y + 1).element == Element::Empty)
        swapCells(x, y, randomDir, 1);

    else if (getCell(x - randomDir, y + 1).element == Element::Empty) {
        swapCells(x, y, -randomDir, 1);
    }

    else if (getCell(x + randomDir, y).element == Element::Empty)
        swapCells(x, y, randomDir, 0);
}
}
