#ifndef SPECIES_H
#define SPECIES_H

#include <cstdint>
#include <string>

#include "SDL.h"


namespace Naito {
class World;


enum class Element : uint8_t {
    Empty,
    Wall,
    Dirt,
    Grass,
    Wood,
    Sand,
    Water,
    Fire,
    Count
};


struct Cell {
    // Data
    Element element;

private:
    uint8_t clock{};

    // Methods
public:
    Cell(Element element, bool clock);

    [[nodiscard]] bool getClock() const;
    void setClock(bool clock);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] bool isLiquid() const;
};


const char* elementName(Element element);
SDL_Color cellColor(Cell cell);

} // Naito

#endif //SPECIES_H
