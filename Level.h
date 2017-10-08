#ifndef PROJECT101_LEVEL_H
#define PROJECT101_LEVEL_H

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include "Box.h"

const int TILE_SIZE = 50;

class Level {
public:
    Level();
    ~Level();

    void loadLevel(const std::string& levelPath);

    void drawLevel();

    const glm::vec2 &getPlayerInitialPos() const;

    const std::vector<glm::vec2> &getEnemyInitialPositions() const;

    const std::vector<std::string> &getLevelData() const;

private:
    std::vector<std::string> _levelData;
    glm::vec2 _playerInitialPos;
    std::vector<glm::vec2> _enemyInitialPositions;
    std::vector<Box> _levelDrawData;
};

#endif //PROJECT101_LEVEL_H
