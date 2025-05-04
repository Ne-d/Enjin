#include "Cell.h"
#include "Random.h"


namespace Naito {

Cell::Cell(const Element element, const bool clock): element(element) {
    setClock(clock);

    switch (element) {
    case Element::Fire:
        value = static_cast<int8_t>(randomInt(100, 127));
        break;

    default:
        value = static_cast<int8_t>(randomInt(0, 127));
    }

}

bool Cell::getClock() const {
    return clock;
}

void Cell::setClock(const bool clock) {
    this->clock = clock;
}

SDL_Color Cell::getColor() const {
    switch (this->element) {
    case Element::Empty:
        return SDL_Color{0, 0, 0};
    case Element::Wall:
        return SDL_Color{150, 150, 150};
    case Element::Sand:
        return SDL_Color{247, 233, 155};
    case Element::Dirt:
        return SDL_Color{58, 41, 18};
    case Element::Grass:
        return SDL_Color{126, 229, 61};
    case Element::Water:
        return SDL_Color{22, 68, 127};

    default:
        return SDL_Color{255, 255, 0};
    }
}

bool Cell::isEmpty() const {
    return element == Element::Empty;
}

bool Cell::isLiquid() const {
    return element == Element::Water;
}

bool Cell::isSolid() const {
    return !isEmpty() && !isLiquid() && !isGas();
}

bool Cell::isGas() const {
    return element == Element::Fire;
}

const char* elementName(const Element element) {
    switch (element) {
    case Element::Empty: return "Empty";
    case Element::Wall: return "Wall";
    case Element::Dirt: return "Dirt";
    case Element::Grass: return "Grass";
    case Element::Wood: return "Wood";
    case Element::Sand: return "Sand";
    case Element::Water: return "Water";
    case Element::Fire: return "Fire";
    default: return "unknown";
    }
}

} // Naito
