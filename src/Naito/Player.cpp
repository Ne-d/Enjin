#include "Player.h"

#include <cmath>
#include <SDL_keyboard.h>
#include <SDL_render.h>

#include "Game.h"

using namespace Naito;

Player::Player(const float x, const float y) :
    Entity(x, y, 3, 5),
    xInput(0),
    yInput(0) {}

void Player::update() {
    Entity::update();

    const float frameTime = Game::get()->getFrameTime();

    // Get movement input
    const bool* keyboardState = SDL_GetKeyboardState(NULL);
    const float rightInput = keyboardState[SDL_SCANCODE_D] ? 1.0f : 0.0f;
    const float leftInput = keyboardState[SDL_SCANCODE_A] ? 1.0f : 0.0f;
    const float upInput = keyboardState[SDL_SCANCODE_W] ? 1.0f : 0.0f;
    const float downInput = keyboardState[SDL_SCANCODE_S] ? 1.0f : 0.0f;

    xInput = rightInput - leftInput;
    yInput = upInput - downInput;

    // Only apply movement if target speed is faster than current speed
    // Otherwise apply friction
    const float targetSpeed = xInput * moveSpeed;
    if (abs(targetSpeed) > abs(dx))
        dx += xInput * accelerationFactor * moveSpeed * Game::get()->getFrameTime();
    else
        dx *= std::pow(frictionX, Game::get()->getFrameTime());

    dy += gravity * Game::get()->getFrameTime();
    dy *= std::pow(frictionY, Game::get()->getFrameTime());

    updatePositionWithCollision();
}

void Player::draw() {
    Entity::draw();
}
