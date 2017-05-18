//
// Created by zztop9 on 13-May-17.
//
#pragma once
#include <string>
#include "Vector2.h"

class Cacho
{
public:
    Cacho();
    Cacho(int r);
    ~Cacho();
    void moveToPoint(float x, float y, float speed);

    int getRadio() const;
    void setRadio(int radio);

    const Vector2 &getPosition() const;
    void setPosition(const Vector2 &position);

    void setPositionX(const float positionX);

    void setPositionY(const float positionY);

    const Vector2 &getSpeed() const;
    void setSpeed(const Vector2 &speed);

    void setSpeedX(const float speedX);

    void setSpeedY(const float speedY);

private:
    int radio;
    Vector2 position;
    Vector2 speed;

};
