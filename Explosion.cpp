#include <GL/gl.h>
#include "Explosion.h"

Explosion::Explosion(glm::vec2 pos) {
    _position = pos;
    _rad = 100.0f;
}

Explosion::~Explosion() {

}

void Explosion::draw() {
    int triangleAmount = 20; //# of triangles used to draw circle
    auto twicePi = (GLfloat) (2.0f * 4 * atan(1));
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.8f, 0.8f, 0.8f);
    //Draw the body
    float x = _position.x;
    float y = _position.y;
    float radius = _rad;
    glVertex2f(x, y); // center of circle
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
                (GLfloat) (x + (radius * cosf(i * twicePi / triangleAmount))),
                (GLfloat) (y + (radius * sinf(i * twicePi / triangleAmount)))
        );
    }
    glEnd();
}