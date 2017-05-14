//
// Created by zztop9 on 13-May-17.
//
#include <cmath>
#include "Cacho.h"
Cacho::Cacho()
{
}
Cacho::~Cacho()
{
}

void Cacho::moveToPoint(double x, double y,double speed){
    double vecX = x - this->position[0];
    double vecY = y - this->position[1];
    double mod = sqrt(pow(vecX,2)+pow(vecY,2));
    double val = 1.5;
    if(mod >= 1){
        vecX = (vecX*speed)/mod;
        vecY = (vecY*speed)/mod;
        this->position = {this->position[0]+vecX*0.5*std::abs(mod+val), this->position[1]+vecY*0.5*std::abs(mod+val)};
    }else{
        this->position = {x,y};
    }
}

int Cacho::getRadio() const {
    return radio;
}

void Cacho::setRadio(int radio) {
    Cacho::radio = radio;
}

const std::array<double, 2> &Cacho::getPosition() const {
    return position;
}

void Cacho::setPosition(const std::array<double, 2> &position) {
    Cacho::position = position;
}

const std::array<double, 2> &Cacho::getSpeed() const {
    return speed;
}

void Cacho::setSpeed(const std::array<double, 2> &speed) {
    Cacho::speed = speed;
}
