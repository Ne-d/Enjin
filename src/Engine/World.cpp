#include "World.h"

#include <iostream>

#include "Render.h"


namespace Naito {

World::World(const size_t width, const size_t height)
    : grid(width, height),
      clock(0),
      width(width),
      height(height),
      surface(SDL_CreateSurface(static_cast<int>(width), static_cast<int>(height), SDL_PIXELFORMAT_RGBX8888)),
      texture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING,
                                static_cast<int>(width), static_cast<int>(height))) {
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
}

void World::update() {
    grid.update();
}


void World::drawCells() {
    int windowWidthInt, windowHeightInt;
    SDL_GetWindowSize(window, &windowWidthInt, &windowHeightInt);

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
    SDL_RenderTexture(renderer, texture, nullptr, &displayRect);
}

unsigned long long World::getClock() const {
    return clock;
}

CellGrid& World::getCellGrid() {
    return grid;
}

void World::windowToWorldCoordinates(float windowX, float windowY, float* worldX, float* worldY) {
    float horizontalScale = width / displayRect.w;
    float verticalScale = height / displayRect.h;
    *worldX = windowX * horizontalScale - displayRect.x * horizontalScale;
    *worldY = windowY * verticalScale - displayRect.y * verticalScale;
}

} // Naito
