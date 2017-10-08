//
// Created by zztop9 on 08-Oct-17.
//

#ifndef PROJECT101_UTILES_H
#define PROJECT101_UTILES_H


#include <cmath>
#include "Vector2.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"
#include <stdlib.h>
#include <string>


float distance(float point1x, float point1y, float point2x, float point2y);
Vector2 moveToPoint(float point1x, float point1y, float point2x, float point2y);
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius);
int random_range(int min, int max);
void renderString(GLdouble x, GLdouble y, const std::string &string);




#endif //PROJECT101_UTILES_H
