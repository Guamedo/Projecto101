#include <GL/gl.h>
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

    if(levelData[vec.y][vec.x] != '.'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    vec = glm::vec2(floorf((_position.x + AGENT_WIDTH) / (float)TILE_SIZE),
                    floorf(_position.y / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] != '.'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    vec = glm::vec2(floorf(_position.x / (float)TILE_SIZE),
                    floorf((_position.y + AGENT_WIDTH) / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] != '.'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    vec = glm::vec2(floorf((_position.x + AGENT_WIDTH) / (float)TILE_SIZE),
                    floorf((_position.y + AGENT_WIDTH) / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] != '.'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    float minDist = (float)TILE_SIZE / 2.0f + (float)AGENT_WIDTH / 2.0f;

    for(glm::vec2 pos : collideTilePositions){

        glm::vec2 agentCenterPos = _position + (float)AGENT_WIDTH / 2.0f;
        glm::vec2 distVec = agentCenterPos - pos;

        float xDepth = minDist - fabsf(distVec.x);
        float yDepth = minDist - fabsf(distVec.y);

        if(xDepth > 0 || yDepth > 0){
            if(xDepth < yDepth){
                if(distVec.x < 0){
                    _position.x -= xDepth;
                }else{
                    _position.x += xDepth;
                }
            }else{
                if(distVec.y < 0){
                    _position.y -= yDepth;
                }else{
                    _position.y += yDepth;
                }
            }
        }
    }
}

const glm::vec2 &Agent::getPosition() const {
    return _position;
}
