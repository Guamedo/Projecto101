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
    ~Cacho();
    int radio;
    std::array<double,2> position;
    std::array<double,2> speed;
};
