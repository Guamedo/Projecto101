#ifndef PROJECT101_EXPLOSION_H
#define PROJECT101_EXPLOSION_H


#include <glm/glm.hpp>
#include "PerlinNoise.h"
#include "Player.h"

class Explosion {
public:
    Explosion(glm::vec2 pos);

    ~Explosion();

    bool update(Player* player);

    void draw();

private:

    PerlinNoise _perlinNoise;
    glm::vec2 _position;
    float _rad;
    float _maxRad;
    float _duration;
    float _maxDuration;

    float _randX;
    float _randY;
};


#endif //PROJECT101_EXPLOSION_H
