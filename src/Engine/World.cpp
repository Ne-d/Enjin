#include "World.h"

#include "SDL.h"


namespace Naito {

World::World(const size_t width, const size_t height) : grid(width, height),
                                                        clock(0),
                                                        width(width),
                                                        height(height) {}

void World::update() {
    grid.update();
}


void World::drawCells(SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer) const {
    SDL_LockTextureToSurface(texture, nullptr, &surface);

    auto* pixels = static_cast<Uint32*>(surface->pixels);
    const auto formatDetails = SDL_GetPixelFormatDetails(surface->format);

    for (int x = 0; x < width; ++x) {
        for (int y = height - 1; y >= 0; --y) {
            const unsigned long offset = x + surface->pitch / sizeof(Uint32) * y;
            const SDL_Color color = cellColor(grid.getCell(x, y));
            *(pixels + offset) = SDL_MapRGBA(formatDetails, nullptr, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        }
    }

    SDL_UnlockTexture(texture);

    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
}

unsigned long long World::getClock() const {
    return clock;
}

CellGrid& World::getCellGrid() {
    return grid;
}

} // Naito
