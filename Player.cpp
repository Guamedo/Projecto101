#include <GL/gl.h>
#include <iostream>
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

    // Set frame initial speed
    _speed = glm::vec2(0.0f, _speed.y - 1.0f);

    // Update speed from input
    this->manageInput();

    //Update player position
    _position += _speed;

    // Calculate head speed
    /**
    glm::vec2 dirVec = glm::vec2(_position.x, _position.y +20) - _headPosition;
    _headSpeed = dirVec/5.0f;
    _headPosition += _headSpeed;
    **/

    //Calculate head speed procedural
    _headPosition +=_headSpeed;

    //_speed = glm::vec2(_speed.x * 0.98f, (_speed.y * 0.98f) + 1.0f);
    _headSpeed = glm::vec2(_headSpeed.x * 0.98f, (_headSpeed.y * 0.98f) + 1.0f);

    float diag = this->diag(glm::vec2(_position.x, _position.y + 15.0f), _headPosition);
    glm::vec2 rtrn = this->moveToPoint(glm::vec2(_position.x, _position.y + 15.0f), _headPosition);
    float getToDiag = 17.0f;

    /*_position = glm::vec2(_position.x + (getToDiag - diag) * rtrn.x * 0.5f,
                          _position.y + (getToDiag - diag) * rtrn.y * 0.5f);

    _speed = glm::vec2(_speed.x + (getToDiag - diag) * rtrn.x * 0.5f,
                       _speed.y + (getToDiag - diag) * rtrn.y * 0.5f);*/

    _headPosition = glm::vec2(_headPosition.x + (getToDiag - diag) * rtrn.x * 0.1f,
                              _headPosition.y + (getToDiag - diag) * rtrn.y * 0.1f);

    _headSpeed = glm::vec2(_headSpeed.x + (getToDiag - diag) * rtrn.x * 0.1f,
                           _headSpeed.y + (getToDiag - diag) * rtrn.y * 0.1f);

    this->collideWithBody();
    //Collide with level
    this->collideWithLevel(levelData);
    std::cout << "X= " << _speed.x << ", Y= " << _speed.y << "\n";
}

void Player::manageInput() {
    if((*_keyStates)['a'] || (*_keyStates)['A']){
        _speed.x -= STEP;
    }
    if((*_keyStates)['d'] || (*_keyStates)['D']){
        _speed.x += STEP;
    }
    if((*_keyStates)[32]/*SPACE*/){
        if (_jump == 0 || _jump == 2) {
            //player.body.setSpeedY(10);
            _speed.y += 15;
            _jump++;
        }
    }
    if (!(*_keyStates)[32]/*SPACE_UP*/) {
        if (_jump == 1) {
            _jump++;
        }
    }
}

float Player::diag(const glm::vec2 & point1, const glm::vec2 & point2) {
    return sqrtf(powf(point2.x + point1.x, 2.0f) + powf(point2.y - point1.y, 2.0f));
}

glm::vec2 Player::moveToPoint(const glm::vec2 & point1, const glm::vec2 & point2) {
    float point2x = point2.x - point1.x;
    float point2y = point2.y - point1.y;
    float diag = this->diag(point1, point2);
    glm::vec2 dirVec;
    if(diag > 0){
        dirVec = glm::vec2(point2x/diag, point2y/diag);
    }else{
        dirVec = glm::vec2(0.0f, 0.0f);
    }
    return dirVec;
}

void Player::collideWithBody() {
    float minDist = (float)AGENT_WIDTH / 4.0f + (float)AGENT_WIDTH / 2.0f;

    glm::vec2 headCenterPos = glm::vec2(_headPosition.x + (float)AGENT_WIDTH / 4.0f, _headPosition.y + (float)AGENT_WIDTH / 4.0f);
    glm::vec2 distVec = headCenterPos - (_position + (float)AGENT_WIDTH / 2.0f);

    float xDepth = minDist - fabsf(distVec.x);
    float yDepth = minDist - fabsf(distVec.y);

    if(xDepth > 0 || yDepth > 0){
        if(xDepth < yDepth){
            if(distVec.x < 0){
                _headPosition.x -= xDepth;
                _headSpeed.x = 0;
            }else{
                _headPosition.x += xDepth;
                _headSpeed.x = 0;
            }
        }else{
            if(distVec.y < 0){
                _headPosition.y -= yDepth;
                _headSpeed.y = 0;
            }else{
                _headPosition.y += yDepth;
                _headSpeed.y = 0;
            }
        }
    }
}