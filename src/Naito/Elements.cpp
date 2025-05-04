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

void CellGrid::updateDirt(const Uint16 x, const Uint16 y) {
    // Transform into grass if in contact with water
    bool foundWater = false;
    for (int i = x - 1; i <= x + 1 && !foundWater; ++i) {
        for (int j = y - 1; j <= y + 1 && !foundWater; ++j) {
            if (getCell(i, j).element == Element::Water) {
                // Turn yourself into grass
                setCell(x, y, Cell{Element::Grass, DONT_UPDATE});

                // Absorb the water
                setCell(i, j, Cell{Element::Empty, DONT_UPDATE});

                // Stop the loop
                foundWater = true;
            }
        }
    }

    // Dirt falls like sand
    updateSand(x, y);
}

void CellGrid::updateGrass(const Uint16 x, const Uint16 y) {
    updateSand(x, y);

    // Spread to nearby dirt
    bool foundDirt = false;
    for (int i = x - 1; i <= x + 1 && !foundDirt; ++i) {
        for (int j = y - 1; j <= y + 1 && !foundDirt; ++j) {
            if (getCell(i, j).element == Element::Dirt) {
                // Randomly turn nearby cell to grass
                if (randomFloat(0, 1) <= 0.02f)
                    setCell(i, j, Cell{Element::Grass, UPDATE});

                // Stop the loop
                foundDirt = true;
            }
        }
    }
}
}
