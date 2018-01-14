#ifndef PROJECT101_CAMERA_H
#define PROJECT101_CAMERA_H


#include <glm/glm.hpp>
#include "PerlinNoise.h"

class Camera {
public:
    Camera();
    ~Camera();

    void init(int width, int height);

    void update();

    void goToPosition(const glm::vec2& target);

    void resize(int width, int height);

    void trauma();

    glm::vec2 screenToWorld(glm::vec2 screenCoords);

    const glm::vec2 &getPosition() const;

    void setPosition(const glm::vec2& position);

    const glm::vec2 &getScale() const;

    void setScale(const glm::vec2& scale);

    float getRotationAngle() const;

    void setRotationAngle(float rotationAngle);

    float getTrauma() const;

    void setTrauma(float trauma);

private:
    int _width;
    int _height;

    glm::vec2 _position;
    glm::vec2 _scale;
    float _rotationAngle;
    float _trauma;
    float _traumaDuration;

    PerlinNoise _perlinNoise;
};


#endif //PROJECT101_CAMERA_H
