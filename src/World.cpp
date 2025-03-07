#include "World.h"

#include "SDL.h"


namespace Naito {

World::World(const size_t width, const size_t height) :
    cells(std::vector<Cell>(width * height)),
    nextCells(std::vector<Cell>(width * height)),
    clock(0),
    width(width),
    height(height) {
    cells.assign(width * height, Cell{Element::Empty});
    cells.assign(width * height, Cell{Element::Empty});
}

World::~World() = default;

Cell World::getCell(const int x, const int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return Cell{Element::Wall};
    }

    return cells.at(x + y * width);
}

void World::setCell(const int x, const int y, const Cell cell) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    nextCells.at(x + y * width) = cell;
}

void World::swapCells(int x, int y, int dx, int dy) {
    const auto cell = getCell(x, y);
    const auto neighbour = getCell(x + dx, y + dy);

    setCell(x + dx, y + dy, cell);
    setCell(x, y, neighbour);
}

void World::updateCells() {
    for (uint16_t y = 0; y < height; ++y) {
        for (uint16_t x = 0; x < width; ++x) {
            Cell cell = getCell(x, y);

            switch (cell.element) {
            case Element::Sand:
                updateSand(this, x, y);
                break;

            default:
                break;
            }
        }
    }

    clock++;
    swapBuffers();
}

void World::drawCells(SDL_Surface* surface, SDL_Texture* texture, SDL_Renderer* renderer) const {
    SDL_LockTextureToSurface(texture, nullptr, &surface);

    auto* pixels = static_cast<Uint32*>(surface->pixels);
    const auto formatDetails = SDL_GetPixelFormatDetails(surface->format);

    for (int x = 0; x < width; ++x) {
        for (int y = height - 1; y >= 0; --y) {
            const unsigned long offset = x + surface->pitch / sizeof(Uint32) * y;
            const SDL_Color color = cellColor(getCell(x, y));
            *(pixels + offset) = SDL_MapRGBA(formatDetails, nullptr, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        }
    }

    SDL_UnlockTexture(texture);

    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
}

void World::swapBuffers() {
    std::swap(cells, nextCells);
    nextCells.assign(width * height, Cell{Element::Empty});
}

unsigned long long World::getClock() const {
    return clock;
}

} // Naito
