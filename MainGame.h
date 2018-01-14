#ifndef PROJECT101_MAINGAME_H
#define PROJECT101_MAINGAME_H

#include <GL/gl.h>
#include <GL/glu.h>

#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>
#include <array>
#include <cmath>

#ifdef WIN32
    #include <windows.h>
    #include <GL/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"

class MainGame {
public:
    MainGame();
    ~MainGame();

    void initSystems(int argc, char* argv[]);

    static float _gravity;
    static float _deltaTime;
    static float _time;
private:

    void initGLUT(int argc, char* argv[]);

    void setInstance();

    void draw();
    void update(int value);
    void updateCamera(int width, int height);
    void logic();

    void resizeWindow(int width, int height);
    void enable2D(int width, int height);

    void keyPressed(unsigned char key, int x, int y);
    void keyUp(unsigned char key, int x, int y);
    void keySpecial(int key, int x, int y);
    void keySpecialUp(int key, int x, int y);

    static void drawCall();
    static void updateCall(int value);
    static void resizeWindowCall(int width, int height);
    static void keyPressedCall(unsigned char key, int x, int y);
    static void keyUpCall(unsigned char key, int x, int y);
    static void keySpecialCall(int key, int x, int y);
    static void keySpecialUpCall(int key, int x, int y);

    int random_range(int min, int max);

    static MainGame* _instance;

    //The window
    int _windowWidth;
    int _windowHeight;
    std::string _windowName;

    //Keys
    bool* _keyStates;
    bool* _keySpecialStates;

    bool* _keyStatesP;
    bool* _keySpecialStatesP;

    //Frame rate
    const unsigned int _interval;

    //Level
    Level _level;

    //Frame counter
    float _lastTime;
    int _frames;
    float _timeScale;

    //Game agents
    Player* _playerV2;
    std::vector<Enemy*> _enemys;

    //Camera
    Camera _camera;
};


#endif //PROJECT101_MAINGAME_H
