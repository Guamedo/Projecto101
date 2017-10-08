#include "Level.h"
#include <fstream>
#include <iostream>
#include <GL/gl.h>
#include <algorithm>

Level::Level() {

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
                case 'B':
                    _levelDrawData.emplace_back(Vector2(x * TILE_SIZE + TILE_SIZE/2, y * TILE_SIZE + TILE_SIZE/2),
                                                Vector2(TILE_SIZE/2, TILE_SIZE/2));
                    break;
                case '@':
                    _playerInitialPos = glm::vec2(x * TILE_SIZE, y * TILE_SIZE);
                    break;
                case '.':
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
    glColor3f(0.42f, 0.6f, 0.5f);
    for(Box b : _levelDrawData){
        glVertex2f(b.getCenter()[0] + b.getHalfSize()[0],
                   b.getCenter()[1] + b.getHalfSize()[1]);
        glVertex2f(b.getCenter()[0] - b.getHalfSize()[0],
                   b.getCenter()[1] + b.getHalfSize()[1]);
        glVertex2f(b.getCenter()[0] - b.getHalfSize()[0],
                   b.getCenter()[1] - b.getHalfSize()[1]);

        glVertex2f(b.getCenter()[0] + b.getHalfSize()[0],
                   b.getCenter()[1] + b.getHalfSize()[1]);
        glVertex2f(b.getCenter()[0] - b.getHalfSize()[0],
                   b.getCenter()[1] - b.getHalfSize()[1]);
        glVertex2f(b.getCenter()[0] + b.getHalfSize()[0],
                   b.getCenter()[1] - b.getHalfSize()[1]);
    }
    glEnd();
}

const std::vector<std::string> &Level::getLevelData() const {
    return _levelData;
}

const glm::vec2 &Level::getPlayerInitialPos() const {
    return _playerInitialPos;
}
