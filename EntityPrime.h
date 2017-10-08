//
// Created by zztop9 on 07-Oct-17.
//

#ifndef PROJECT101_ENTITYPRIME_H
#define PROJECT101_ENTITYPRIME_H

class CachoPrime;
#include "Vector2.h"
#include "CachoPrime.h"
#include "World.h"
#include <iostream>
#include <vector>
#include "Utiles.h"

class EntityPrime {

    Vector2 position;
    Vector2 velocity;
    std::vector<CachoPrime*> attached;

    float rozS;
    float rozA;
    World *world;
    std::string tonterias;
    int jump;
    int sprint;
    float radio;
    int id;


public:
    EntityPrime(Vector2 pos, Vector2 vel, int mode, float rad, World *wrld, int id);
    ~EntityPrime();

    void setPosition(float x, float y);
    void setPosition(Vector2 p);
    void setVelocity(float x, float y);
    void setVelocity(Vector2 v);
    void setTonterias(const std::string &tonteria);
    void setJump(int);
    void setSprint(int);
    void setRadio(float);

    Vector2 getPosition();
    Vector2 getVelocity();
    std::string getTonterias();
    std::vector<CachoPrime*> getAttached();
    int getJump();
    int getSprint();
    float getRadio();

    void reset();
    void newFrameMovement();
    void regularMovement(Vector2 newPos, Vector2 newVel);
    void drawEntity();
    void print();
};


#endif //PROJECT101_ENTITYPRIME_H
