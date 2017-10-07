#include "Player.h"

Player::Player() = default;

Player::~Player() = default;

void Player::init(glm::vec2 pos, glm::vec2 speed) {
    _position = pos;
    _speed = speed;
}

void Player::update(const std::vector<std::string> &levelData) {
    _position.y--;
    this->collideWithLevel(levelData);
}