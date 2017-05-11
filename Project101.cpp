#include <string>
#include <windows.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#include "World.h"
#include "Dot.h"
#include "Frame.h"
#include "Animacion.h"
#include "Action.h"

using namespace std;
#pragma comment(lib, "OpenGL32.lib")

void draw();

void update(int value);

void enable2D(int width, int height);

bool *keyStates = new bool[256];
bool *keySpecialStates = new bool[246];
const unsigned int interval = 1000 / 10;
static World world = World();

class Entity {
public:
    string nombre;
    int posx, posy, tipo;
    vector<Action> actions;
    int accion = -1; //-1 libre. 1 ataque. 2 wave.
    int frameActual = -1;
    bool interrumpible;

    void attack() {
        if (accion == -1) {
            cout << "ataco\n";
            accion = 1;
            frameActual = 0;
        }
    }

    void wave() {
        if (accion == -1) {
            cout << "wave-o\n";
            accion = 2;
            frameActual = 0;
        }
    }

    void setposx(int x) {
        if (world.inrangex(x)) {
            posx = x;
        }
    }

    void setposy(int y) {
        if (world.inrangey(y)) {
            posy = y;
        }
    }

    void moveR() {
        int random = (rand() % 4) + 1;
        switch (random) {
            case 1:
                setposx(posx + 1);
                break;
            case 2:
                setposx(posx - 1);
                break;
            case 3:
                setposy(posy + 1);
                break;
            case 4:
                setposy(posy - 1);
                break;
            default:
                break;
        }
    }

    Entity() {

    }

    Entity(string nombreEntidad, int x, int y, int tipoo) {
        nombre = nombreEntidad;
        posx = x;
        posy = y;
        tipo = tipoo;
        actions.push_back(Action("actionAttack", (char*)"Animaciones/attack.anim"));
        actions.push_back(Action("actionWave", (char*)"Animaciones/wave.anim"));
    }
};

class Player : public Entity {
public:
    float speed[2] = {0.0, 0.0};// (X,Y)
    Player(string nombreEntidad, int x, int y, int tipoo) : Entity(nombreEntidad, x, y, tipoo) {}
};

class Enemy : public Entity {
public:
    Enemy(string nombreEntidad, int x, int y, int tipoo) {
        nombre = nombreEntidad;
        posx = x;
        posy = y;
        tipo = tipoo;
    }

    Enemy() {
    }
};

Player player = Player("player", 25, 25, 1);
std::vector<Enemy> enemies;

void keyPressed(unsigned char key, int x, int y) {
    keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void keySpecial(int key, int x, int y) {
    keySpecialStates[key] = true;
}

void keySpecialUp(int key, int x, int y) {
    keySpecialStates[key] = false;
}

void keyOperations(void) {
    if (keyStates[32]/*SPACE*/) {
        cout << "El espacio se ha ido de vacaciones loquete";

        /*if (rand() % 2 == 1) {
            player.attack();
        } else {
            player.wave();
        }*/
    }
    if (keyStates['j'] || keyStates['J']) {
        cout << "La 'j' Master Race";
        if (rand() % 2 == 1) {
            player.attack();
        } else {
            player.wave();
        }
    }
    if (keyStates['w'] || keyStates['W']) {
        player.setposy((int)(player.posy + 1 + player.speed[1]));
        if (player.speed[1] < 5.0 - 0.2) {
            player.speed[1] += 0.2;
        }
    }
    if (!keyStates['w'] && !keyStates['W'] && player.speed[1] > 0.0) {
        player.speed[1] = 0.0;
    }
    if (keyStates['s'] || keyStates['S']) {
        player.setposy((int)(player.posy - 1 + player.speed[1]));
        if (player.speed[1] > -5.0 + 0.2) {
            player.speed[1] -= 0.2;
        }
    }
    if (!keyStates['s'] && !keyStates['S'] && player.speed[1] < 0.0) {
        player.speed[1] = 0.0;
    }
    if (keyStates['a'] || keyStates['A']) {
        player.setposx((int)(player.posx - 1 + player.speed[0]));
        if (player.speed[0] > -5.0 + 0.2) {
            player.speed[0] -= 0.2;
        }
    }
    if (!keyStates['a'] && !keyStates['A'] && player.speed[0] < 0.0) {
        player.speed[0] = 0.0;
    }
    if (keyStates['d'] || keyStates['D']) {
        player.setposx((int)(player.posx + 1 + player.speed[0]));
        if (player.speed[0] < 5.0 - 0.2) {
            player.speed[0] += 0.2;
        }
    }
    if (!keyStates['d'] && !keyStates['D'] && player.speed[0] > 0.0) {
        player.speed[0] = 0.0;
    }
    if (keyStates[27]/*ESC*/) {
        exit(1);
    }
    cout << player.speed[0] << "	" << player.speed[1] << "\n";
}

void keySpecialOperations(void) {
    cout << "No me gusta esta movida loco...";
    if (keySpecialStates[GLUT_KEY_UP]) {
        player.setposy(player.posy + 1);
    }
    if (keySpecialStates[GLUT_KEY_DOWN]) {
        player.setposy(player.posy - 1);
    }
    if (keySpecialStates[GLUT_KEY_LEFT]) {
        player.setposx(player.posx - 1);
    }
    if (keySpecialStates[GLUT_KEY_RIGHT]) {
        player.setposx(player.posx + 1);
    }
}

int eucDist(int x1, int y1, int x2, int y2) {
    return (int) sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int main(int argc, char **argv) {
    for (int i = 0; i < 15; i++) {
        enemies.push_back(Enemy("enemigo", (rand() % 170) + 12, (rand() % 170) + 12, 2));
    }
    srand((unsigned int)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(world.H, world.W);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(world.name.c_str());


    std::fill_n(keyStates, 256, false);
    std::fill_n(keySpecialStates, 246, false);

    glutDisplayFunc(draw);

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);

    glutSpecialFunc(keySpecial);
    glutSpecialUpFunc(keySpecialUp);

    glutTimerFunc(interval, update, 0);

    enable2D(world.W, world.H);

    glutMainLoop();
    return 0;
}

void drawPlayer() {
    //Dibujo el jugador
    glColor3f(1.0f, 0.0f, 0.0f);
    int x, y;
    for (int i = -3; i <= 3; i++) {
        for (int j = -3; j <= 3; j++) {
            x = player.posx - i;
            y = player.posy - j;
            if (eucDist(x, y, player.posx, player.posy) <= 3) {
                glVertex2d(x, y);
            }
        }
    }
    //glVertex2f(player.posx, player.posy);

    //Dibujo la accion, si la hay
    if (player.accion != -1) {

        glColor3f(0.8f, 0.8f, 0.0f);
        int cosaQuePlayerHace = player.accion - 1;
        int frameDeLaCosa = player.frameActual;

        for (int j = 0; j < player.actions[cosaQuePlayerHace].animacion.frames.at((unsigned int)frameDeLaCosa).numDots; j++) {
            glVertex2f(player.actions[cosaQuePlayerHace].animacion.frames.at((unsigned int)frameDeLaCosa).dots.at((unsigned int)j).x + player.posx,
                       player.actions[cosaQuePlayerHace].animacion.frames.at((unsigned int)frameDeLaCosa).dots.at((unsigned int)j).y + player.posy);
        }

        if (frameDeLaCosa == player.actions[cosaQuePlayerHace].animacion.numFrames - 1) {
            player.accion = -1;
            player.frameActual = -1;
        } else {
            player.frameActual++;
        }
    }
}

void drawEnemies() {
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 15; i++) {
        glVertex2f(enemies[i].posx, enemies[i].posy);
    }
}

void drawEntity() {
    drawPlayer();
    drawEnemies();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBegin(GL_POINTS);

    drawEntity();

    glEnd();
    glutSwapBuffers();
}

void logic() {
    for (int i = 0; i < 15; i++) {
        enemies[i].moveR();
    }
}

void update(int value) {
    keyOperations();
    keySpecialOperations();
    logic();
    glutTimerFunc(interval, update, 0);
    glutPostRedisplay();
}

void enable2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void resize(int width, int height){
    glViewport(0,0, width, height);
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
}