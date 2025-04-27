#ifndef ENTITY_H
#define ENTITY_H


namespace Naito {

class Entity {
public:
    Entity(float x, float y);
    virtual ~Entity() = default;

    virtual void update();
    virtual void updatePositionWithCollision();
    virtual void draw();

protected:
    int cx, cy; // Cell position
    float rx, ry; // Position in cell
    float dx, dy; // Velocity

    float width = 1;
    float height = 1;

    bool isOnGround = false;
    bool isOnLeftWall = false;
    bool isOnRightWall = false;
};

}


#endif //ENTITY_H
