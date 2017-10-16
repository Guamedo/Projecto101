#ifndef PROJECT101_AGENT_H
#define PROJECT101_AGENT_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

const int AGENT_WIDTH = 30;
const int STEP = 10;

class Agent {
public:
    Agent();
    virtual ~Agent();

    virtual void update(const std::vector<std::string> & levelData);

    virtual void draw();

    virtual void collideWithLevel(const std::vector<std::string> & levelData);

    virtual void collideWithLevelV2(const std::vector<std::string> & levelData);

    const glm::vec2 &getPosition() const;

    void setPosition(const glm::vec2 &position);

    void setSpeed(const glm::vec2 &speed);

protected:
    glm::vec2 _position;
    glm::vec2 _speed;
    int _jump;
};


#endif //PROJECT101_AGENT_H
