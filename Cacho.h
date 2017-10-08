//
// Created by zztop9 on 13-May-17.
//
#pragma once
#include <string>
#include <glm/glm.hpp>

class Cacho
{
public:
    Cacho();
    Cacho(int r);
    ~Cacho();
    void moveToPoint(float x, float y, float speed);

    int getRadio() const;
    void setRadio(int radio);

    const glm::vec2 &getPosition() const;
    void setPosition(const glm::vec2 &position);

    void setPositionX(const float positionX);

    void setPositionY(const float positionY);

    const glm::vec2 &getSpeed() const;
    void setSpeed(const glm::vec2 &speed);

    void setSpeedX(const float speedX);

    void setSpeedY(const float speedY);

private:
    int radio;
    glm::vec2 position;
    glm::vec2 speed;

};
