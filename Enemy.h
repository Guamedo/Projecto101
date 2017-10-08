#ifndef PROJECT101_ENEMY_H
#define PROJECT101_ENEMY_H

#include "Agent.h"

class Enemy : public Agent {
public:
    Enemy();
    ~Enemy();

    void init(const glm::vec2& pos);

    void update(const std::vector<std::string> & levelData, const glm::vec2 playerPos);
private:

};


#endif //PROJECT101_ENEMY_H
