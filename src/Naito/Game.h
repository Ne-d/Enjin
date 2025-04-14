#ifndef GAME_H
#define GAME_H

#include "World.h"


namespace Naito {

static constexpr unsigned int SIMULATION_WIDTH = 256;
static constexpr unsigned int SIMULATION_HEIGHT = 192;


class Game {
public:
    static Game* get();
    [[nodiscard]] World* getWorld();
    [[nodiscard]] bool isRunning() const;

    void drawGui();

    void quit();

private:
    Game();

    static Game* instance;
    World world;

    bool running;
};
}

#endif //GAME_H
