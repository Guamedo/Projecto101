//
// Created by zztop9 on 13-May-17.
//
#include <cmath>
#include "Cacho.h"
Cacho::Cacho()
{
}
Cacho::Cacho(int r)
{
    radio=r;
}
Cacho::~Cacho()
{
}

void Cacho::moveToPoint(float x, float y, float speed){
    float vecX = x - position.x();
    float vecY = y - position.y();
    float mod = sqrtf(powf(vecX,2)+powf(vecY,2));
    float val = 1.5;
    if(mod >= 1){
        vecX = (vecX*speed)/mod;
        vecY = (vecY*speed)/mod;
        this->position = Vector2(this->position.x()+vecX*0.5f*std::fabs(mod+val), this->position.y()+vecY*0.5f*std::fabs(mod+val));
    }else{
        this->position = Vector2(x, y);
    }
}

int Cacho::getRadio() const {
    return radio;
}

void Cacho::setRadio(int radio) {
    Cacho::radio = radio;
}

const Vector2 &Cacho::getPosition() const {
    return position;
}

void Cacho::setPosition(const Vector2 &position) {

    this->position = position;

}

void Cacho::setPositionX(const float positionX) {
    this->position.x() = positionX;
}

void Cacho::setPositionY(const float positionY) {
    this->position.y() = positionY;
}

const Vector2 &Cacho::getSpeed() const {
    return speed;
}

void Cacho::setSpeed(const Vector2 &speed) {
    this->speed = speed;
}

void Cacho::setSpeedX(const float speedX) {
    this->speed.x() = speedX;
}

void Cacho::setSpeedY(const float speedY) {
    this->speed.y() = speedY;
}