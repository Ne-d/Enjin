#include "Entity.h"

#include <cmath>
#include <SDL_render.h>

#include "Game.h"
#include "Render.h"


namespace Naito {
class World;
}


using namespace Naito;

Entity::Entity(const float x, const float y, const float width, const float height) :
    cx(std::floor(x)), cy(std::floor(y)),
    rx(x - static_cast<float>(cx)), ry(y - static_cast<float>(cy)),
    dx(0), dy(0),
    width(width), height(height) {}

void Entity::update() {}

void Entity::updatePositionWithCollision() {
    Game* game = Game::get();
    World* world = game->getWorld();

    // Integrate position based on velocity
    rx += dx * game->getFrameTime();
    ry += dy * game->getFrameTime();

    // Collisions
    constexpr float collisionThresholdX = 0.01f;

    isOnLeftWall = false;
    // X(-) Movement collisions
    do {
        if (world->hasCollision(cx - 1, cy, width, height) && rx <= collisionThresholdX) {
            rx = collisionThresholdX;
            dx = 0.0f;
            isOnLeftWall = true;
        }
        if (rx < 0.0f) {
            cx--;
            rx++;
        }
    }
    while (rx < 0.0f);

    isOnRightWall = false;
    // X(+) Movement collisions
    do {
        if (world->hasCollision(cx + 2, cy, width, height) && rx >= 1 - collisionThresholdX) {
            rx = 1 - collisionThresholdX;
            dx = 0.0f;
            isOnRightWall = true;
        }
        if (rx > 1.0f) {
            cx++;
            rx--;
        }
    }
    while (rx > 1.0f);

    // Y(-) Movement collisions
    do {
        if (world->hasCollision(cx, cy - 1, width * 1, height + 1) && ry < 0.0f) {
            ry = 0.0f;
            dy = 0.0f;
        }

        isOnGround = false;

        if (ry < 0.0f) {
            cy--;
            ry++;
        }
    }
    while (ry < 0.0f);

    // Y(+) Movement collisions
    do {
        if (world->hasCollision(cx, cy + 1, width + 1, height) && ry >= 0.99f) {
            ry = 0.99f;
            dy = 0.0f;
            isOnGround = true;
        }
        else
            isOnGround = false;

        if (ry > 1.0f) {
            cy++;
            ry--;
        }
    }
    while (ry > 1.0f);
}

void Entity::draw() {
    SDL_SetRenderDrawColor(globalRenderer, 255, 255, 255, 255);
    const float x = static_cast<float>(cx) + rx;
    const float y = static_cast<float>(cy) + ry;

    float windowX, windowY;
    const World* world = Game::get()->getWorld();
    world->worldToWindowCoordinates(x, y, &windowX, &windowY);

    const SDL_FRect rect = SDL_FRect{windowX, windowY, width * world->getCellSize(), height * world->getCellSize()};
    SDL_RenderFillRect(globalRenderer, &rect);

    SDL_SetRenderDrawColor(globalRenderer, 0, 0, 0, 255);
}
