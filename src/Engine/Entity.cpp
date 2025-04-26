#include "Entity.h"

#include <cmath>

using namespace Naito;

Entity::Entity(const float x, const float y) :
    cx(std::floor(x)),
    cy(std::floor(y)),
    rx(x - static_cast<float>(cx)),
    ry(y - static_cast<float>(cy)),
    dx(0), dy(0) {}
