//
// Created by zztop9 on 08-Oct-17.
//

#include "Utiles.h"

float distance(float point1x, float point1y, float point2x, float point2y) {
    float rectHeight = std::abs(point1y - point2y);
    float rectWidth = std::abs(point1x - point2x);
    return sqrtf(powf(rectHeight, 2.0) + powf(rectWidth, 2.0));
}

Vector2 moveToPoint(float point1x, float point1y, float point2x, float point2y){
    float dirVecX, dirVecY;
    float p2x = point2x - point1x;
    float p2y = point2y - point1y;
    float distancia = distance(point1x, point1y, point2x, point2y);
    if (distancia>0) {
        dirVecX = p2x / distancia;
        dirVecY = p2y / distancia;
    }else{
        dirVecX = 0;
        dirVecY = 1;
    }
    Vector2 cosa;
    cosa[0] = dirVecX;
    cosa[1] = dirVecY;
    return cosa;
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle
    GLfloat twicePi = (GLfloat) (2.0f * 4 * atan(1));
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
                (GLfloat) (x + (radius * cos(i * twicePi / triangleAmount))),
                (GLfloat) (y + (radius * sin(i * twicePi / triangleAmount)))
        );
    }
    glEnd();
}

int random_range(int min, int max) {
    return min + (rand() % (max - min + 1));
}

void renderString(GLdouble x, GLdouble y, const std::string &string)
{
    glColor3d(random_range(0,1), random_range(0,1), random_range(0,1));
    glRasterPos2d(x, y);
    int size = string.size();
    for (int n=0; n<size; ++n) {
        //glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[n]);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[n]);
    }
}