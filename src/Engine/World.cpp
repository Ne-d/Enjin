#include "World.h"

#include <iostream>

#include "Render.h"


namespace Naito {

World::World(const size_t width, const size_t height)
    : grid(width, height),
      clock(0),
      width(width),
      height(height),
      surface(SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBX8888)) {
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
}

void World::update() {
    grid.update();
}


void World::drawCells() {
    SDL_LockTextureToSurface(texture, nullptr, &surface);

    // As unclean as that is, it seems to be the way to go with SDL
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

    const bool res = SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    if (!res) std::cerr << "World::drawCells: SDL_RenderTexture failed - " << SDL_GetError() << std::endl;
}

unsigned long long World::getClock() const {
    return clock;
}

CellGrid& World::getCellGrid() {
    return grid;
}

} // Naito
