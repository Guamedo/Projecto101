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

void Agent::collideWithLevelAndUpdatePos(const std::vector<std::string> &levelData) {

    // Create the vector to store the colliding tiles
    std::vector<glm::vec2> collideTilePositions;

    // Un vector
    glm::vec2 vec;

    // Define the minimum distance between the agent and the tile
    float minDist = (float)TILE_SIZE / 2.0f + (float)AGENT_WIDTH / 2.0f;

    /********************
     * X Axes Collision *
     ********************/

    // Update player position in X axe

    _position.x += _speed.x;

    //Check agent horizontal direction
    glm::vec2 faceX[2];
    if(_speed.x >= 0/*Right face*/){
        std::cout << "Right face\n";
        faceX[0] = glm::vec2(_position.x + AGENT_WIDTH, _position.y);
        faceX[1] = glm::vec2(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);
    }else/*Left face*/{
        std::cout << "Left face\n";
        faceX[0] = glm::vec2(_position.x, _position.y);
        faceX[1] = glm::vec2(_position.x, _position.y + AGENT_WIDTH);
    }

    // Check for the tiles colliding with the agent
    vec = glm::vec2(floorf(faceX[0].x / (float)TILE_SIZE),
                    floorf(faceX[0].y / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] == 'G' || levelData[vec.y][vec.x] == 'W' || levelData[vec.y][vec.x] == 'C'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    vec = glm::vec2(floorf(faceX[1].x / (float)TILE_SIZE),
                    floorf(faceX[1].y / (float)TILE_SIZE));

    if(levelData[vec.y][vec.x] == 'G' || levelData[vec.y][vec.x] == 'W' || levelData[vec.y][vec.x] == 'C'){
        collideTilePositions.push_back(vec * (float)TILE_SIZE + glm::vec2((float)TILE_SIZE/ 2.0f));
    }

    // Manage the collisions
    for(glm::vec2 pos : collideTilePositions){

        glm::vec2 agentCenterPos = glm::vec2(_position.x + (float)AGENT_WIDTH / 2.0f, _position.y + (float)AGENT_WIDTH / 2.0f);
        glm::vec2 distVec = agentCenterPos - pos;

        float xDepth = minDist - fabsf(distVec.x);

        if(xDepth > 0){
            if(distVec.x < 0){
                _position.x -= xDepth + 1;
                _speed.x = 0;
                //_speed.y *= 0.7;
            }else{
                _position.x += xDepth + 1;
                _speed.x = 0;
                //_speed.y *= 0.7;
            }
        }
    }

    // Clean tile vector
    collideTilePositions.clear();

    /**********
     * Y Axes *
     **********/

    //Update player position in Y axe
    _position.y += _speed.y * (60.0f/1000.0f) + 120.0f * (60.0f/1000.0f) * (60.0f/1000.0f);

    //Check agent vertical direction
    glm::vec2 faceY[2];
    if(_speed.y >= 0/*Up face*/){
        std::cout << "Up face\n";
        faceY[0] = glm::vec2(_position.x, _position.y);
        faceY[1] = glm::vec2(_position.x + AGENT_WIDTH, _position.y);
    }else/*Down face*/{
        std::cout << "Down face\n";
        faceY[0] = glm::vec2(_position.x + AGENT_WIDTH, _position.y);
        faceY[1] = glm::vec2(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);
    }


    // Top left point
    vec = glm::vec2(floorf(_position.x / (float)TILE_SIZE),
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

    for(glm::vec2 pos : collideTilePositions){

        glm::vec2 agentCenterPos = glm::vec2(_position.x + (float)AGENT_WIDTH / 2.0f, _position.y + (float)AGENT_WIDTH / 2.0f);
        glm::vec2 distVec = agentCenterPos - pos;

        float yDepth = minDist - fabsf(distVec.y);

        if(yDepth > 0){
            if(distVec.y < 0){
                _position.y -= yDepth + 1;
                _speed.y = 0;
            }else{
                _position.y += yDepth;
                _speed.y = 0;
                _jump = 0;
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
