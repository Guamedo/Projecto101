#pragma once
#include "Box.h"
#include "Action.h"
#include "Cacho.h"
#include "World.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;

class Entity {
public:

    Entity();
    ~Entity();
    Entity(string entityName, float x, float y, int entityType);

    void reset();
    void setSpeed(float x, float y);
    void setPosition(float x, float y);
    Vector2 getPosition();
    float distance(float point1x, float point1y, float point2x, float point2y);
    Vector2 moveToPoint(float point1x, float point1y, float point2x, float point2y);
    void newFrameMovePoints(World world);
    void newFrameMovePoints2(World world);
    void setSprint(int val);
    int getSprint();

    void moveToPoint(float x, float y, float speed);
    void attack();
    void wave();

    const string &getTonterias() const;

    void setTonterias(const string &tonterias);

    string name;
    string tonterias;
    vector<Action> actions;
    int actualAction = -1;
    int actualFrame = -1;
    int type;
    int sprint;
    int jump = 0;
    int flip = 0;
    double flipGrades = M_PI/2.0f;
    Box BBox;
    Cacho head;
    vector<Cacho> tail;
    vector<Cacho> tailBody;
    Cacho body;

};