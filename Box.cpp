#include "Box.h"

Box::Box() = default;

Box::~Box() = default;

Box::Box(glm::vec2 pos, float width) {
    _position = pos;
    _width = width;
}

const glm::vec2 &Box::getPosition() const {
    return _position;
}

float Box::getWidth() const {
    return _width;
}
