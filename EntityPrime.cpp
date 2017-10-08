//
// Created by zztop9 on 07-Oct-17.
//

#include "EntityPrime.h"

EntityPrime::EntityPrime(Vector2 pos, Vector2 vel, int mode, float rad, World *wrld, int i){

    position = pos;
    velocity = vel;
    rozS = 0.66;
    rozA = 0.97;
    world = wrld;
    jump = 0;
    id = i;
    radio = rad;

    if (mode == 1){ //mode == 1 cuando se esta haciendo el jugador
        attached.push_back(new CachoPrime(Vector2(pos[0],pos[1]), Vector2(0,0), Vector2(0,4), 0.5, 0.55, 2, this, 64));
        attached.push_back(new CachoPrime(pos, Vector2(0,0), Vector2(0,0), 4, 1, 1, this, 11));
        attached.at(1)->addCacho(new CachoPrime(pos, Vector2(0,0), Vector2(0,0), 4, 1, 1, attached.at(1), 12));
        attached.at(1)->getAttached().at(0)->addCacho(new CachoPrime(pos, Vector2(0,0), Vector2(0,0), 4, 1, 1, attached.at(1)->getAttached().at(0), 13));
    }
    std::cout << "Constructora Entity " << i << "\n";
    print();
}
EntityPrime::~EntityPrime(){

}

void EntityPrime::setPosition(float x, float y){
    position[0] = x;
    position[1] = y;
}
void EntityPrime::setPosition(Vector2 p){
    position[0] = p[0];
    position[1] = p[1];
}
void EntityPrime::setVelocity(float x, float y){
    velocity[0] = x;
    velocity[1] = y;
}
void EntityPrime::setVelocity(Vector2 v){
    velocity[0] = v[0];
    velocity[1] = v[1];
}
void EntityPrime::setTonterias(const std::string &tonteria){
    tonterias = tonteria;
}
void EntityPrime::setJump(int val){
    jump = val;
}
void EntityPrime::setSprint(int val){
    sprint = val;
}
void EntityPrime::setRadio(float rad){
    radio = rad;
}

Vector2 EntityPrime::getPosition(){
    return position;
}
Vector2 EntityPrime::getVelocity(){
    return velocity;
}
std::string EntityPrime::getTonterias(){
    return tonterias;
}
std::vector<CachoPrime*> EntityPrime::getAttached(){
    return attached;
}
int EntityPrime::getJump(){
    return jump;
}
int EntityPrime::getSprint(){
    return sprint;
}
float EntityPrime::getRadio(){
    return radio;
}

void EntityPrime::reset(){

}

void EntityPrime::newFrameMovement(){
    float mainx = position[0] + velocity[0];
    float mainy = position[1] + velocity[1];
    float smainx = velocity[0] * rozA * rozS;
    float smainy = (velocity[1] *(float)0.98) - 1;

    regularMovement(Vector2(mainx, mainy), Vector2(smainx,smainy));

    for (unsigned int i=0; i < attached.size(); i++){
        attached.at(i)->updateObj();
        attached.at(i)->calcMovement();
    }
}

void EntityPrime::regularMovement(Vector2 newPos, Vector2 newVel){
    bool colX = false;
    int colY = -1;

    for(int i = 0; i < world->platforms.size(); i++){
        if(world->platforms[i].Overlaps(Box({newPos[0],position[1]},{3,3}))){
            colX = true;
        }
        if(world->platforms[i].Overlaps(Box({position[0],newPos[1]},{3,3}))) {
            colY = i;
        }
    }
    if(!colX){
        position[0] = newPos[0];
        velocity[0] = newVel[0];
    }else{
        velocity[0] = 0;
    }
    if(colY == -1){
        position[1] = newPos[1];
        velocity[1] = newVel[1];
    }else{
        if(position[1] > world->platforms[colY].center[1]){
            //Si esta pisando la plataforma
            position[1] =  world->platforms[colY].center[1] + world->platforms[colY].halfSize[1] + 4;
            jump = 0;
            velocity[1] = 0;
        }else{
            //Si la esta tocando por abajo
            position[1] = world->platforms[colY].center[1] - world->platforms[colY].halfSize[1] - 4;
            velocity[1] = 0;
        }
    }
}

void EntityPrime::drawEntity() {

    glColor3f(1.0, 0.0, 0.0);
    drawFilledCircle(position[0], position[1], radio);
    for (unsigned int i=0; i < attached.size(); i++){
        attached.at(i)->drawCacho();
    }
}

void EntityPrime::print(){
    std::cout << "Entity id "<< id <<"\n";
    for (unsigned int i=0; i < attached.size(); i++){
        std::cout << "L__{ ";
        attached.at(i)->print();
    }
}