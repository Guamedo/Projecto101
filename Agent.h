#ifndef PROJECT101_AGENT_H
#define PROJECT101_AGENT_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

const int AGENT_WIDTH = 30;
const int STEP = 300;

class Agent {
public:
    Agent();
    virtual ~Agent();

    virtual void update(const std::vector<std::string> & levelData);

    virtual void draw();

    virtual void collideWithLevel(const std::vector<std::string> & levelData);

    virtual void collideWithLevelInX(const std::vector<std::string> & levelData, const std::vector<glm::vec2> & faceVec);
    virtual void collideWithLevelInY(const std::vector<std::string> & levelData, const std::vector<glm::vec2> & faceVec);

    virtual void collideAndUpdateInX(const std::vector<std::string> & levelData, const std::vector<glm::vec2> & faceVec);
    virtual void collideAndUpdateInY(const std::vector<std::string> & levelData, const std::vector<glm::vec2> & faceVec);

    const glm::vec2 &getPosition() const;
    const glm::vec2 &getSpeed() const;
    const glm::vec2 &getAcceleration() const;

    void setPosition(const glm::vec2 &position);
    void setSpeed(const glm::vec2 &speed);
    void setAcceleration(const glm::vec2 &acceleration);

protected:
    glm::vec2 _position;
    glm::vec2 _speed;
    glm::vec2 _acceleration;
    int _jump;

    glm::vec2 _obstacleX;
    bool _isInitX = false;

    glm::vec2 _obstacleY;
    bool _isInitY = false;

    bool _collideWaterX;
    bool _collideWaterY;
};


#endif //PROJECT101_AGENT_H
