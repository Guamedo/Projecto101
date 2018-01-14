#include "MainGame.h"

MainGame* MainGame::_instance = nullptr;

float MainGame::_deltaTime;
float MainGame::_gravity;
float MainGame::_time;

MainGame::MainGame(): _windowHeight(800),
                      _windowWidth(600),
                      _windowName("El mundo de J"),
                      _interval(1000/60),
                      _keyStates(new bool[256]),
                      _keySpecialStates(new bool[246]),
                      _keyStatesP(new bool[256]),
                      _keySpecialStatesP(new bool[246]),
                      _lastTime(0.0f),
                      _timeScale(1.0f){

}

MainGame::~MainGame() {
    delete _playerV2;
    for(Enemy* e: _enemys){
        delete e;
    }
}

void MainGame::initSystems(int argc, char* argv[]) {

    // Init global static variables
    MainGame::_deltaTime = (float)_interval/1000.0f;
    MainGame::_gravity = 1500.0f;
    MainGame::_time = 0.0f;

    // Set random seed
    srand((unsigned int) time(nullptr));

    //Init key arrays
    std::fill_n(_keyStates, 256, false);
    std::fill_n(_keySpecialStates, 246, false);

    std::fill_n(_keyStatesP, 256, false);
    std::fill_n(_keySpecialStatesP, 246, false);

    //Init the camera
    _camera.init(_windowWidth, _windowHeight);

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
    glutInitWindowPosition(300, 80);
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

void MainGame::update(int value) {

    //Frame counter
    float cosaTime = (float)glutGet(GLUT_ELAPSED_TIME) - _lastTime;


    _frames = (int)floorf(1.0f/(cosaTime/1000.0f));

    MainGame::_deltaTime = (cosaTime/1000.0f) * _timeScale;
    MainGame::_time += MainGame::_deltaTime;

    //Draw FPS in window title
    std::ostringstream string;
    string << _frames;
    glutSetWindowTitle(("FPS = " + string.str()).c_str());

    _lastTime = (float)glutGet(GLUT_ELAPSED_TIME);

    //Input
    if((_keyStates['j'] || _keyStates['J']) && !_keyStatesP['j'] && !_keyStatesP['J']){
        _camera.trauma();
    }
    if((_keyStates['t'] || _keyStates['T']) && !_keyStatesP['t'] && !_keyStatesP['T']){
        if(_timeScale < 1.0f){
            _timeScale = 1.0f;
        }else{
            _timeScale = 0.2f;
        }
    }

    // Update the camera
    _camera.goToPosition(_playerV2->getPosition() - glm::vec2(_windowWidth/2.0f, _windowHeight/2.0f - 150.0f/_camera.getScale().y));
    _camera.update();

    // Update player and enemy positions
    logic();

    // Draw the game
    glutTimerFunc(_interval, updateCall, 0);
    glutPostRedisplay();

    // Update previous pressed keys
    std::copy(_keyStates, _keyStates + 256, _keyStatesP);
    std::copy(_keySpecialStates, _keySpecialStates + 246, _keySpecialStatesP);
}

void MainGame::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(.2f, .2f, .2f, 1.0f);
    glLoadIdentity();

    _playerV2->draw();
    _level.drawLevel();

    for(Enemy* enemy : _enemys){
        enemy->draw();
    }

    glutSwapBuffers();
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
    _camera.resize(width, height);
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