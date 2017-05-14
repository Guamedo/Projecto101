//
// Created by zztop9 on 13-May-17.
//
#pragma once
#include <array>
#include <string>
#include <vector>

class Cacho
{
public:
    Cacho();
    Cacho(int r);
    ~Cacho();
    void moveToPoint(double x, double y,double speed);

    int radio;
    std::array<double,2> position;
    std::array<double,2> speed;

    int getRadio() const;
    void setRadio(int radio);
    const std::array<double, 2> &getPosition() const;
    void setPosition(const std::array<double, 2> &position);
    const std::array<double, 2> &getSpeed() const;
    void setSpeed(const std::array<double, 2> &speed);
};
