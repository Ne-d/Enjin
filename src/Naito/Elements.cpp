#include "CellGrid.h"
#include "Random.h"
#include "Math.h"

// These macros are made to avoid having to enter clock values manually because they're confusing.
#define UPDATE !this->clock
#define DONT_UPDATE this->clock


namespace Naito {
// I don't like sand. It's coarse and rough, and irritating. And it gets everywhere.
void CellGrid::updateSand(const Uint16 x, const Uint16 y) {
    // If we can go down, do so.
    const Cell down = getCell(x, y + 1);
    if (down.isEmpty() || down.isLiquid()) {
        swapCells(x, y, 0, 1);
        return;
    }

    // Otherwise, try to go down diagonally.
    const int randomDir = randomDirection();
    const Cell downRand = getCell(x + randomDir, y + 1);
    if (downRand.isEmpty() || downRand.isLiquid())
        swapCells(x, y, randomDir, 1);
}

// Water takes forever to flatten out, but being physically accurate wasn't in the design docs.
void CellGrid::updateWater(const Uint16 x, const Uint16 y) {
    // If we can go down, do so.
    if (getCell(x, y + 1).isEmptyOrGas()) {
        swapCells(x, y, 0, 1);
        return;
    }

    // Otherwise, try to go down diagonally.
    const short randomDir = randomDirection();
    if (getCell(x + randomDir, y + 1).isEmptyOrGas())
        swapCells(x, y, randomDir, 1);

    else if (getCell(x - randomDir, y + 1).isEmptyOrGas()) {
        swapCells(x, y, -randomDir, 1);
    }

    // Otherwise, go sideways.
    else if (getCell(x + randomDir, y).isEmptyOrGas())
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
    const Cell self = getCell(x, y);

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

    // Catch fire
    bool foundFire = false;
    for (int i = x - 1; i <= x + 1 && !foundFire; ++i) {
        for (int j = y - 1; j <= y + 1 && !foundFire; ++j) {
            if (getCell(i, j).element == Element::Fire) {
                // Set cell on fire by setting fuel to less than max
                setCell(x, y, Cell{Element::Grass, self.value, addUint8(self.fuel, -1), DONT_UPDATE});

                // Stop the loop
                foundFire = true;
            }
        }
    }

    // Spread fire
    if (self.fuel < 255) { // If the cell has started burning
        const short dx = static_cast<short>(randomInt(-1, 1));
        const short dy = static_cast<short>(randomInt(-1, 1));

        if (getCell(x + dx, y + dy).isEmpty()) {
            // Create new fire cell
            setCell(x + dx, y + dy, Cell{Element::Fire, DONT_UPDATE});

            // Decay fuel
            setCell(x, y, Cell{Element::Grass, self.value, addUint8(self.fuel, -50), DONT_UPDATE});
        }
    }

    // Extinguish fire
    bool foundWater = false;
    for (int i = x - 1; i <= x + 1 && !foundWater; ++i) {
        for (int j = y - 1; j <= y + 1 && !foundWater; ++j) {
            if (getCell(i, j).element == Element::Water) {
                // Extinguish fire by setting fuel to max
                setCell(x, y, Cell{Element::Grass, self.value, 255, DONT_UPDATE});

                // Stop the loop
                foundWater = true;
            }
        }
    }

    // Empty the cell if it reaches zero fuel.
    if (self.fuel == 0) {
        setCell(x, y, Cell{Element::Empty, DONT_UPDATE});
        return;
    }

    // Grass falls like sand
    updateSand(x, y);
}

void CellGrid::updateFire(const Uint16 x, const Uint16 y) {
    const Cell self = getCell(x, y);

    // Empty the cell if it reaches zero fuel.
    if (self.value <= 0) {
        setCell(x, y, Cell{Element::Empty, DONT_UPDATE});
        return;
    }

    // Setup random motion: potentially move sideways.
    short dx;
    if (randomFloat(0, 1) <= 0.5)
        dx = randomDirection();
    else
        dx = 0;

    // Setup random motion: likely move up.
    short dy;
    if (randomFloat(0, 1) <= 0.9 || dx == 0)
        dy = -1;
    else
        dy = 0;

    const Cell neighbour = getCell(x + dx, y + dy);

    // If we can go to the chosen position, do so, and decrease our value.
    if (neighbour.isEmpty()) {
        setCell(x + dx, y + dy, Cell{Element::Fire, addInt8Positive(self.value, -5), DONT_UPDATE});
        setCell(x, y, Cell{Element::Empty, DONT_UPDATE});
    }
    // If the chosen position is already occupied by fire, increase its value.
    else if (neighbour.element == Element::Fire) {
        setCell(x + dx, y + dy, Cell{
                    Element::Fire, addInt8Positive(self.value, addInt8Positive(neighbour.value, 5)), DONT_UPDATE
                });
        setCell(x, y, Cell{Element::Empty, DONT_UPDATE});
    }
    else // Otherwise, don't move, but decrease our value
        setCell(x, y, Cell{Element::Fire, addInt8Positive(self.value, -5), DONT_UPDATE});
}

void CellGrid::updateWood(Uint16 x, Uint16 y) {
    const Cell self = getCell(x, y);

    // Catch fire
    bool foundFire = false;
    for (int i = x - 1; i <= x + 1 && !foundFire; ++i) {
        for (int j = y - 1; j <= y + 1 && !foundFire; ++j) {
            if (getCell(i, j).element == Element::Fire) {
                // Set cell on fire by setting fuel to less than max
                setCell(x, y, Cell{Element::Wood, self.value, addUint8(self.fuel, -1), DONT_UPDATE});

                // Stop the loop
                foundFire = true;
            }
        }
    }

    // Spread fire
    if (self.fuel < 255) { // If the cell has started burning
        const short dx = static_cast<short>(randomInt(-1, 1));
        const short dy = static_cast<short>(randomInt(-1, 1));

        if (getCell(x + dx, y + dy).isEmpty()) {
            // Create new fire cell
            setCell(x + dx, y + dy, Cell{Element::Fire, DONT_UPDATE});

            // Decay fuel
            setCell(x, y, Cell{Element::Wood, self.value, addUint8(self.fuel, -20), DONT_UPDATE});
        }
    }

    // Extinguish fire
    bool foundWater = false;
    for (int i = x - 1; i <= x + 1 && !foundWater; ++i) {
        for (int j = y - 1; j <= y + 1 && !foundWater; ++j) {
            if (getCell(i, j).element == Element::Water) {
                // Extinguish fire by setting fuel to max
                setCell(x, y, Cell{Element::Wood, self.value, 255, DONT_UPDATE});

                // Stop the loop
                foundWater = true;
            }
        }
    }

    // Empty the cell if it reaches zero fuel.
    if (self.fuel == 0)
        setCell(x, y, Cell{Element::Empty, DONT_UPDATE});
}

}

