#ifndef PROJECT101_EXPLOSION_H
#define PROJECT101_EXPLOSION_H


#include <glm/glm.hpp>

class Explosion {
public:
    Explosion(glm::vec2 pos);

    ~Explosion();

    void draw();

private:

    glm::vec2 _position;
    float _rad;
};


#endif //PROJECT101_EXPLOSION_H
