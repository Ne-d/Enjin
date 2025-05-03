#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "SDL_render.h"
#include "SDL_surface.h"

#include "Cell.h"
#include "CellGrid.h"
#include "Entity.h"


namespace Naito {

class World {
public:
    // Methods
    World(size_t width, size_t height);

    void update();

    void draw();
    void drawCells();

    [[nodiscard]] unsigned long long getClock() const;
    [[nodiscard]] CellGrid& getCellGrid();
    void windowToWorldCoordinates(float windowX, float windowY, float* worldX, float* worldY) const;
    void worldToWindowCoordinates(float worldX, float worldY, float* windowX, float* windowY) const;

    [[nodiscard]] float getHorizontalScale() const;
    [[nodiscard]] float getVerticalScale() const;
    [[nodiscard]] float getCellSize() const;

    [[nodiscard]] bool hasCollision(Uint16 x, Uint16 y, Uint16 width, Uint16 height);

private:
    CellGrid grid;
    std::vector<Entity*> entities;

    unsigned long long clock;

    const uint16_t width;
    const uint16_t height;

    SDL_Surface* surface;
    SDL_Texture* texture;

    SDL_FRect displayRect{};
};

} // Naito

#endif //WORLD_H
