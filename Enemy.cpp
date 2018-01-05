#include <algorithm>
#include "Enemy.h"
#include "Level.h"

Enemy::Enemy() = default;

Enemy::~Enemy() = default;

void Enemy::init(const glm::vec2 &pos) {
    _position = pos;
    _speed = glm::vec2(0.0f);
    _jump = 0;
}

void Enemy::update(const std::vector<std::string> &levelData, const glm::vec2 playerPos) {

    _speed.x = 0;
    _speed.y -= 1;
    if(playerPos.x < _position.x){
        _speed.x -= 5;
    }else if(playerPos.x > _position.x){
        _speed.x += 5;
    } else{
        _speed.x = 0;
    }
    if(_jump == 1){
        _speed.y = 15;
        _jump = 2;
    }

    _position += _speed;

    this->collideWithLevel(levelData);
}

void Enemy::collideWithLevel(const std::vector<std::string> &levelData) {
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
    float minDistJ = minDist + AGENT_WIDTH;

    for(glm::vec2 pos : collideTilePositions){

        glm::vec2 agentCenterPos = glm::vec2(_position.x + (float)AGENT_WIDTH / 2.0f, _position.y + (float)AGENT_WIDTH / 2.0f);
        glm::vec2 distVec = agentCenterPos - pos;

        float xDepth = minDist - fabsf(distVec.x);
        float yDepth = minDist - fabsf(distVec.y);

        if(xDepth > 0 || yDepth > 0){
            if(xDepth <= yDepth){
                _jump = 1;
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