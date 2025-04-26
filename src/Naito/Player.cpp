#include "Player.h"

#include <SDL_keyboard.h>
#include <SDL_render.h>

#include "Game.h"
#include "Render.h"

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
}

void Player::draw() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    const float x = static_cast<float>(cx) + rx;
    const float y = static_cast<float>(cy) + ry;

    float windowX, windowY;
    World* world = Game::get()->getWorld();
    world->worldToWindowCoordinates(x, y, &windowX, &windowY);

    const SDL_FRect rect = SDL_FRect{windowX, windowY, world->getCellSize(), world->getCellSize()};
    SDL_RenderRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}
