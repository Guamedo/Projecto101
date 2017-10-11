//
// Created by zztop9 on 07-Oct-17.
//

#include "CachoPrime.h"

CachoPrime::CachoPrime(Vector2 pos, Vector2 vel, Vector2 des, float mar, float fc, float rad, float roza, float rozs, CachoPrime *fath, int i){
    position = pos;
    velocity = vel;
    desv = des;
    marg = mar;
    factorc = fc;
    tipo = 2;
    father2 = fath;
    radio = rad;
    rozA = roza;
    rozS = rozs;
    updateObj();
    id=i;
    std::cout << "Constructora cacho(cacho) " << i <<" -- " << fath->id<< "\n";
}
CachoPrime::CachoPrime(Vector2 pos, Vector2 vel, Vector2 des, float mar, float fc, float rad, float roza, float rozs, EntityPrime *fath, int i){
    position = pos;
    velocity = vel;
    desv = des;
    marg = mar;
    factorc = fc;
    tipo = 1;
    father1 = fath;
    radio = rad;
    rozA = roza;
    rozS = rozs;
    updateObj();
    id=i;
    std::cout << "Constructora cacho(entity) " << i <<" -- " << "incaccesible"<< "\n";
}
CachoPrime::~CachoPrime(){

}

void CachoPrime::setPosition(float x, float y){
    position[0] = x;
    position[1] = y;
}
void CachoPrime::setPosition(Vector2 p){
    position[0] = p[0];
    position[1] = p[1];
}
void CachoPrime::setVelocity(float x, float y){
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
void CachoPrime::setRadio(float rad){
    radio = rad;
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
std::vector<CachoPrime*> CachoPrime::getAttached(){
    return attached;
}
float CachoPrime::getRadio(){
    return radio;
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
    float vely = (velocity[1] *(float)0.78) - 1;

    float dist = distance(obj[0],obj[1], posx, posy);
    Vector2 vect = moveToPoint(obj[0],obj[1], posx, posy);

    float correctionx = (marg - dist) * vect[0] * factorc;
    float correctiony = (marg - dist) * vect[1] * factorc;

    position[0] = posx + correctionx;
    position[1] = posy + correctiony;
    velocity[0] = velx + correctionx;
    velocity[1] = vely + correctiony;


    for (int i=0; i < attached.size(); i++){
        attached.at(i)->updateObj();
        attached.at(i)->calcMovement();
    }
}

void CachoPrime::drawCacho(){
    for (unsigned int i=0; i < attached.size(); i++){
        attached.at(i)->drawCacho();
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(position[0], position[1], radio);
}

void CachoPrime::print(){
    std::cout << "cacho id "<< id <<"\n";
    for (unsigned int i=0; i < attached.size(); i++){
        std::cout << "     ->";
        attached.at(i)->print();
    }
}

void CachoPrime::addCacho(CachoPrime* cacho){
    attached.push_back(cacho);
}