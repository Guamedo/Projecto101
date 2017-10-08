#ifndef PROJECT101_BOX_H
#define PROJECT101_BOX_H

#include <glm/glm.hpp>

class Box {
public:
    Box();
    Box(glm::vec2 pos, float width);
    ~Box();

    const glm::vec2 &getPosition() const;

    float getWidth() const;

private:
    glm::vec2 _position;
    float _width;
};


#endif //PROJECT101_BOX_H
