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

Cell::Cell(const Element element, const int8_t value, const bool clock): element(element), value(value) {
    setClock(clock);
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
        return getColorFromValue({150, 150, 150}, value, 0.1f);
    case Element::Sand:
        return getColorFromValue({249, 237, 167}, value, 0.25f);
    case Element::Dirt:
        return getColorFromValue({70, 50, 30}, value, 0.5f);
    case Element::Grass:
        return getColorFromValue({126, 229, 61}, value, 0.3f);
    case Element::Water:
        // Water changes color randomly every frame, not based on the value of the cell,
        // otherwise, it looks like some sort of powder.
        return getColorFromValue({22, 68, 127}, static_cast<int8_t>(randomInt(0, 127)), 0.2f);
    case Element::Fire:
        return getColorFromValue({252, 55, 20}, value, 0.9f);

    default:
        // If the debug color was anything other than pink, it wouldn't be legit.
        return SDL_Color{255, 0, 255};
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

SDL_Color getColorFromValue(const SDL_Color color, const int8_t value, const float influence) {
    const float modifier = (static_cast<float>(value) / 127.0f) * influence;
    float r = color.r;
    float g = color.g;
    float b = color.b;

    r -= r * modifier;
    g -= g * modifier;
    b -= b * modifier;

    const SDL_Color newColor = {
        static_cast<uint8_t>(r),
        static_cast<uint8_t>(g),
        static_cast<uint8_t>(b),
        SDL_ALPHA_OPAQUE
    };

    return newColor;
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
