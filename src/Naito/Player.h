#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"


namespace Naito {

class Player : public Entity {
public:
    Player(float x, float y);

    void update() override;
    void draw() override;

private:
    float xInput;
    float yInput;

    float moveSpeed = 1000.0f;
    float accelerationFactor = 0.15f;
    float gravity = 50.0f;
};

}


#endif //PLAYER_H
