#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "SDL_render.h"
#include "SDL_surface.h"

#include "Cell.h"
#include "CellGrid.h"


namespace Naito {

class World {
public:
    // Methods
    World(size_t width, size_t height);

    void update();

    void drawCells(SDL_Window* window);

    [[nodiscard]] unsigned long long getClock() const;
    [[nodiscard]] CellGrid& getCellGrid();

private:
    CellGrid grid;

    unsigned long long clock;

    const uint16_t width;
    const uint16_t height;

    SDL_Surface* surface;
    SDL_Texture* texture;
};

} // Naito

#endif //WORLD_H
