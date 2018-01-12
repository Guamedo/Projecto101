#ifndef PROJECT101_PLAYER_H
#define PROJECT101_PLAYER_H

#include "Agent.h"

class Player : public Agent {
public:
    Player();
    ~Player();

    void init(glm::vec2 pos, bool** keyStates, bool** speccialkeysStates, bool** keyStatesP, bool** speccialkeysStatesP);

    void draw();

    void update(const std::vector<std::string> & levelData);

private:
    void manageInput();

    float diag(const glm::vec2 & point1,const glm::vec2 & point2);
    glm::vec2 moveToPoint(const glm::vec2 & point1,const glm::vec2 & point2);

    bool** _keyStates;
    bool** _speccialkeysStates;

    bool** _keyStatesP;
    bool** _speccialkeysStatesP;

    glm::vec2 _headPosition;
    glm::vec2 _headSpeed;

    std::vector<glm::vec2> _facePointsX;
    std::vector<glm::vec2> _facePointsY;

    //Debug
    bool _sowCollidingFaces;
    //glm::vec2 _obstacle;
};


#endif //PROJECT101_PLAYER_H
