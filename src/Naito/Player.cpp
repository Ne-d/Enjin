#include "Player.h"

#include <iostream>
#include <SDL_keyboard.h>

using namespace Naito;

Player::Player(const float x, const float y) : Entity(x, y), xInput(0), yInput(0) {}

void Player::update() {
    Entity::update();

    // Get movement input
    const bool* keyboardState = SDL_GetKeyboardState(NULL);
    const float rightInput = keyboardState[SDL_SCANCODE_D] ? 1.0f : 0.0f;
    const float leftInput = keyboardState[SDL_SCANCODE_A] ? 1.0f : 0.0f;
    const float upInput = keyboardState[SDL_SCANCODE_W] ? 1.0f : 0.0f;
    const float downInput = keyboardState[SDL_SCANCODE_S] ? 1.0f : 0.0f;

    xInput = rightInput - leftInput;
    yInput = upInput - downInput;

    std::cout << "Player::update: xInput : " << xInput << ", yInput : " << yInput << std::endl;
}
