#include "Game.h"

#include <imgui.h>


namespace Naito {

Game* Game::instance = nullptr;

Game::Game() :
    world(World(WORLD_WIDTH, WORLD_HEIGHT)),
    selectedElement(0),
    running(true) {}

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

Element Game::getSelectedElement() const {
    return static_cast<Element>(selectedElement);
}

void Game::drawGui() {
    ImGui::Begin("Game");

    ImGui::BeginTabBar("Main Tabs");
    if (ImGui::BeginTabItem("Elements")) {
        const char* elementNames[static_cast<Uint8>(Element::Count)];

        for (Uint8 i = 0; i < static_cast<Uint8>(Element::Count); ++i) {
            const char* name = elementName(static_cast<Element>(i));
            elementNames[i] = name;
        }

        ImGui::ListBox("Elements", &selectedElement, elementNames, static_cast<int>(Element::Count));

        ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("CellGrid")) {
        world.getCellGrid().drawGui();
        ImGui::EndTabItem();
    }
    ImGui::EndTabBar();

    ImGui::End();
}

void Game::quit() {
    running = false;
}

}
