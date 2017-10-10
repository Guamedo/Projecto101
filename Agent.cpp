#include <GL/gl.h>
#include <iostream>
#include "Agent.h"
#include "Level.h"

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

    //Check for corners
    glm::vec2 vec = glm::vec2(floorf(_position.x / (float)TILE_SIZE),
                              floorf(_position.y / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] == 'B'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    vec = glm::vec2(floorf((_position.x + AGENT_WIDTH) / (float)TILE_SIZE),
                    floorf(_position.y / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] == 'B'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    vec = glm::vec2(floorf(_position.x / (float)TILE_SIZE),
                    floorf((_position.y + AGENT_WIDTH) / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] == 'B'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    vec = glm::vec2(floorf((_position.x + AGENT_WIDTH) / (float)TILE_SIZE),
                    floorf((_position.y + AGENT_WIDTH) / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] == 'B'){
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
                if(distVec.x < 0){
                    _position.x -= xDepth;
                    _speed.x = 0;
                }else{
                    _position.x += xDepth;
                    _speed.x = 0;
                }
            }else{
                if(distVec.y < 0){
                    _position.y -= yDepth;
                    _speed.y *= 0.5;
                }else{
                    _position.y += yDepth;
                    _speed.y *= 0.5;
                    _jump = 0;
                }
            }
        }
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
