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

class EntityPrime {

    Vector2 position;
    Vector2 velocity;
    std::vector<CachoPrime> *attached;

    float rozS;
    float rozA;
    World *world;
    int jump;

public:
    EntityPrime(Vector2 pos, Vector2 vel, int mode, World *wrld);
    ~EntityPrime();

    void setPosition(int x, int y);
    void setPosition(Vector2 p);
    void setVelocity(int x, int y);
    void setVelocity(Vector2 v);

    Vector2 getPosition();
    Vector2 GetVelocity();

    void reset();
    void newFrameMovement();
    void regularMovement(Vector2 newPos, Vector2 newVel);
};


#endif //PROJECT101_ENTITYPRIME_H
