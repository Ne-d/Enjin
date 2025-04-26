#ifndef ENTITY_H
#define ENTITY_H


namespace Naito {

class Entity {
public:
    Entity(float x, float y);
    virtual ~Entity() = default;

    virtual void update() = 0;
    virtual void move(float x, float y) = delete;
    virtual void draw() = delete;

private:
    int cx, cy; // Cell position
    float rx, ry; // Position in cell
    float dx, dy; // Velocity
};

}


#endif //ENTITY_H
