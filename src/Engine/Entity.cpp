#include "Entity.h"

#include <cmath>
#include <SDL_render.h>

#include "Game.h"
#include "Render.h"


namespace Naito {
class World;
}


using namespace Naito;

Entity::Entity(const float x, const float y) :
    cx(std::floor(x)),
    cy(std::floor(y)),
    rx(x - static_cast<float>(cx)),
    ry(y - static_cast<float>(cy)),
    dx(0), dy(0) {}

void Entity::update() {}

void Entity::draw() {
    SDL_SetRenderDrawColor(globalRenderer, 255, 255, 255, 255);
    const float x = static_cast<float>(cx) + rx;
    const float y = static_cast<float>(cy) + ry;

    float windowX, windowY;
    const World* world = Game::get()->getWorld();
    world->worldToWindowCoordinates(x, y, &windowX, &windowY);

    const SDL_FRect rect = SDL_FRect{windowX, windowY, world->getCellSize(), world->getCellSize()};
    SDL_RenderFillRect(globalRenderer, &rect);

    SDL_SetRenderDrawColor(globalRenderer, 0, 0, 0, 255);
}
