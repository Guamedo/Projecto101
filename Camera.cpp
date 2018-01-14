#include <GL/gl.h>
#include <cstdlib>
#include "Camera.h"
#include "MainGame.h"

Camera::Camera() = default;

Camera::~Camera() = default;

void Camera::init(int width, int height) {
    _width = width;
    _height = height;

    _position = glm::vec2(0.0f);
    _scale = glm::vec2(1.0f);
    _rotationAngle = 0.0f;
    _trauma = 0.0f;
    _traumaDuration = 1.0f;
}

void Camera::update() {

    //Set viewport
    glViewport(0, 0, _width, _height);

    //Load the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Scale the camera
    glScalef(_scale.x, _scale.y, 1.0f);

    // Set rotation trauma
    float trauma3 = _trauma * _trauma * _trauma;
    float intensity = 30.0f;
    float random = (_perlinNoise.noise(MainGame::_time*60.0f)*2.0f - 1.0f) * intensity * trauma3;
    glRotatef(_rotationAngle + random, 0.0f, 0.0f, 1.0f);

    // Set translation trauma
    intensity = 200.0f;
    float randomX = (_perlinNoise.noise(MainGame::_time*60.0f, 0.0f)*2.0f - 1.0f) * intensity * trauma3;//(((float(rand())/RAND_MAX) * 2.0f) - 1.0f) * intensity * trauma3;
    float randomY = (_perlinNoise.noise(MainGame::_time*60.0f, 50.0f)*2.0f - 1.0f) * intensity * trauma3;//(((float(rand())/RAND_MAX) * 2.0f) - 1.0f) * intensity * trauma3;
    _position.x += randomX;
    _position.y += randomY;

    // Set new camera position
    glOrtho(0.0f + _position.x, _width + _position.x,
            0.0f + _position.y, _height + _position.y,
            0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    _trauma = std::fmaxf(_trauma - MainGame::_deltaTime, 0.0f);
}

void Camera::goToPosition(const glm::vec2& target) {
    glm::vec2 dirVec = target - _position;
    _position += dirVec/10.0f;
}

void Camera::resize(int width, int height) {
    _width = width;
    _height = height;
}

void Camera::trauma() {
    _trauma = _traumaDuration;
}

glm::vec2 Camera::screenToWorld(glm::vec2 screenCoords) {
    glm::vec2 result;
    result = glm::vec2(screenCoords.x, _height - screenCoords.y);
    result -= glm::vec2(_width/2.0f, _height/2.0f);
    result /= _scale;
    result += glm::vec2(_width/2.0f, _height/2.0f);
    result += _position;
    return result;
}

const glm::vec2 &Camera::getPosition() const {
    return _position;
}

void Camera::setPosition(const glm::vec2& position) {
    _position = position;
}

const glm::vec2 &Camera::getScale() const {
    return _scale;
}

void Camera::setScale(const glm::vec2& scale) {
    _scale = scale;
}

float Camera::getRotationAngle() const {
    return _rotationAngle;
}

void Camera::setRotationAngle(float rotationAngle) {
    _rotationAngle = rotationAngle;
}