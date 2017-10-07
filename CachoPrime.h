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

class CachoPrime {

    Vector2 position;
    Vector2 velocity;
    std::vector<CachoPrime> *attached;

    Vector2 obj; //Definirla en funcion de la posicion de otro Cacho/Entidad
    Vector2 desv;
    float marg;
    EntityPrime *father1;
    CachoPrime *father2;
    int tipo;
    float rozS;
    float rozA;
    float factorc;

public:

    CachoPrime(Vector2 pos, Vector2 vel, Vector2 des, float mar, float fc, CachoPrime *fath);
    CachoPrime(Vector2 pos, Vector2 vel, Vector2 des, float mar, float fc, EntityPrime *fath);
    ~CachoPrime();

    void setPosition(int x, int y);
    void setPosition(Vector2 p);
    void setVelocity(int x, int y);
    void setVelocity(Vector2 v);
    void setMarg(float margen);

    Vector2 getPosition();
    Vector2 GetVelocity();
    float getMarg();
    std::vector<CachoPrime>* getAttached();

    void updateObj();
    void calcMovement();

};


#endif //PROJECT101_CACHOPRIME_H
