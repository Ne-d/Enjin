#include "Game.h"

#include <imgui.h>


namespace Naito {

Game* Game::instance = nullptr;

Game::Game() :
    world(World(WORLD_WIDTH, WORLD_HEIGHT)), brushSize(3),
    selectedElement(0),
    running(true),
    frameTime(0) {}

int Game::getBrushSize() const { return brushSize; }

void Game::setFrameTime(const float frameTime) {
    this->frameTime = frameTime;
}

float Game::getFrameTime() const {
    return frameTime;
}

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

    if (ImGui::CollapsingHeader("Performance", ImGuiTreeNodeFlags_DefaultOpen)) {
        const float frameRate = 1.0f / frameTime;
        ImGui::Value("Framerate", frameRate);

        const float frameTimeMs = frameTime * 1000.0f;
        ImGui::Value("Frametime", frameTimeMs);
    }

    if (ImGui::CollapsingHeader("Level save and load", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Filename", levelFilename, 1024);

        // Weird shenanigans to avoid calling the function multiple times if the button is held down
        static int clickedSave = 0;
        if (ImGui::Button("Save"))
            clickedSave++;

        if (clickedSave & 1) {
            world.saveToFile(levelFilename);
            clickedSave = 0;
        }

        ImGui::SameLine();
        static int clickedLoad = 0;
        if (ImGui::Button("Load"))
            clickedLoad++;

        if (clickedLoad & 1) {
            world.loadFromFile(levelFilename);
            clickedLoad = 0;
        }
    }

    if (ImGui::CollapsingHeader("Placement", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputInt("Brush size", &brushSize);

        const char* elementNames[static_cast<Uint8>(Element::Count)];

        for (Uint8 i = 0; i < static_cast<Uint8>(Element::Count); ++i) {
            const char* name = elementName(static_cast<Element>(i));
            elementNames[i] = name;
        }

        ImGui::ListBox("Elements", &selectedElement, elementNames, static_cast<int>(Element::Count));
    }

    world.getCellGrid().drawGui();
    ImGui::End();
}

void Game::quit() {
    running = false;
}

}
