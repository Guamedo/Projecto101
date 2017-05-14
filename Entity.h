#pragma once
#include "Entity.h"
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
    string name;
    vector<Action> actions;
    int actualAction = -1;
    int actualFrame = -1;
    int type;
    int jump = 0;
    int flip = 0;
    double flipGrades = M_PI/2;
    Box BBox;
    Cacho head;
    vector<Cacho> tail;
    Cacho body;

    void reset();
    void setSpeed(double x, double y);
    void setPosition(double x, double y);
    array<double, 2> getPosition();
    double distance(double point1x, double point1y, double point2x, double point2y);
    array<double,2> moveToPoint(double point1x, double point1y, double point2x, double point2y);
    void newFrameMovePoints(World world);
    void newFrameMovePoints2(World world);

    void moveToPoint(double x, double y,double speed);
    void attack();
    void wave();

    Entity();
    ~Entity();
    Entity(string entityName, double x, double y, int entityType);
};