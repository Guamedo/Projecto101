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
        this->position = {this->position[0]+vecX*0.5*abs(mod+val), this->position[1]+vecY*0.5*abs(mod+val)};
    }else{
        this->position = {x,y};
    }
}