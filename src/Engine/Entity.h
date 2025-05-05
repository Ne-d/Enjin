#ifndef ENTITY_H
#define ENTITY_H


namespace Naito {

/// An entity that is basically copy-pasted from Enjminshake.
class Entity {
public:
    Entity(float x, float y, float width = 1.0f, float height = 1.0f);
    virtual ~Entity() = default;

    virtual void update();
    virtual void updatePosition();
    virtual void draw();

    int cx, cy; // Cell position
    float rx, ry; // Position in cell
    float dx, dy; // Velocity

protected:
    float frictionX = 0.000001f;
    float frictionY = 0.91f;

    float width = 1;
    float height = 1;

    bool isOnGround = false;
    bool isOnLeftWall = false;
    bool isOnRightWall = false;
};

}


#endif //ENTITY_H
