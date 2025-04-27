#include "World.h"

#include <iostream>

#include "Player.h"
#include "Render.h"


namespace Naito {

World::World(const size_t width, const size_t height)
    : grid(width, height),
      clock(0),
      width(width),
      height(height),
      surface(SDL_CreateSurface(static_cast<int>(width), static_cast<int>(height), SDL_PIXELFORMAT_RGBX8888)),
      texture(SDL_CreateTexture(globalRenderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING,
                                static_cast<int>(width), static_cast<int>(height))) {
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    entities.emplace_back(new Player(10, 10));
}

void World::update() {
    for (Entity* e : entities)
        e->update();
}

void World::draw() {
    SDL_SetRenderDrawColor(globalRenderer, 0, 0, 0, 255);
    drawCells();

    for (Entity* e : entities)
        e->draw();
}


void World::drawCells() {
    int windowWidthInt, windowHeightInt;
    SDL_GetWindowSize(globalWindow, &windowWidthInt, &windowHeightInt);

    const float windowWidth = static_cast<float>(windowWidthInt);
    const float windowHeight(static_cast<float>(windowHeightInt));
    const float windowAspectRatio = windowWidth / windowHeight;

    const float worldAspectRatio = static_cast<float>(width) / static_cast<float>(height);

    if (windowAspectRatio > worldAspectRatio) {
        const float scaledWidth = windowHeight * worldAspectRatio;
        const float scaledHeight = windowHeight;
        const float xOffset = (windowWidth - scaledWidth) / 2.0f;
        displayRect = {xOffset, 0, scaledWidth, scaledHeight};
    }
    else if (windowAspectRatio < worldAspectRatio) {
        const float scaledWidth = windowWidth;
        const float scaledHeight = windowWidth / worldAspectRatio;
        const float yOffset = (windowHeight - scaledHeight) / 2.0f;
        displayRect = {0, yOffset, scaledWidth, scaledHeight};
    }
    else
        displayRect = {0, 0, windowWidth, windowHeight};

    // Critical section: Drawing and copying data to frontbuffer cannot happen at the same time
    std::lock_guard guard(getCellGrid().getMutex());

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
    SDL_RenderTexture(globalRenderer, texture, nullptr, &displayRect);
}

unsigned long long World::getClock() const {
    return clock;
}

CellGrid& World::getCellGrid() {
    return grid;
}

void World::windowToWorldCoordinates(const float windowX, const float windowY, float* worldX, float* worldY) const {
    *worldX = windowX * getHorizontalScale() - displayRect.x * getHorizontalScale();
    *worldY = windowY * getVerticalScale() - displayRect.y * getVerticalScale();
}

void World::worldToWindowCoordinates(const float worldX, const float worldY, float* windowX, float* windowY) const {
    *windowX = worldX / getHorizontalScale() + displayRect.x;;
    *windowY = worldY / getVerticalScale() + displayRect.y;
}

float World::getHorizontalScale() const {
    return static_cast<float>(width) / displayRect.w;
}

float World::getVerticalScale() const {
    return static_cast<float>(height) / displayRect.h;
}

float World::getCellSize() const {
    return 1 / getHorizontalScale();
}

bool World::hasCollision(const Uint16 x, const Uint16 y, const Uint16 width, const Uint16 height) {
    bool collision = false;

    for (int i = 0; i < width && !collision; ++i)
        for (int j = 0; j < height && !collision; ++j)
            if (!getCellGrid().getCell(x + i, y + j).isEmpty())
                collision = true;

    return collision;
}


} // Naito
