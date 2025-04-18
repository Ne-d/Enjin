#ifndef GAME_H
#define GAME_H

#include "World.h"


namespace Naito {

constexpr unsigned int WORLD_WIDTH = 256;
constexpr unsigned int WORLD_HEIGHT = 192;


class Game {
public:
    static Game* get();
    [[nodiscard]] World* getWorld();
    [[nodiscard]] bool isRunning() const;

    [[nodiscard]] Element getSelectedElement() const;

    void drawGui();

    void quit();

private:
    Game();

    static Game* instance;
    World world;

    int selectedElement;

    bool running;
};
}

#endif //GAME_H
