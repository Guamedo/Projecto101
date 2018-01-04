#include "MainGame.h"

MainGame* MainGame::_instance = nullptr;

MainGame::MainGame(): _windowHeight(800),
                      _windowWidth(800),
                      _windowName("El mundo de J"),
                      _interval(1000/60),
                      _timeSinceStart(0),
                      _timeEspecial(0),
                      _keyStates(new bool[256]),
                      _keySpecialStates(new bool[246]){

}

MainGame::~MainGame() {

}

void MainGame::initSystems(int argc, char* argv[]) {

    srand((unsigned int) time(nullptr));

    //Init key arrays
    for (int i = 0; i < 256; i++){
        _keyPresionadoRecientemente[i]=0;
    }
    std::fill_n(_keyStates, 256, false);
    std::fill_n(_keySpecialStates, 246, false);

    //Load the level
    _level.loadLevel("Levels/LevelTest.txt");

    //Init the player
    _playerV2 = new Player;
    _playerV2->init(_level.getPlayerInitialPos(), &_keyStates, &_keySpecialStates);

    //Init enemys
    for(glm::vec2 enemyPos : _level.getEnemyInitialPositions()){
        Enemy* newEnemy = new Enemy;
        newEnemy->init(enemyPos);
        _enemys.push_back(newEnemy);
    }

    /*************
     * Init GLUT *
     *************/

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

    // Get starting time
    _timeSinceStart = glutGet(GLUT_ELAPSED_TIME);

    glutMainLoop();
}

void MainGame::draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    _level.drawLevel();
    _playerV2->draw();

    for(Enemy* enemy : _enemys){
        enemy->draw();
    }

    glutSwapBuffers();
}

void MainGame::update(int value) {
    updateCamera(_windowWidth, _windowHeight);
    logic();
    glutTimerFunc(_interval, updateCall, 0);
    glutPostRedisplay();
    _timeEspecial = glutGet(GLUT_ELAPSED_TIME) - _timeSinceStart;
    if (_timeEspecial > 1000){
        std::ostringstream strng;
        strng << _frames;
        //player.setTonterias(strng.str());
        glutSetWindowTitle(("FPS = " + strng.str()).c_str());
        _frames = 0;
        _timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    }
    _frames++;
}

void MainGame::updateCamera(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float newX = _playerV2->getPosition().x - (_windowWidth/2);
    float newY = _playerV2->getPosition().y - (_windowHeight/2) + 150;
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