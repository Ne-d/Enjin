#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"


namespace Naito {

/// A player that is adapted from Enjminshake because the architecture was perfect for this.
class Player : public Entity {
public:
    Player(float x, float y);

    void update() override;

    void updatePosition() override;

private:
    float xInput = 0;
    float yInput = 0;
    bool jumpInput = false;

    float moveSpeed = 50.0f;
    float accelerationFactor = 5.0f;
    float gravity = 500.0f;

    float jumpImpulse = 100.0f;
};

}


#endif //PLAYER_H
