#include "Cell.h"


namespace Naito {

Cell::Cell(const Element element, const bool clock): element(element) {
    setClock(clock);
}

bool Cell::getClock() const {
    return clock;
}

void Cell::setClock(const bool clock) {
    this->clock = clock;
}

bool Cell::isEmpty() const {
    return element == Element::Empty;
}

bool Cell::isLiquid() const {
    return element == Element::Water;
}

std::string elementName(const Element element) {
    switch (element) {
    case Element::Empty: return "Empty";
    case Element::Wall: return "Wall";
    case Element::Dirt: return "Dirt";
    case Element::Sand: return "Sand";
    case Element::Water: return "Water";
    case Element::Lava: return "Lava";
    case Element::Fire: return "Fire";
    case Element::Count: return "Count";
    default: return "unknown";
    }
}

SDL_Color cellColor(const Cell cell) {
    switch (cell.element) {
    case Element::Sand:
        return SDL_Color{247, 233, 155};
    case Element::Water:
        return SDL_Color{22, 68, 127};

    default:
        return SDL_Color{255, 255, 255};
    }
}
} // Naito
