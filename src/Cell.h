#ifndef SPECIES_H
#define SPECIES_H

#include <cstdint>

#include "SDL.h"


namespace Naito {
class World;


enum class Element : uint8_t {
    Empty,
    Wall,
    Dirt,
    Sand,
    Water,
    Lava,
    Fire
};


struct Cell {
    Element element;
};


SDL_Color cellColor(Cell cell);

void updateSand(World* world, uint16_t x, uint16_t y);

} // Naito

#endif //SPECIES_H
