#ifndef WORLD_H
#define WORLD_H

#include "SDL_render.h"
#include "SDL_surface.h"

#include "Cell.h"
#include "CellGrid.h"
#include "Entity.h"


namespace Naito {

/// A class that contains a CellGrid and a list of entities
class World {
public:
    // Methods
    World(size_t width, size_t height);
    ~World();

    void update();

    void draw();
    void drawCells();

    [[nodiscard]] CellGrid& getCellGrid();

    // Coordinate conversion between world and window coordinates
    void windowToWorldCoordinates(float windowX, float windowY, float* worldX, float* worldY) const;
    void worldToWindowCoordinates(float worldX, float worldY, float* windowX, float* windowY) const;
    [[nodiscard]] float getHorizontalScale() const;
    [[nodiscard]] float getVerticalScale() const;
    [[nodiscard]] float getCellSize() const;

    [[nodiscard]] bool hasCollision(Uint16 x, Uint16 y, Uint16 width, Uint16 height) const;

    void saveToFile(const std::string& fileName);
    void loadFromFile(const std::string& fileName);

private:
    CellGrid grid;
    std::vector<Entity*> entities;

    uint16_t width;
    uint16_t height;

    // Used for drawing
    SDL_Surface* surface;
    SDL_Texture* texture;

    // Rectangle in which the world is drawn
    SDL_FRect displayRect{};
};

} // Naito

#endif //WORLD_H
