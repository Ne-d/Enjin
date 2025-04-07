#include "Game.h"


namespace Naito {

Game* Game::instance = nullptr;

Game::Game(): world(World(SCREEN_WIDTH, SCREEN_HEIGHT)), running(true) {}

Game* Game::get() {
    if (instance == nullptr)
        instance = new Game();

    return instance;
}

World* Game::getWorld() {
    return &world;
}

bool Game::isRunning() const {
    return running;
}

void Game::quit() {
    running = false;
}

}
