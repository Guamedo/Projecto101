#include <GL/gl.h>
#include "Explosion.h"
#include "MainGame.h"

Explosion::Explosion(glm::vec2 pos) {

    _position = pos;

    _maxRad = 200.0f;
    _rad = 0.0f;

    _maxDuration = 2.0f;
    _duration = _maxDuration;

    _randX = rand() % 255;
    _randY = rand() % 255;
}

Explosion::~Explosion() {

}

bool Explosion::update(Player* player) {
    _rad = _maxRad * (1.0f - _duration/_maxDuration);
    _duration -= MainGame::_deltaTime;

    //Collide with player
    if(glm::distance(player->getPosition() + glm::vec2(AGENT_WIDTH/2.0f), _position) <= _rad + AGENT_WIDTH/2.0f){
        //std::cout << "QUE DAÑITO\n";
        glm::vec2 dirVec = player->getPosition() - _position;
        dirVec = glm::normalize(dirVec);
        player->setAcceleration(glm::vec2(2000.0f*dirVec.x, 2000.0f*dirVec.y));
    }
    return _duration <= 0.0f;
}

void Explosion::draw() {
    int triangleAmount = 50; //# of triangles used to draw circle
    auto twicePi = (GLfloat) (2.0f * 4 * atan(1));
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    //Draw the body
    float x = _position.x;
    float y = _position.y;
    float radius = _rad;
    glVertex2f(x, y); // center of circle
    glColor4f(0.8f, 0.1f, 0.0f, 0.7f);
    for (int i = 0; i <= triangleAmount; i++) {
        float deviationX = 0.0f;//((_perlinNoise.noise((i%triangleAmount)*0.06f, _randX)*2.0f) - 1.0f) * 100.0f;
        float deviationY = 0.0f;//((_perlinNoise.noise((i%triangleAmount)*0.06f, _randY)*2.0f) - 1.0f) * 100.0f;
        glVertex2f(
                (GLfloat) (x + (radius * cosf(i * twicePi / triangleAmount)) + deviationX),
                (GLfloat) (y + (radius * sinf(i * twicePi / triangleAmount)) + deviationY)
        );
    }
    glEnd();
}