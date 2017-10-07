//
// Created by zztop9 on 07-Oct-17.
//

#include "EntityPrime.h"

EntityPrime::EntityPrime(Vector2 pos, Vector2 vel, int mode, World *wrld){

    position = pos;
    velocity = vel;
    rozS = 0.66;
    rozA = 0.97;
    world = wrld;
    jump = 0;

    if (mode == 1){ //mode == 1 cuando se esta haciendo el jugador
        attached->push_back(CachoPrime(
                //los datos del cacho correspondiente a la cabeza
                Vector2(pos[0],pos[1]), Vector2(0,0), Vector2(4,0), (float)0.5, (float)0.55, this
        ));

        CachoPrime aux = CachoPrime(
                //los datos del cacho correspondiente a el primer cacho de la tail
                pos, Vector2(0,0), Vector2(0,0), 4, 1, this
        );
        //añadiendo segundo cacho de la tail
        aux.getAttached()->push_back(CachoPrime(pos, Vector2(0,0), Vector2(0,0), (float)4, 1, &aux));
        attached->push_back(aux);
    }
}
EntityPrime::~EntityPrime(){

}

void EntityPrime::setPosition(int x, int y){
    position[0] = x;
    position[1] = y;
}
void EntityPrime::setPosition(Vector2 p){
    position[0] = p[0];
    position[1] = p[1];
}
void EntityPrime::setVelocity(int x, int y){
    velocity[0] = x;
    velocity[1] = y;
}
void EntityPrime::setVelocity(Vector2 v){
    velocity[0] = v[0];
    velocity[1] = v[1];
}

Vector2 EntityPrime::getPosition(){
    return position;
}
Vector2 EntityPrime::GetVelocity(){
    return velocity;
}

void EntityPrime::reset(){

}

void EntityPrime::newFrameMovement(){
    float mainx = position[0] + velocity[0];
    float mainy = position[1] + velocity[1];
    float smainx = velocity[0] * rozA * rozS;
    float smainy = (velocity[1] *(float)0.98) - 1;

    regularMovement(Vector2(mainx, mainy), Vector2(smainx,smainy));

    for (unsigned int i=0; i < attached->size(); i++){
        attached->at(i).updateObj();
        attached->at(i).calcMovement();
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