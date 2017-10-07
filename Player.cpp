#include "Player.h"

Player::Player() = default;

Player::~Player() = default;

void Player::init(glm::vec2 pos, bool** keyStates, bool** speccialkeysStates) {
    _position = pos;
    _speed = glm::vec2(0.0f);
    _jump = 0;
    _keyStates = keyStates;
    _speccialkeysStates = speccialkeysStates;
}

void Player::update(const std::vector<std::string> &levelData) {
    _speed.x = 0;
    _speed.y -= 1;
    if((*_keyStates)['a'] || (*_keyStates)['A']){
        _speed.x -=STEP;
    }
    if((*_keyStates)['d'] || (*_keyStates)['D']){
        _speed.x +=STEP;
    }
    if((*_keyStates)[32]/*SPACE*/){
        if (_jump == 0 || _jump == 2) {
            //player.body.setSpeedY(10);
            _speed.y += 20;
            _jump++;
        }
    }
    if (!(*_keyStates)[32]/*SPACE_UP*/) {
        if (_jump == 1) {
            _jump++;
        }
    }
    _position += _speed;

    this->collideWithLevel(levelData);
}