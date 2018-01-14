#include <algorithm>
#include "Enemy.h"
#include "Level.h"

Enemy::Enemy() = default;

Enemy::~Enemy() = default;

void Enemy::init(const glm::vec2 &pos) {
    _position = pos;
    _speed = glm::vec2(0.0f);
    _acceleration = glm::vec2(0.0f);
    _jump = 0;
}

void Enemy::update(const std::vector<std::string> &levelData, const glm::vec2& playerPos) {

    if(playerPos.x < _position.x){
        _speed.x = -STEP/2.0f;
    }else if(playerPos.x > _position.x){
        _speed.x = STEP/2.0f;
    }

    std::vector<glm::vec2> facePointsX;
    if(_speed.x < 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH-1.0f){
            facePointsX.emplace_back(_position.x, _position.y + cosa);
            //_facePointsX.push_back(glm::vec2(_position.x, _position.y + cosa));
            cosa += TILE_SIZE/2.0f;
        }
        facePointsX.emplace_back(_position.x, _position.y + AGENT_WIDTH - 1.0f);
        //_facePointsX.push_back(glm::vec2(_position.x, _position.y + AGENT_WIDTH - 1.0f));
    }else if(_speed.x > 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            facePointsX.emplace_back(_position.x + AGENT_WIDTH, _position.y + cosa);
            //_facePointsX.push_back(glm::vec2(_position.x + AGENT_WIDTH, _position.y + cosa));
            cosa += TILE_SIZE/2.0f;
        }
        facePointsX.emplace_back(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH - 1.0f);
        //_facePointsX.push_back(glm::vec2(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH - 1.0f));
    }

    collideAndUpdateInX(levelData, facePointsX);

    facePointsX.clear();
    if(_speed.x < 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH-1.0f){
            facePointsX.emplace_back(_position.x, _position.y + cosa);
            //_facePointsX.push_back(glm::vec2(_position.x, _position.y + cosa));
            cosa += TILE_SIZE/2.0f;
        }
        facePointsX.emplace_back(_position.x, _position.y + AGENT_WIDTH - 1.0f);
        //_facePointsX.push_back(glm::vec2(_position.x, _position.y + AGENT_WIDTH - 1.0f));
    }else if(_speed.x > 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            facePointsX.emplace_back(_position.x + AGENT_WIDTH, _position.y + cosa);
            //_facePointsX.push_back(glm::vec2(_position.x + AGENT_WIDTH, _position.y + cosa));
            cosa += TILE_SIZE/2.0f;
        }
        facePointsX.emplace_back(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH - 1.0f);
        //_facePointsX.push_back(glm::vec2(_position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH - 1.0f));
    }

    this->collideWithLevelInX(levelData, facePointsX);

    std::vector<glm::vec2> facePointsY;
    if(_speed.y < 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            facePointsY.emplace_back(_position.x + cosa, _position.y);
            cosa += TILE_SIZE/2.0f;
        }
        facePointsY.emplace_back(_position.x + AGENT_WIDTH - 1.0f, _position.y);
    }else if(_speed.y > 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            facePointsY.emplace_back(_position.x + cosa, _position.y + AGENT_WIDTH);
            cosa += TILE_SIZE/2.0f;
        }
        facePointsY.emplace_back(_position.x + AGENT_WIDTH - 1.0f, _position.y + AGENT_WIDTH);
    }

    this->collideAndUpdateInY(levelData, facePointsY);

    facePointsY.clear();
    if(_speed.y < 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            facePointsY.emplace_back(_position.x + cosa, _position.y);
            cosa += TILE_SIZE/2.0f;
        }
        facePointsY.emplace_back(_position.x + AGENT_WIDTH - 1.0f, _position.y);
    }else if(_speed.y > 0){
        float cosa = 1.0f;
        while(cosa <= AGENT_WIDTH - 1.0f){
            facePointsY.emplace_back(_position.x + cosa, _position.y + AGENT_WIDTH);
            cosa += TILE_SIZE/2.0f;
        }
        facePointsY.emplace_back(_position.x + AGENT_WIDTH - 1.0f, _position.y + AGENT_WIDTH);
    }

    this->collideWithLevelInY(levelData, facePointsY);

    //_position += _speed;

    //this->collideWithLevel(levelData);
}

/*void Enemy::collideWithLevel(const std::vector<std::string> &levelData) {
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
}*/