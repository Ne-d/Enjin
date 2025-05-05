#include "World.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Player.h"
#include "Render.h"


namespace Naito {

World::World(const size_t width, const size_t height)
    : grid(width, height),
      width(width),
      height(height),
      surface(SDL_CreateSurface(static_cast<int>(width), static_cast<int>(height), SDL_PIXELFORMAT_RGBX8888)),
      texture(SDL_CreateTexture(globalRenderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING,
                                static_cast<int>(width), static_cast<int>(height))) {
    // To avoid blurry pixels.
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    entities.emplace_back(new Player(10, 10));
}

World::~World() {
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);

    for (const Entity* e : entities)
        delete e;
}

void World::update() {
    // Update all entities
    for (Entity* e : entities)
        e->update();

    // This function does not update the CellGrid because that is done in a separate loop, on a different thread.
}

void World::draw() {
    SDL_SetRenderDrawColor(globalRenderer, 0, 0, 0, 255);
    drawCells();

    for (Entity* e : entities)
        e->draw();
}


void World::drawCells() {
    // Lots of stuff to get proper, fixed aspect ratio scaling,
    // which amounts to calculating the correct displayRect.
    int windowWidthInt, windowHeightInt;
    SDL_GetWindowSize(globalWindow, &windowWidthInt, &windowHeightInt);

    const auto windowWidth = static_cast<float>(windowWidthInt);
    const auto windowHeight(static_cast<float>(windowHeightInt));
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

    // Writing to the surface will write to the texture, which can then be drawn by the GPU.
    SDL_LockTextureToSurface(texture, nullptr, &surface);

    // As unclean as that is, it seems to be the way to go with SDL
    auto* pixels = static_cast<Uint32*>(surface->pixels);
    const auto formatDetails = SDL_GetPixelFormatDetails(surface->format);

    // Write a pixel for each cell
    for (int x = 0; x < width; ++x) {
        for (int y = height - 1; y >= 0; --y) {
            const unsigned long offset = x + surface->pitch / sizeof(Uint32) * y;
            const SDL_Color color = grid.getCell(x, y).getColor();
            *(pixels + offset) = SDL_MapRGBA(formatDetails, nullptr, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        }
    }

    SDL_UnlockTexture(texture);
    SDL_RenderTexture(globalRenderer, texture, nullptr, &displayRect);
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

bool World::hasCollision(const Uint16 x, const Uint16 y, const Uint16 width, const Uint16 height) const {
    bool collision = false;

    for (int i = 0; i < width && !collision; ++i)
        for (int j = 0; j < height && !collision; ++j)
            if (grid.getCell(x + i, y + j).isSolid())
                collision = true;

    return collision;
}

void World::saveToFile(const std::string& fileName) {
    std::lock_guard guard(getCellGrid().getMutex());

    std::setlocale(LC_ALL, "C"); // To avoid decimal point nonsense (learned that the hard way)

    std::ofstream fileOut(fileName);

    if (!fileOut.is_open()) {
        std::cerr << "World::saveToFile: Failed to open file " << fileName << std::endl;
        return;
    }

    // Write header
    fileOut << "NaitoLevel v1 " << width << " " << height << "\n";

    // Write player position
    const Entity* player = entities.front();
    fileOut << "PlayerPos " << static_cast<float>(player->cx) + player->rx << " "
        << static_cast<float>(player->cy) + player->ry << "\n";

    // Write contents of the CellGrid
    fileOut << "CellGrid\n";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            fileOut << grid.getCell(x, y).serialise() << "\n";
        }
    }

    fileOut << "End";

    fileOut.flush();
    fileOut.close();
}

void World::loadFromFile(const std::string& fileName) {
    std::lock_guard guard(getCellGrid().getMutex());

    std::setlocale(LC_ALL, "C"); // Still don't trust whatever locale might be set

    std::ifstream fileIn(fileName);

    if (!fileIn.is_open()) {
        std::cerr << "World::loadFromFile: Failed to open file " << fileName << std::endl;
        return;
    }

    // Read header
    std::string headerString;
    std::getline(fileIn, headerString);
    std::istringstream headerStream(headerString);
    std::string buffer;

    headerStream >> buffer;
    if (buffer != "NaitoLevel") {
        std::cerr << "World::loadFromFile: File has incorrect header, found " << buffer << std::endl;
        return;
    }

    headerStream >> buffer;
    if (buffer != "v1") {
        std::cerr << "World::loadFromFile: File has incorrect version, found " << buffer << std::endl;
        return;
    }

    headerStream >> buffer;
    width = std::stoi(buffer);

    headerStream >> buffer;
    height = std::stoi(buffer);

    // Read player info
    std::string playerString;
    std::getline(fileIn, playerString);
    std::istringstream playerStream(playerString);
    playerStream >> buffer;
    if (buffer != "PlayerPos") {
        std::cerr << "World::loadFromFile: PlayerPos not found, found " << buffer << std::endl;
        return;
    }

    playerStream >> buffer;
    float playerX = std::stof(buffer);

    playerStream >> buffer;
    float playerY = std::stof(buffer);

    // Create new player (the only entity in the list)
    entities.clear();
    entities.emplace_back(new Player(playerX, playerY));

    // Read CellGrid info
    std::string cellString;
    std::getline(fileIn, cellString);
    if (cellString != "CellGrid") {
        std::cerr << "World::loadFromFile: CellGrid not found, found " << cellString << std::endl;
        return;
    }

    // Set each cell to the correct value
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::getline(fileIn, cellString);

            Cell cell = deserialiseCell(cellString);
            grid.setCell(x, y, cell);
        }
    }

    // Sanity check: verify that the file ends when expected.
    std::getline(fileIn, cellString);
    if (cellString != "End")
        std::cerr << "World::loadFromFile: End not found, found " << cellString << std::endl;

    fileIn.close();
}


} // Naito
