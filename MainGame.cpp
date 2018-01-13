#include "MainGame.h"

MainGame* MainGame::_instance = nullptr;
float MainGame::_deltaTime;
float MainGame::_gravity;

MainGame::MainGame(): _windowHeight(800),
                      _windowWidth(800),
                      _windowName("El mundo de J"),
                      _interval(1000/60),
                      _keyStates(new bool[256]),
                      _keySpecialStates(new bool[246]),
                      _keyStatesP(new bool[256]),
                      _keySpecialStatesP(new bool[246]),
                      _trauma(0.0f),
                      _traumaDuration(1.0f),
                      _lastTime(0.0f),
                      _time(0.0f){

    MainGame::_deltaTime = (float)_interval/1000.0f;
    MainGame::_gravity = 1500.0f;
}

MainGame::~MainGame() {
    delete _playerV2;
    for(Enemy* e: _enemys){
        delete e;
    }
}

void MainGame::initSystems(int argc, char* argv[]) {

    // Set random seed
    srand((unsigned int) time(nullptr));

    //Init key arrays
    std::fill_n(_keyStates, 256, false);
    std::fill_n(_keySpecialStates, 246, false);

    std::fill_n(_keyStatesP, 256, false);
    std::fill_n(_keySpecialStatesP, 246, false);

    //Load the level
    _level.loadLevel("Levels/LevelTest.txt");

    //Init the player
    _playerV2 = new Player;
    _playerV2->init(_level.getPlayerInitialPos(), &_keyStates, &_keySpecialStates, &_keyStatesP, &_keySpecialStatesP);

    //Init enemys
    for(glm::vec2 enemyPos : _level.getEnemyInitialPositions()){
        Enemy* newEnemy = new Enemy;
        newEnemy->init(enemyPos);
        _enemys.push_back(newEnemy);
    }

    initGLUT(argc, argv);

    glutMainLoop();
}

void MainGame::initGLUT(int argc, char* argv[]) {

    setInstance();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(_windowHeight, _windowWidth);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(_windowName.c_str());

    // To draw with RGBA
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(drawCall);
    glutReshapeFunc(resizeWindowCall);
    glutKeyboardFunc(keyPressedCall);
    glutKeyboardUpFunc(keyUpCall);

    glutSpecialFunc(keySpecialCall);
    glutSpecialUpFunc(keySpecialUpCall);

    glutTimerFunc(_interval, updateCall, 0);

    enable2D(_windowWidth, _windowHeight);
}

void MainGame::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(.2f, .2f, .2f, 1.0f);
    glLoadIdentity();

    _level.drawLevel();
    _playerV2->draw();


    for(Enemy* enemy : _enemys){
        enemy->draw();
    }

    glutSwapBuffers();
}

void MainGame::update(int value) {

    //Frame counter
    float cosaTime = (float)glutGet(GLUT_ELAPSED_TIME) - _lastTime;

    MainGame::_deltaTime = cosaTime/1000.0f;
    _frames = (int)floorf(1.0f/(cosaTime/1000.0f));
    _time += MainGame::_deltaTime;

    //Draw FPS in window title
    std::ostringstream string;
    string << _frames;
    glutSetWindowTitle(("FPS = " + string.str()).c_str());

    _lastTime = (float)glutGet(GLUT_ELAPSED_TIME);

    //Input
    if(_keyStates['j'] || _keyStates['J']){
        _trauma = 1.0f;
    }

    // Update the camera
    updateCamera(_windowWidth, _windowHeight);
    _trauma = std::fmaxf(_trauma - _deltaTime, 0.0f);

    // Update player and enemy positions
    logic();

    // Draw the game
    glutTimerFunc(_interval, updateCall, 0);
    glutPostRedisplay();

    // Update previous pressed keys
    std::copy(_keyStates, _keyStates + 256, _keyStatesP);
    std::copy(_keySpecialStates, _keySpecialStates + 246, _keySpecialStatesP);
}

void MainGame::updateCamera(int width, int height){

    //Set viewport
    glViewport(0, 0, width, height);

    //Load the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calculate new camera position
    float newX = _playerV2->getPosition().x - (_windowWidth/2);
    float newY = _playerV2->getPosition().y - (_windowHeight/2) + 150;

    // Set rotation trauma
    float trauma3 = _trauma * _trauma * _trauma;
    float intensity = 10.0f;
    float random = (((float(rand())/RAND_MAX) * 2.0f) - 1.0f) * intensity * trauma3;
    glRotatef(random, 0.0f,0.0f,1.0f);

    // Set translation trauma
    intensity = 20.0f;
    float randomX = (((float(rand())/RAND_MAX) * 2.0f) - 1.0f) * intensity * trauma3;
    float randomY = (((float(rand())/RAND_MAX) * 2.0f) - 1.0f) * intensity * trauma3;
    newX += randomX;
    newY += randomY;

    // Set new camera position
    glOrtho(0.0f + newX, width + newX, 0.0f + newY, height + newY, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainGame::logic() {
    _playerV2->update(_level.getLevelData());
    for(Enemy* e : _enemys){
        e->update(_level.getLevelData(), _playerV2->getPosition());
    }
}

void MainGame::resizeWindow(int width, int height) {
    _windowWidth = width;
    _windowHeight = height;

    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
}

void MainGame::enable2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int MainGame::random_range(int min, int max) {
    return min + (rand() % (max - min + 1));
}

void MainGame::keyPressed(unsigned char key, int x, int y) {
    _keyStates[key] = true;
}

void MainGame::keyUp(unsigned char key, int x, int y) {
    _keyStates[key] = false;
}

void MainGame::keySpecial(int key, int x, int y) {
    _keySpecialStates[key] = true;
}

void MainGame::keySpecialUp(int key, int x, int y) {
    _keySpecialStates[key] = false;
}

void MainGame::setInstance() {
    _instance = this;
}

void MainGame::drawCall(){
    _instance->draw();
}
void MainGame::updateCall(int value){
    _instance->update(value);
}
void MainGame::resizeWindowCall(int width, int height){
    _instance->resizeWindow(width, height);
}
void MainGame::keyPressedCall(unsigned char key, int x, int y){
    _instance->keyPressed(key, x, y);
}
void MainGame::keyUpCall(unsigned char key, int x, int y){
    _instance->keyUp(key, x, y);
}
void MainGame::keySpecialCall(int key, int x, int y){
    _instance->keySpecial(key, x, y);
}
void MainGame::keySpecialUpCall(int key, int x, int y){
    _instance->keySpecialUp(key, x, y);
}