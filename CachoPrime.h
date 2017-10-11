//
// Created by zztop9 on 07-Oct-17.
//

#ifndef PROJECT101_CACHOPRIME_H
#define PROJECT101_CACHOPRIME_H

class EntityPrime;
#include <vector>
#include <cmath>
#include "Vector2.h"
#include "EntityPrime.h"
#include "Utiles.h"

class CachoPrime {

    Vector2 position;
    Vector2 velocity;
    std::vector<CachoPrime*> attached;

    Vector2 obj;
    Vector2 desv;
    float marg;
    EntityPrime *father1;
    CachoPrime *father2;
    int tipo;
    float rozS;
    float rozA;
    float factorc;
    float radio;
    int id;

public:

    CachoPrime(Vector2 pos, Vector2 vel, Vector2 des, float mar, float fc, float rad, float roza, float rozs, CachoPrime *fath, int i);
    CachoPrime(Vector2 pos, Vector2 vel, Vector2 des, float mar, float fc, float rad, float roza, float rozs, EntityPrime *fath, int i);
    ~CachoPrime();

    void setPosition(float x, float y);
    void setPosition(Vector2 p);
    void setVelocity(float x, float y);
    void setVelocity(Vector2 v);
    void setMarg(float margen);
    void setRadio(float);

    Vector2 getPosition();
    Vector2 GetVelocity();
    float getMarg();
    std::vector<CachoPrime*> getAttached();
    float getRadio();


    void updateObj();
    void calcMovement();
    void drawCacho();
    void print();
    void addCacho(CachoPrime *cacho);
};


#endif //PROJECT101_CACHOPRIME_H
