#ifndef SPECIES_H
#define SPECIES_H

#include <string>

#include "SDL.h"


namespace Naito {
class World;


/// Elements are the types of "matter" that can be in a cell
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


/**
 * Compact structure that holds all values contained in each cell of the grid.
 */
struct Cell {
    Element element;

    /// Value is initialised to a random value when creating a cell.
    int8_t value;

    /**
     * Fuel is initialised to 255, manages burning reactions.
     * 255 means the cell is not burning, 1 to 254 means the cell is burning,
     * 0 means the cell has been burnt and will be emptied.
     */
    uint8_t fuel;

private:
    /**
     * 1-bit value that toggles back and forth, used to avoid updating a cell
     * several times if it moved during the update loop.
     */
    uint8_t clock{};

    // Methods
public:
    // Constructors
    Cell(Element element, bool clock);
    Cell(Element element, int8_t value, bool clock);
    Cell(Element element, int8_t value, Uint8 fuel, bool clock);

    // The clock can't be manipulated directly because it's a 1-bit value in an 8-bit type,
    // so I might want to pack more data into it in the future, that's why we have to use methods.
    [[nodiscard]] bool getClock() const;
    void setClock(bool clock);

    [[nodiscard]] SDL_Color getColor() const;

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] bool isLiquid() const;
    [[nodiscard]] bool isSolid() const;
    [[nodiscard]] bool isGas() const;
    [[nodiscard]] bool isEmptyOrGas() const; // Because I got tired of writing conditions with both.

    [[nodiscard]] std::string serialise() const;
};


/**
 * Calculates a color that varies depending on a cell's value.
 * @param color Base color
 * @param value Value of a cell
 * @param influence How much the color will be darkened. 0 means no change, 1 means the color can go all the way to black
 * @return The base color, darkened by the value, more or less depending on the influence.
 */
SDL_Color getColorFromValue(SDL_Color color, int8_t value, float influence);

/// Lerps between color1 and color2 depending on a cell's value from 0 to 127.
SDL_Color lerpColorFromValue(SDL_Color color1, SDL_Color color2, int8_t value);

const char* elementName(Element element);

std::string serialiseElement(Element element);
Element deserialiseElement(const std::string& serialised);

Cell deserialiseCell(const std::string& serialised);

} // Naito

#endif //SPECIES_H
