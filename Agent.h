#ifndef PROJECT101_AGENT_H
#define PROJECT101_AGENT_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

const int AGENT_WIDTH = 30;

class Agent {
public:
    Agent();
    virtual ~Agent();

    virtual void update(const std::vector<std::string> & levelData);

    void draw();

    void collideWithLevel(const std::vector<std::string> & levelData);

    const glm::vec2 &getPosition() const;

protected:
    glm::vec2 _position;
    glm::vec2 _speed;
};


#endif //PROJECT101_AGENT_H
