#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "Entity.h"
#include "Box.h"
#include "Action.h"
#include "Cacho.h"
#include "World.h"
#include <iostream>

using namespace std;

class Entity {
public:
    string name;
    vector<Action> actions;
    int actualAction = -1;
    int actualFrame = -1;
    int type;
    int jump = 0;
    int flip = 0;
    double flipGrades = M_PI/2.0f;
    Box BBox;
    Cacho head;
    vector<Cacho> tail;
    vector<Cacho> tailBody;
    Cacho body;

    void reset();
    void setSpeed(float x, float y);
    void setPosition(float x, float y);
    Vector2 getPosition();
    float distance(float point1x, float point1y, float point2x, float point2y);
    Vector2 moveToPoint(float point1x, float point1y, float point2x, float point2y);
    void newFrameMovePoints(World world);
    void newFrameMovePoints2(World world);

    void moveToPoint(float x, float y, float speed);
    void attack();
    void wave();

    Entity();
    ~Entity();
    Entity(string entityName, float x, float y, int entityType);
};