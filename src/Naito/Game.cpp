#include "Game.h"

#include <imgui.h>


namespace Naito {

Game* Game::instance = nullptr;

Game::Game(): world(World(SIMULATION_WIDTH, SIMULATION_HEIGHT)), running(true) {}

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

void Game::drawGui() {
    ImGui::Begin("Game");
    world.getCellGrid().drawGui();
    ImGui::End();
}

void Game::quit() {
    running = false;
}

}
