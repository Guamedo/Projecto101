#ifndef PROJECT101_PLAYER_H
#define PROJECT101_PLAYER_H

#include "Agent.h"

class Player : public Agent {
public:
    Player();
    ~Player();

    void init(glm::vec2 pos, bool** keyStates, bool** speccialkeysStates);

    void update(const std::vector<std::string> & levelData);
private:
    bool** _keyStates;
    bool** _speccialkeysStates;
};


#endif //PROJECT101_PLAYER_H
