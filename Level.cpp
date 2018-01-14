#include "Level.h"
#include "Enemy.h"
#include "PerlinNoise.h"
#include <fstream>
#include <iostream>
#include <GL/gl.h>
#include <algorithm>

Level::Level(): _gravity(800.0f), _perlinNoise(PerlinNoise()){

}

Level::~Level() {

}

void Level::loadLevel(const std::string &levelPath) {
    std::fstream file;
    file.open(levelPath);

    if(file.fail()){
        std::cout << "Error reading level file: " + levelPath + "\n";
        exit(42);
    }

    std::string tmp;
    while(std::getline(file, tmp)){
        _levelData.push_back(tmp);
    }
    std::reverse(_levelData.begin(), _levelData.end());

    for(int y = 0; y < _levelData.size(); y++){
        for(int x = 0; x < _levelData[y].size(); x++){
            char tile = _levelData[y][x];
            switch (tile){
                case 'W':
                case 'G':
                case 'C':
                case 'H':
                    _levelDrawData.emplace_back(glm::vec2(x * TILE_SIZE, y * TILE_SIZE), TILE_SIZE);
                    break;
                case '@':
                    _levelData[y][x] = '.';
                    //_levelDrawData.emplace_back(glm::vec2(x * TILE_SIZE, y * TILE_SIZE), TILE_SIZE);
                    _playerInitialPos = glm::vec2(x * TILE_SIZE, y * TILE_SIZE);
                    break;
                case '.':
                    break;
                case 'E':
                    _levelData[y][x] = '.';
                    //_levelDrawData.emplace_back(glm::vec2(x * TILE_SIZE, y * TILE_SIZE), TILE_SIZE);
                    _enemyInitialPositions.emplace_back(x * TILE_SIZE, y * TILE_SIZE);
                    break;
                default:
                    std::cout << "Invalide character: " << tile << "\n";
                    break;
            }
        }
    }
}

void Level::drawLevel() {
    glBegin(GL_TRIANGLES);
    for(Box b : _levelDrawData){
        int tileY = (int)(floorf(b.getPosition().y)/(float)TILE_SIZE);
        int tileX = (int)(floorf(b.getPosition().x)/(float)TILE_SIZE);
        char tileType = _levelData[tileY][tileX];

        bool cosa = false;
        if(tileX-1 > 0){
            if(_levelData[tileY][tileX-1] == '.'){
                cosa = true;
            }
        }else{
            cosa = true;
        }

        if(tileX + 1 < _levelData[tileY].size()){
            if(_levelData[tileY][tileX + 1] == '.'){
                cosa = true;
            }
        }else{
            cosa = true;
        }

        if(tileY - 1 >= 0){
            if(_levelData[tileY - 1][tileX] == '.'){
                cosa = true;
            }
        }else{
            cosa = true;
        }

        float x = b.getPosition().x;
        float y = b.getPosition().y;

        float n = _perlinNoise.noise((x/(b.getWidth()/2.0f))*0.1f, (y/(b.getWidth()/2.0f))*0.1f);

        if(tileType == 'C'){
            glColor3f(n, n, n);
        }else if(tileType == 'W'){
            glColor3f(n*0.2f, n*0.2f, n*0.2f);
        }else if(tileType == 'G'){
            if(n < 0.2f && !cosa){
                glColor4f(n * 0.2f, n * 0.2f, (n+0.55f) * 0.78f, 0.5f);
                _levelData[tileY][tileX] = 'H';
            }else {
                glColor3f(n * 0.2f, n * 0.78f, n * 0.2f);
            }
        }else if(tileType == 'H'){
            glColor4f(n * 0.2f, n * 0.2f, (n+0.55f) * 0.78f, 0.5f);
        }

        glVertex2f(x, y);
        glVertex2f(x, y + b.getWidth());
        glVertex2f(x + b.getWidth(), y);

        glVertex2f(x + b.getWidth(), y);
        glVertex2f(x, y + b.getWidth());
        glVertex2f(x + b.getWidth(), y + b.getWidth());

    }
    glEnd();
}

const std::vector<std::string> &Level::getLevelData() const {
    return _levelData;
}

const glm::vec2 &Level::getPlayerInitialPos() const {
    return _playerInitialPos;
}

const std::vector<glm::vec2> &Level::getEnemyInitialPositions() const {
    return _enemyInitialPositions;
}
