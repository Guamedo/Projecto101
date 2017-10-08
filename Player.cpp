#include <GL/gl.h>
#include "Player.h"

Player::Player() = default;

Player::~Player() = default;

void Player::init(glm::vec2 pos, bool** keyStates, bool** speccialkeysStates) {
    _position = pos;
    _headPosition = glm::vec2(pos.x, pos.y +20);
    _speed = glm::vec2(0.0f);
    _headSpeed = glm::vec2(0.0f);
    _jump = 0;
    _keyStates = keyStates;
    _speccialkeysStates = speccialkeysStates;
}

void Player::draw() {
    int triangleAmount = 20; //# of triangles used to draw circle
    GLfloat twicePi = (GLfloat) (2.0f * 4 * atan(1));
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 0.0f, 0.0f);
    //Draw the body
    float x = _position.x + (float)AGENT_WIDTH / 2.0f;
    float y = _position.y + (float)AGENT_WIDTH / 2.0f;
    float radius = (float)AGENT_WIDTH/2.0f;
    glVertex2f(x, y); // center of circle
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
                (GLfloat) (x + (radius * cos(i * twicePi / triangleAmount))),
                (GLfloat) (y + (radius * sin(i * twicePi / triangleAmount)))
        );
    }
    glEnd();

    //Draw the head
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 0.0f);
    x = _headPosition.x + (float)AGENT_WIDTH / 2.0f;
    y = _headPosition.y + (float)AGENT_WIDTH / 2.0f;
    radius = (float)AGENT_WIDTH/4.0f;
    glVertex2f(x, y); // center of circle
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
                (GLfloat) (x + (radius * cos(i * twicePi / triangleAmount))),
                (GLfloat) (y + (radius * sin(i * twicePi / triangleAmount)))
        );
    }
    glEnd();
}

void Player::update(const std::vector<std::string> &levelData) {
    _speed.x = 0;
    _speed.y -= 1;
    if((*_keyStates)['a'] || (*_keyStates)['A']){
        _speed.x -=STEP;
    }
    if((*_keyStates)['d'] || (*_keyStates)['D']){
        _speed.x +=STEP;
    }
    if((*_keyStates)[32]/*SPACE*/){
        if (_jump == 0 || _jump == 2) {
            //player.body.setSpeedY(10);
            _speed.y += 20;
            _jump++;
        }
    }
    if (!(*_keyStates)[32]/*SPACE_UP*/) {
        if (_jump == 1) {
            _jump++;
        }
    }

    _position += _speed;

    glm::vec2 dirVec = glm::vec2(_position.x, _position.y +20) - _headPosition;
    _headSpeed = dirVec/5.0f;
    _headPosition += _headSpeed;

    this->collideWithLevel(levelData);
}