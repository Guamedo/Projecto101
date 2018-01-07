#include <GL/gl.h>
#include <iostream>
#include "Agent.h"
#include "Level.h"
#include "MainGame.h"

Agent::Agent() = default;

Agent::~Agent() = default;

void Agent::update(const std::vector<std::string> &levelData) {

}

void Agent::draw() {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle
    GLfloat twicePi = (GLfloat) (2.0f * 4 * atan(1));
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f, 0.0f, 1.0f);
    float x = _position.x + (float)AGENT_WIDTH / 2.0f;
    float y = _position.y + (float)AGENT_WIDTH / 2.0f;
    float radius = (float)AGENT_WIDTH/2.0f;
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
                (GLfloat) (x + (radius * cos(i * twicePi / triangleAmount))),
                (GLfloat) (y + (radius * sin(i * twicePi / triangleAmount)))
        );
    }
    glEnd();
}

void Agent::collideWithLevel(const std::vector<std::string> &levelData) {
    std::vector<glm::vec2> collideTilePositions;

    /**Check for corners**/

    // Top left point
    glm::vec2 vec = glm::vec2(floorf(_position.x / (float)TILE_SIZE),
                              floorf(_position.y / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] == 'G' || levelData[vec.y][vec.x] == 'W' || levelData[vec.y][vec.x] == 'C'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    // Top right
    vec = glm::vec2(floorf((_position.x + AGENT_WIDTH) / (float)TILE_SIZE),
                    floorf(_position.y / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] == 'G' || levelData[vec.y][vec.x] == 'W' || levelData[vec.y][vec.x] == 'C'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }


    // Bottom left
    vec = glm::vec2(floorf(_position.x / (float)TILE_SIZE),
                    floorf((_position.y + AGENT_WIDTH) / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] == 'G' || levelData[vec.y][vec.x] == 'W' || levelData[vec.y][vec.x] == 'C'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }


    // Bottom right
    vec = glm::vec2(floorf((_position.x + AGENT_WIDTH) / (float)TILE_SIZE),
                    floorf((_position.y + AGENT_WIDTH) / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] == 'G' || levelData[vec.y][vec.x] == 'W' || levelData[vec.y][vec.x] == 'C'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    float minDist = (float)TILE_SIZE / 2.0f + (float)AGENT_WIDTH / 2.0f;

    for(glm::vec2 pos : collideTilePositions){

        glm::vec2 agentCenterPos = glm::vec2(_position.x + (float)AGENT_WIDTH / 2.0f, _position.y + (float)AGENT_WIDTH / 2.0f);
        glm::vec2 distVec = agentCenterPos - pos;

        float xDepth = minDist - fabsf(distVec.x);
        float yDepth = minDist - fabsf(distVec.y);

        if(xDepth > 0 || yDepth > 0){
            if(xDepth < yDepth){
                // X axe collision
                if(distVec.x < 0){
                    _position.x -= xDepth;
                    _speed.x = 0;
                }else{
                    _position.x += xDepth ;
                    _speed.x = 0;
                }
            }else{
                // Y axe collision
                if(distVec.y < 0){
                    _position.y -= yDepth;
                    _speed.y = 0;

                }else{
                    _position.y += yDepth;
                    _speed.y = 0;
                    _jump = 0;
                }
            }
        }
    }
}

void Agent::collideWithLevelInX(const std::vector<std::string> &levelData, const std::vector<glm::vec2> &faceVec) {
    std::vector<glm::vec2> collideTilePositions;
    glm::vec2 vec;
    for(glm::vec2 v: faceVec){
        vec = glm::vec2(floorf(v.x/(float)TILE_SIZE),
                        floorf(v.y/(float)TILE_SIZE));
        if(levelData[vec.y][vec.x] == 'G' || levelData[vec.y][vec.x] == 'W' || levelData[vec.y][vec.x] == 'C'){
            //std::cout << "Collide in X" << "\n";
            collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
        }
    }

    // Define the minimum distance between the agent and the tile
    float minDist = (float)TILE_SIZE / 2.0f + (float)AGENT_WIDTH / 2.0f;
    // Manage the collisions
    for(glm::vec2 pos : collideTilePositions){

        glm::vec2 agentCenterPos = glm::vec2(_position.x + (float)AGENT_WIDTH / 2.0f, _position.y + (float)AGENT_WIDTH / 2.0f);
        glm::vec2 distVec = agentCenterPos - pos;

        float xDepth = minDist - fabsf(distVec.x);
        float yDepth = minDist - fabsf(distVec.y);

        if(xDepth >= 0 || yDepth >= 0){

            if(xDepth <= yDepth){
                // X axe collision
                if(distVec.x < 0){
                    _position.x -= xDepth;
                    _speed.x = 0;
                }else{
                    _position.x += xDepth ;
                    _speed.x = 0;
                }
            }
        }
    }
}

void Agent::collideWithLevelInY(const std::vector<std::string> &levelData, const std::vector<glm::vec2> &faceVec) {
    std::vector<glm::vec2> collideTilePositions;
    glm::vec2 vec;
    for(glm::vec2 v: faceVec){
        vec = glm::vec2(floorf(v.x/(float)TILE_SIZE),
                        floorf(v.y/(float)TILE_SIZE));

        if(levelData[vec.y][vec.x] == 'G' || levelData[vec.y][vec.x] == 'W' || levelData[vec.y][vec.x] == 'C'){
            //std::cout << "Collide in Y" << "\n";
            collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
        }
    }

    // Define the minimum distance between the agent and the tile
    float minDist = (float)TILE_SIZE / 2.0f + (float)AGENT_WIDTH / 2.0f;

    // Manage the collisions
    for(glm::vec2 pos : collideTilePositions){

        glm::vec2 agentCenterPos = glm::vec2(_position.x + (float)AGENT_WIDTH / 2.0f, _position.y + (float)AGENT_WIDTH / 2.0f);
        glm::vec2 distVec = agentCenterPos - pos;

        float xDepth = minDist - fabsf(distVec.x);
        float yDepth = minDist - fabsf(distVec.y);

        if(xDepth > 0 || yDepth > 0){
            if(xDepth > yDepth){
                // Y axe collision
                if(distVec.y < 0){
                    _position.y -= yDepth;
                    _speed.y = 0;

                }else{
                    _position.y += yDepth;
                    _speed.y = 0;
                    _jump = 0;
                }
            }
        }
    }
}

void Agent::collideAndUpdateInX(const std::vector<std::string> &levelData, const std::vector<glm::vec2> &faceVec) {

    _isInitX = false;

    glm::vec2 vec;

    glm::vec2 agentCenterPos = glm::vec2(_position.x + (float)AGENT_WIDTH / 2.0f, _position.y + (float)AGENT_WIDTH / 2.0f);
    float minDist = (float)TILE_SIZE / 2.0f + (float)AGENT_WIDTH / 2.0f;

    for(glm::vec2 v: faceVec){
        vec = glm::vec2(floorf(v.x/(float)TILE_SIZE),
                        floorf(v.y/(float)TILE_SIZE));

        if(_speed.x < 0){
            bool found = false;
            for(int i = (int)(vec.x); i >= 0 && !found; i--){
                if(levelData[vec.y][i] == 'G' || levelData[vec.y][i] == 'W' || levelData[vec.y][i] == 'C'){
                    glm::vec2 obstaclePos = glm::vec2(i, vec.y) * (float) TILE_SIZE + glm::vec2((float) TILE_SIZE / 2.0f);
                    if(!_isInitX) {
                        _obstacleX = obstaclePos;
                        //found = true;
                        _isInitX = true;
                    }else{
                        if(glm::distance(obstaclePos, agentCenterPos) < glm::distance(_obstacleX, agentCenterPos)){
                            _obstacleX = obstaclePos;
                        }
                    }
                }
            }
        }else if(_speed.x > 0){
            bool found = false;
            for(int i = (int)(vec.x); i < levelData[vec.y].size() && !found; i++){
                if(levelData[vec.y][i] == 'G' || levelData[vec.y][i] == 'W' || levelData[vec.y][i] == 'C'){
                    glm::vec2 obstaclePos = glm::vec2(i, vec.y) * (float) TILE_SIZE + glm::vec2((float) TILE_SIZE / 2.0f);
                    if(!_isInitX) {
                        _obstacleX = obstaclePos;
                        //found = true;
                        _isInitX = true;
                    }else{
                        if(glm::distance(obstaclePos, agentCenterPos) < glm::distance(_obstacleX, agentCenterPos)){
                            _obstacleX = obstaclePos;
                        }
                    }
                }
            }
        }
    }
    if(_speed.x < 0){
        float normalUpdate = fabsf(_speed.x * MainGame::_deltaTime);
        float collisionUpdate = fabsf((_obstacleX.x+(float)TILE_SIZE/2.0f)-(agentCenterPos.x-(float)AGENT_WIDTH/2.0f));
        if(normalUpdate < collisionUpdate){
            _position.x -= normalUpdate;
        }else{
            _position.x -= collisionUpdate;
            _speed.x = 0.0f;
            if(_speed.y < 0){
                _speed.y *= 0.85f;
            }
        }
        //_position.x -= std::fminf(fabsf(_speed.x * MainGame::_deltaTime), fabsf((_obstacleX.x+(float)TILE_SIZE/2.0f)-(agentCenterPos.x-(float)AGENT_WIDTH/2.0f)));
    }else if(_speed.x > 0){
        float normalUpdate = fabsf(_speed.x * MainGame::_deltaTime);
        float collisionUpdate = fabsf((_obstacleX.x-(float)TILE_SIZE/2.0f) - (agentCenterPos.x+(float)AGENT_WIDTH/2.0f));
        if(normalUpdate < collisionUpdate){
            _position.x += normalUpdate;
        }else{
            _position.x += collisionUpdate;
            _speed.x = 0.0f;
            if(_speed.y < 0){
                _speed.y *= 0.85f;
            }
        }
        //_position.x += std::fminf(fabsf(_speed.x * MainGame::_deltaTime), fabsf((_obstacleX.x-(float)TILE_SIZE/2.0f) - (agentCenterPos.x+(float)AGENT_WIDTH/2.0f)));
    }
}

void Agent::collideAndUpdateInY(const std::vector<std::string> &levelData, const std::vector<glm::vec2> &faceVec) {
    _isInitY = false;

    glm::vec2 vec;

    glm::vec2 agentCenterPos = glm::vec2(_position.x + (float)AGENT_WIDTH / 2.0f, _position.y + (float)AGENT_WIDTH / 2.0f);
    float minDist = (float)TILE_SIZE / 2.0f + (float)AGENT_WIDTH / 2.0f;

    for(glm::vec2 v: faceVec){
        vec = glm::vec2(floorf(v.x/(float)TILE_SIZE),
                        floorf(v.y/(float)TILE_SIZE));

        if(_speed.y < 0){
            bool found = false;
            for(int i = (int)(vec.y); i >= 0 && !found; i--){
                if(levelData[i][vec.x] == 'G' || levelData[i][vec.x] == 'W' || levelData[i][vec.x] == 'C'){
                    glm::vec2 obstaclePos = glm::vec2(vec.x, i) * (float) TILE_SIZE + glm::vec2((float) TILE_SIZE / 2.0f);
                    if(!_isInitY) {
                        _obstacleY = obstaclePos;
                        //found = true;
                        _isInitY = true;
                    }else{
                        if(glm::distance(obstaclePos.y, agentCenterPos.y) < glm::distance(_obstacleY.y, agentCenterPos.y)){
                            _obstacleY = obstaclePos;
                        }
                    }
                }
            }
        }else if(_speed.y > 0){
            bool found = false;
            for(int i = (int)(vec.y); i < levelData.size() && !found; i++){
                if(levelData[i][vec.x] == 'G' || levelData[i][vec.x] == 'W' || levelData[i][vec.x] == 'C'){
                    glm::vec2 obstaclePos = glm::vec2(vec.x, i) * (float) TILE_SIZE + glm::vec2((float) TILE_SIZE / 2.0f);
                    if(!_isInitY) {
                        _obstacleY = obstaclePos;
                        //found = true;
                        _isInitY = true;
                    }else{
                        if(glm::distance(obstaclePos.y, agentCenterPos.y) < glm::distance(_obstacleX.y, agentCenterPos.y)){
                            _obstacleY = obstaclePos;
                        }
                    }
                }
            }
        }
    }

    if(_speed.y < 0){
        float updateNormal = fabsf(_speed.y * MainGame::_deltaTime + MainGame::_gravity * MainGame::_deltaTime * MainGame::_deltaTime);
        float updateCollision = fabsf((_obstacleY.y+(float)TILE_SIZE/2.0f)-(agentCenterPos.y-(float)AGENT_WIDTH/2.0f));
        if(updateNormal < updateCollision){
            _position.y -= updateNormal;
        }else{
            _position.y -= updateCollision;
            _speed.y = 0.0f;
            _jump = 0;
        }
        //_position.y -= std::fminf(fabsf(_speed.y * MainGame::_deltaTime), fabsf((_obstacleY.y+(float)TILE_SIZE/2.0f)-(agentCenterPos.y-(float)AGENT_WIDTH/2.0f)));
    }else if(_speed.y > 0){
        float updateNormal = fabsf(_speed.y * MainGame::_deltaTime + MainGame::_gravity * MainGame::_deltaTime * MainGame::_deltaTime);
        float updateCollision = fabsf((_obstacleY.y-(float)TILE_SIZE/2.0f) - (agentCenterPos.y+(float)AGENT_WIDTH/2.0f));
        if(updateNormal < updateCollision){
            _position.y += updateNormal;
        }else{
            _position.y += updateCollision - 5;
            _speed.y = 0.0f;
        }
        //_position.y += std::fminf(fabsf(_speed.y * MainGame::_deltaTime), fabsf((_obstacleY.y-(float)TILE_SIZE/2.0f) - (agentCenterPos.y+(float)AGENT_WIDTH/2.0f)));
    }
}

const glm::vec2 &Agent::getPosition() const {
    return _position;
}

void Agent::setPosition(const glm::vec2 &position) {
    _position = position;
}

void Agent::setSpeed(const glm::vec2 &speed) {
    _speed = speed;
}
