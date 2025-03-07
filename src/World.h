#ifndef WORLD_H
#define WORLD_H

#include <SDL_render.h>
#include <SDL_surface.h>
#include <vector>

#include "Cell.h"


namespace Naito {

class World {
public:
    // Methods
    World(size_t width, size_t height);
    ~World();

    // Cells
    [[nodiscard]] Cell getCell(int x, int y) const;
    void setCell(int x, int y, Cell cell);
    void swapCells(int x, int y, int dx, int dy);

    void updateCells();
    void drawCells(SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer) const;

    void swapBuffers();

    [[nodiscard]] unsigned long long getClock() const;

private:
    std::vector<Cell> cells;
    std::vector<Cell> nextCells;

    unsigned long long clock;

    const uint16_t width;
    const uint16_t height;
};

} // Naito

#endif //WORLD_H
