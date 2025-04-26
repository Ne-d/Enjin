#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"


namespace Naito {

class Player : public Entity {
public:
    Player(float x, float y);

    void update() override;

private:
    float xInput;
    float yInput;
};

}


#endif //PLAYER_H
