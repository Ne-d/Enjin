#ifndef SPECIES_H
#define SPECIES_H

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
    int8_t value;

private:
    uint8_t clock{};

    // Methods
public:
    Cell(Element element, bool clock);

    // The clock can't be manipulated directly because it's a 1-bit value in an 8-bit type,
    // so I might want to pack more data into it in the future, that's why we have to use methods.
    [[nodiscard]] bool getClock() const;
    void setClock(bool clock);

    [[nodiscard]] SDL_Color getColor() const;

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] bool isLiquid() const;
    [[nodiscard]] bool isSolid() const;
    [[nodiscard]] bool isGas() const;
};


const char* elementName(Element element);

} // Naito

#endif //SPECIES_H
