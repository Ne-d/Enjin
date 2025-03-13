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

SDL_Color cellColor(const Cell cell) {
    switch (cell.element) {
    case Element::Sand:
        return SDL_Color{247, 233, 155};
    case Element::Water:
        return SDL_Color{22, 68, 127};

    default:
        return SDL_Color{0, 0, 0};
    }
}
} // Naito
