#include <GL/gl.h>
#include <iostream>
#include "Player.h"
#include "MainGame.h"

Player::Player() = default;

Player::~Player() = default;

void Player::init(glm::vec2 pos, bool** keyStates, bool** speccialkeysStates, bool** keyStatesP, bool** speccialkeysStatesP) {
    _position = pos;
    _headPosition = glm::vec2(pos.x, pos.y +20);
    _speed = glm::vec2(0.0f);
    _headSpeed = glm::vec2(0.0f);
    _jump = 0;
    _keyStates = keyStates;
    _speccialkeysStates = speccialkeysStates;
    _keyStatesP = keyStatesP;
    _speccialkeysStatesP = speccialkeysStatesP;
    _sowCollidingFaces = false;
    _collideWaterY = false;
    _collideWaterX = false;
    //_obstacle = pos;
}

void Player::draw() {

    int triangleAmount = 20; //# of triangles used to draw circle
    auto twicePi = (GLfloat) (2.0f * 4 * atan(1));
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.78f, 0.2f, 0.2f);
    //Draw the body
    float x = _position.x + (float)AGENT_WIDTH / 2.0f;
    float y = _position.y + (float)AGENT_WIDTH / 2.0f;
    float radius = (float)AGENT_WIDTH/2.0f;
    glVertex2f(x, y); // center of circle
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
                (GLfloat) (x + (radius * cosf(i * twicePi / triangleAmount))),
                (GLfloat) (y + (radius * sinf(i * twicePi / triangleAmount)))
        );
    }
    glEnd();

    //Draw the head
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.78f, 0.78f, 0.2f);
    x = _headPosition.x + (float)AGENT_WIDTH / 2.0f;
    y = _headPosition.y + (float)AGENT_WIDTH / 2.0f;
    radius = (float)AGENT_WIDTH/4.0f;
    glVertex2f(x, y); // center of circle
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
                (GLfloat) (x + (radius * cosf(i * twicePi / triangleAmount))),
                (GLfloat) (y + (radius * sinf(i * twicePi / triangleAmount)))
        );
    }
    glEnd();

    if(_isInitX) {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f, 0.0f, 1.0f);
        x = _obstacleX.x;
        y = _obstacleX.y;
        radius = 5;
        glVertex2f(x, y); // center of circle
        for (int i = 0; i <= triangleAmount; i++) {
            glVertex2f(
                    (GLfloat) (x + (radius * cosf(i * twicePi / triangleAmount))),
                    (GLfloat) (y + (radius * sinf(i * twicePi / triangleAmount)))
            );
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        glVertex2f(_position.x + AGENT_WIDTH/2.0f, _position.y + AGENT_WIDTH/2.0f);
        glVertex2f(_obstacleX.x, _obstacleX.y);
        glEnd();
    }

    if(_isInitY) {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f, 0.0f, 1.0f);
        x = _obstacleY.x;
        y = _obstacleY.y;
        radius = 5;
        glVertex2f(x, y); // center of circle
        for (int i = 0; i <= triangleAmount; i++) {
            glVertex2f(
                    (GLfloat) (x + (radius * cosf(i * twicePi / triangleAmount))),
                    (GLfloat) (y + (radius * sinf(i * twicePi / triangleAmount)))
            );
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        glVertex2f(_position.x + AGENT_WIDTH/2.0f, _position.y + AGENT_WIDTH/2.0f);
        glVertex2f(_obstacleY.x, _obstacleY.y);
        glEnd();
    }

    if(_sowCollidingFaces){

        for(glm::vec2 v: _facePointsX){
            //Draw the head
            glBegin(GL_TRIANGLE_FAN);
            glColor3f(0.0f, 0.0f, 1.0f);
            x = v.x;
            y = v.y;
            radius = 5;
            glVertex2f(x, y); // center of circle
            for (int i = 0; i <= triangleAmount; i++) {
                glVertex2f(
                        (GLfloat) (x + (radius * cosf(i * twicePi / triangleAmount))),
                        (GLfloat) (y + (radius * sinf(i * twicePi / triangleAmount)))
                );
            }
            glEnd();
        }

        for(glm::vec2 v: _facePointsY){
            //Draw the head
            glBegin(GL_TRIANGLE_FAN);
            glColor3f(0.0f, 0.0f, 1.0f);
            x = v.x;
            y = v.y;
            radius = 5;
            glVertex2f(x, y); // center of circle
            for (int i = 0; i <= triangleAmount; i++) {
                glVertex2f(
                        (GLfloat) (x + (radius * cosf(i * twicePi / triangleAmount))),
                        (GLfloat) (y + (radius * sinf(i * twicePi / triangleAmount)))
                );
            }
            glEnd();
        }
    }
}

void Player::update(const std::vector<std::string> &levelData) {

    float gravity = MainGame::_gravity;
    // Set frame initial speed
    float newSpeedY = _speed.y - gravity * MainGame::_deltaTime;
    _speed = glm::vec2(0.0f, newSpeedY);

    // Update speed from input
    this->manageInput();


    //Update player position in X
    //_position.x += _speed.x * MainGame::_deltaTime;

    // Check the colliding face
    _facePointsX.clear();
    if(_speed.x < 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH-1.0f){
            _facePointsX.emplace_back(_position.x, _position.y + cosa);
            //_facePointsX.push_back(glm::vec2(_position.x, _position.y + cosa));
            cosa += TILE_SIZE/2.0f;
        }
        _facePointsX.emplace_back(_position.x, _position.y + AGENT_WIDTH - 1.0f);
        //_facePointsX.push_back(glm::vec2(_position.x, _position.y + AGENT_WIDTH - 1.0f));
    }else if(_speed.x > 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            _facePointsX.emplace_back(_position.x + AGENT_WIDTH, _position.y + cosa);
            //_facePointsX.push_back(glm::vec2(_position.x + AGENT_WIDTH, _position.y + cosa));
            cosa += TILE_SIZE/2.0f;
        }
        _facePointsX.emplace_back(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH - 1.0f);
        //_facePointsX.push_back(glm::vec2(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH - 1.0f));
    }

    //Collide in X with the level
    //this->collideWithLevelInX(levelData, _facePointsX);
    this->collideAndUpdateInX(levelData, _facePointsX);

    _facePointsX.clear();
    if(_speed.x < 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH-1.0f){
            _facePointsX.emplace_back(_position.x, _position.y + cosa);
            //_facePointsX.push_back(glm::vec2(_position.x, _position.y + cosa));
            cosa += TILE_SIZE/2.0f;
        }
        _facePointsX.emplace_back(_position.x, _position.y + AGENT_WIDTH - 1.0f);
        //_facePointsX.push_back(glm::vec2(_position.x, _position.y + AGENT_WIDTH - 1.0f));
    }else if(_speed.x > 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            _facePointsX.emplace_back(_position.x + AGENT_WIDTH, _position.y + cosa);
            //_facePointsX.push_back(glm::vec2(_position.x + AGENT_WIDTH, _position.y + cosa));
            cosa += TILE_SIZE/2.0f;
        }
        _facePointsX.emplace_back(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH - 1.0f);
        //_facePointsX.push_back(glm::vec2(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH - 1.0f));
    }

    this->collideWithLevelInX(levelData, _facePointsX);

    //Update player position in Y
    //_position.y += _speed.y * MainGame::_deltaTime + gravity * MainGame::_deltaTime * MainGame::_deltaTime;

    // Check the colliding face
    _facePointsY.clear();
    if(_speed.y < 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            _facePointsY.emplace_back(_position.x + cosa, _position.y);
            cosa += TILE_SIZE/2.0f;
        }
        _facePointsY.emplace_back(_position.x + AGENT_WIDTH - 1.0f, _position.y);
    }else if(_speed.y > 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            _facePointsY.emplace_back(_position.x + cosa, _position.y + AGENT_WIDTH);
            cosa += TILE_SIZE/2.0f;
        }
        _facePointsY.emplace_back(_position.x + AGENT_WIDTH - 1.0f, _position.y + AGENT_WIDTH);
    }

    //Collide in Y with the level
    //this->collideWithLevelInY(levelData, _facePointsY);
    this->collideAndUpdateInY(levelData, _facePointsY);

    _facePointsY.clear();
    if(_speed.y < 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            _facePointsY.emplace_back(_position.x + cosa, _position.y);
            cosa += TILE_SIZE/2.0f;
        }
        _facePointsY.emplace_back(_position.x + AGENT_WIDTH - 1.0f, _position.y);
    }else if(_speed.y > 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            _facePointsY.emplace_back(_position.x + cosa, _position.y + AGENT_WIDTH);
            cosa += TILE_SIZE/2.0f;
        }
        _facePointsY.emplace_back(_position.x + AGENT_WIDTH - 1.0f, _position.y + AGENT_WIDTH);
    }

    this->collideWithLevelInY(levelData, _facePointsY);

    //this->collideWithLevel(levelData);
    //_obstacle = _position;

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


    //this->collideWithBody();

    //Collide with level
    //this->collideWithLevel(levelData);
}

void Player::manageInput() {
    if((*_keyStates)['a'] || (*_keyStates)['A']){
        _speed.x = -STEP;
    }
    if((*_keyStates)['d'] || (*_keyStates)['D']){
        _speed.x = STEP;
    }
    if((*_keyStates)[32]/*SPACE*/){
        if (_jump == 0 && !(*_keyStatesP)[32]/*SPACE*/) {
            //player.body.setSpeedY(10);
            _speed.y = 2.0f*STEP;
            _jump = 1;
        }
        if(_collideWaterX || _collideWaterY){
            _speed.y = 2.0f*STEP;
        }
    }
    /*
    if(!(*_keyStates)[32]){
        if (_jump == 2) {
            _jump = 0;
        }
    }*/

    if((*_keyStates)['c'] || (*_keyStates)['C']){
        if(!(*_keyStatesP)['c'] && !(*_keyStatesP)['C']){
            std::cout << "c\n";
            _sowCollidingFaces = !_sowCollidingFaces;
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