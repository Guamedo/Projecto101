//
// Created by zztop9 on 07-Oct-17.
//

#include "CachoPrime.h"

//funciones auxiliares
float distance(float point1x, float point1y, float point2x, float point2y) {
    float rectHeight = std::abs(point1y - point2y);
    float rectWidth = std::abs(point1x - point2x);
    return sqrtf(powf(rectHeight, 2.0) + powf(rectWidth, 2.0));
}

Vector2 moveToPoint(float point1x, float point1y, float point2x, float point2y){
    float dirVecX, dirVecY;
    float p2x = point2x - point1x;
    float p2y = point2y - point1y;
    float distancia = distance(point1x, point1y, point2x, point2y);
    if (distancia>0) {
        dirVecX = p2x / distancia;
        dirVecY = p2y / distancia;
    }else{
        dirVecX = 0;
        dirVecY = 1;
    }
    Vector2 cosa;
    cosa[0] = dirVecX;
    cosa[1] = dirVecY;
    return cosa;
}

CachoPrime::CachoPrime(Vector2 pos, Vector2 vel, Vector2 des, float mar, float fc, CachoPrime *fath){
    position = pos;
    velocity = vel;
    desv = des;
    marg = mar;
    rozS = 1;
    rozA = 0.97;
    factorc = fc;
    tipo = 2;
    father2 = fath;
    updateObj();
}
CachoPrime::CachoPrime(Vector2 pos, Vector2 vel, Vector2 des, float mar, float fc, EntityPrime *fath){
    position = pos;
    velocity = vel;
    desv = des;
    marg = mar;
    rozS = 1;
    rozA = 0.97;
    factorc = fc;
    tipo = 1;
    father1 = fath;

    updateObj();
}
CachoPrime::~CachoPrime(){

}

void CachoPrime::setPosition(int x, int y){
    position[0] = x;
    position[1] = y;
}
void CachoPrime::setPosition(Vector2 p){
    position[0] = p[0];
    position[1] = p[1];
}
void CachoPrime::setVelocity(int x, int y){
    velocity[0] = x;
    velocity[1] = y;
}
void CachoPrime::setVelocity(Vector2 v){
    velocity[0] = v[0];
    velocity[1] = v[1];
}
void CachoPrime::setMarg(float margen){
    marg = margen;
}

Vector2 CachoPrime::getPosition(){
    return position;
}
Vector2 CachoPrime::GetVelocity(){
    return velocity;
}
float CachoPrime::getMarg(){
    return marg;
}
std::vector<CachoPrime>* CachoPrime::getAttached(){
    return attached;
}

void CachoPrime::updateObj(){
    if (tipo == 1) {
        obj[0] = father1->getPosition()[0] + desv[0];
        obj[1] = father1->getPosition()[1] + desv[1];
    }
    else if (tipo == 2){
        obj[0] = father2->getPosition()[0] + desv[0];
        obj[1] = father2->getPosition()[1] + desv[1];
    }
}

void CachoPrime::calcMovement(){
    float posx = position[0] + velocity[0];
    float posy = position[1] + velocity[1];
    float velx = velocity[0] * rozA * rozS;
    float vely = (velocity[1] *(float)0.98) - 1;

    float dist = distance(obj[0],obj[1], posx, posy);
    Vector2 vect = moveToPoint(obj[0],obj[1], posx, posy);

    float correctionx = (marg - dist) * vect[0] * factorc;
    float correctiony = (marg - dist) * vect[1] * factorc;

    position[0] = posx + correctionx;
    position[1] = posy + correctiony;
    velocity[0] = velx + correctionx;
    velocity[1] = vely + correctiony;


    for (int i=0; i < attached->size(); i++){
        attached->at(i).updateObj();
        attached->at(i).calcMovement();
    }
}