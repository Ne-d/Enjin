#include "Player.h"

#include <cmath>
#include <SDL_keyboard.h>
#include <SDL_render.h>

#include "Game.h"

using namespace Naito;

Player::Player(const float x, const float y) :
    Entity(x, y, 3, 5),
    xInput(0),
    yInput(0),
    jumpInput(false) {}

void Player::update() {
    Entity::update();

    const float frameTime = Game::get()->getFrameTime();

    // Get movement input
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    const float rightInput = keyboardState[SDL_SCANCODE_D] ? 1.0f : 0.0f;
    const float leftInput = keyboardState[SDL_SCANCODE_A] ? 1.0f : 0.0f;
    const float upInput = keyboardState[SDL_SCANCODE_W] ? 1.0f : 0.0f;
    const float downInput = keyboardState[SDL_SCANCODE_S] ? 1.0f : 0.0f;
    jumpInput = keyboardState[SDL_SCANCODE_SPACE];

    xInput = rightInput - leftInput;
    yInput = upInput - downInput;

    // Only apply movement if target speed is faster than current speed
    // Otherwise apply friction
    const float targetSpeed = xInput * moveSpeed;
    if (std::abs(targetSpeed) > std::abs(dx))
        dx += xInput * accelerationFactor * moveSpeed * frameTime;
    else
        dx *= std::pow(frictionX, frameTime);

    if (isOnGround && jumpInput) {
        dy -= jumpImpulse;
    }

    dy += gravity * frameTime;
    dy *= std::pow(frictionY, frameTime);

    updatePosition();
}

void Player::draw() {
    Entity::draw();
}

void Player::updatePosition() {
    Entity::updatePosition();

    Game* game = Game::get();
    World* world = game->getWorld();

    // Collisions
    constexpr float collisionThresholdX = 0.01f;
    constexpr int stepUpHeight = 3;

    isOnLeftWall = false;
    // X(-) Movement collisions
    do {
        if (world->hasCollision(cx - 1, cy - stepUpHeight, 1, static_cast<Uint16>(height - stepUpHeight)) && rx <=
            collisionThresholdX) {
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
        if (world->hasCollision(cx + static_cast<int>(width) + 1, cy - stepUpHeight, 1,
                                static_cast<Uint16>(height - stepUpHeight)) && rx >= 1 -
            collisionThresholdX) {
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
        if (world->hasCollision(cx, cy - static_cast<int>(height), static_cast<Uint16>(width) + 1, 1) && ry <
            0.0f) {
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
        if (world->hasCollision(cx, cy + 1, static_cast<Uint16>(width) + 1, 1) && ry >=
            0.99f) {
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

    // While the player's feet are in the ground and its head isn't against a ceiling
    while (world->hasCollision(cx, cy, static_cast<int>(width), 1) &&
        !world->hasCollision(cx, cy - static_cast<int>(height), static_cast<int>(width), 1)) {
        cy--;
    }
}
