#ifndef GAME_H
#define GAME_H

#include "World.h"


namespace Naito {

constexpr unsigned int WORLD_WIDTH = 256;
constexpr unsigned int WORLD_HEIGHT = 192;


/// A singleton that represents the state of the entire game.
class Game {
public:
    static Game* get();
    [[nodiscard]] World* getWorld();
    [[nodiscard]] bool isRunning() const;

    // These values are selected by the player in the GUI
    [[nodiscard]] Element getSelectedElement() const;
    [[nodiscard]] int getBrushSize() const;

    void setFrameTime(float frameTime);
    [[nodiscard]] float getFrameTime() const;

    void drawGui();

    void quit();

private:
    Game();

    static Game* instance;
    World world;

    int brushSize;
    int selectedElement;

    bool running;
    float frameTime;

    char levelFilename[1024] = "defaultLevel.txt";
};
}

#endif //GAME_H
