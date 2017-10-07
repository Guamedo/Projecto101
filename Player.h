#ifndef PROJECT101_PLAYER_H
#define PROJECT101_PLAYER_H

#include "Agent.h"

class Player : public Agent {
public:
    Player();
    ~Player();

    void init(glm::vec2 pos, glm::vec2 speed);

    void update(const std::vector<std::string> & levelData);
private:
};


#endif //PROJECT101_PLAYER_H
