#include "Enemy.h"

Enemy::Enemy() = default;

Enemy::~Enemy() = default;

void Enemy::init(const glm::vec2 &pos) {
    _position = pos;
    _speed = glm::vec2(0.0f);
    _jump = 0;
}

void Enemy::update(const std::vector<std::string> &levelData, const glm::vec2 playerPos) {
    this->collideWithLevel(levelData);
}