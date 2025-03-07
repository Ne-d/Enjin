#include "Cell.h"

#include "Random.h"
#include "World.h"


namespace Naito {

void updateSand(World* const world, const uint16_t x, const uint16_t y) {

    if (world->getCell(x, y + 1).element == Element::Empty) {
        world->swapCells(x, y, 0, 1);
        return;
    }

    short randomDir = randomDirection();
    if (world->getCell(x + randomDir, y + 1).element == Element::Empty)
        world->swapCells(x, y, randomDir, 1);

    else if (world->getCell(x - randomDir, y + 1).element == Element::Empty)
        world->swapCells(x, y, -randomDir, 1);

    else
        world->setCell(x, y, Cell{Element::Sand});
}

SDL_Color cellColor(const Cell cell) {
    switch (cell.element) {
    case Element::Sand:
        return SDL_Color{255, 255, 255};

    default:
        return SDL_Color{0, 0, 0};
    }
}
} // Naito
