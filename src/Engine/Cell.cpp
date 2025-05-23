#include "Cell.h"

#include <cmath>
#include <sstream>

#include "Random.h"


namespace Naito {

Cell::Cell(const Element element, const bool clock): element(element), fuel(255) {
    setClock(clock);

    switch (element) {
    case Element::Fire:
        // Fire, unlike other elements, starts with a high value, because it will decrease over time.
        value = static_cast<int8_t>(randomInt(100, 127));
        break;

    default:
        value = static_cast<int8_t>(randomInt(0, 127));
    }

}

Cell::Cell(const Element element, const int8_t value, const bool clock): element(element), value(value), fuel(255) {
    setClock(clock);
}

Cell::Cell(const Element element, const int8_t value, const Uint8 fuel, const bool clock) :
    element(element),
    value(value),
    fuel(fuel) {
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
        return getColorFromValue({249, 237, 167}, value, 0.3f);

    case Element::Dirt:
        return getColorFromValue({70, 50, 30}, value, 0.5f);

    case Element::Grass:
        return getColorFromValue({126, 229, 61}, value, 0.3f);

    case Element::Water:
        // Water changes color randomly every frame, not based on the value of the cell,
        // otherwise, it looks like some sort of powder.
        return getColorFromValue({22, 68, 127}, static_cast<int8_t>(randomInt(0, 127)), 0.2f);

    case Element::Fire:
        return lerpColorFromValue({0, 0, 0}, {252, 55, 20}, value);

    case Element::Wood:
        return getColorFromValue({76, 54, 20}, value, 0.3f);

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

bool Cell::isEmptyOrGas() const {
    return isEmpty() || isGas();
}

std::string Cell::serialise() const {
    std::string result{};
    std::stringstream stream{result};

    stream << serialiseElement(element) << " "
        << std::to_string(value) << " "
        << std::to_string(fuel) << " "
        << std::to_string(clock);

    return stream.str();
}

SDL_Color getColorFromValue(const SDL_Color color, const int8_t value, const float influence) {
    const float modifier = (static_cast<float>(value) / 127.0f) * influence;
    float r = color.r;
    float g = color.g;
    float b = color.b;

    // Pretty ugly way to handle colors, but I'm not looking for precision.
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

SDL_Color lerpColorFromValue(const SDL_Color color1, const SDL_Color color2, const int8_t value) {
    const float modifier = static_cast<float>(value) / 127.0f;

    // Lerping individual RGB components probably won't yeld the best-looking result, but I'm no graphics programmer.
    const float r = std::lerp(static_cast<float>(color1.r), static_cast<float>(color2.r), modifier);
    const float g = std::lerp(static_cast<float>(color1.g), static_cast<float>(color2.g), modifier);
    const float b = std::lerp(static_cast<float>(color1.b), static_cast<float>(color2.b), modifier);

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

std::string serialiseElement(Element element) {
    return std::to_string(static_cast<int>(element));
}

Element deserialiseElement(const std::string& serialised) {
    return static_cast<Element>(std::stoi(serialised));
}

Cell deserialiseCell(const std::string& serialised) {
    std::istringstream stream{serialised};
    std::string buffer;

    stream >> buffer;
    const Element element = deserialiseElement(buffer);

    stream >> buffer;
    const auto value = static_cast<int8_t>(std::stoi(buffer));

    stream >> buffer;
    const auto fuel = static_cast<uint8_t>(std::stoi(buffer));

    stream >> buffer;
    const auto clock = static_cast<uint8_t>(std::stoi(buffer));

    return Cell{element, value, fuel, clock == 1};
}

} // Naito
