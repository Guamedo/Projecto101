#include <string>
#include <windows.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <array>
#include "GL/glut.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "entity.h"


using namespace std;

void draw();
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius);
int random_range(int min, int max);
void update(int value);
void enable2D(int width, int height);
void loadLevel(string level);

static const double PI = 4*atan(1);
bool *keyStates = new bool[256];
bool *keySpecialStates = new bool[246];
const unsigned int interval = 1000 / 30;
World world = World("El mundo de J", 400, 400, 0.5);
double grados = 0.0;

Entity player = Entity("player", 20, 200, 1);
std::vector<Entity> enemies;

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

        if(player.jump == 0 || player.jump == 2){
            player.body.speed[1] = 10;
            player.jump++;
        }
    }
    if(!keyStates[32]/*SPACE_UP*/){
        if(player.jump == 1){
            player.jump++;

        }
    }
    if (keyStates['j'] || keyStates['J']) {
        if (rand() % 2 == 1) {
            player.attack();
        } else {
            player.wave();
        }
    }
    if (keyStates['a'] || keyStates['A']) {
        player.body.speed[0] = -5;
    }
    if (!keyStates['a'] && !keyStates['A'] && player.body.speed[0] < 0.0) {
        player.body.speed[0] = 0.0;
    }
    if (keyStates['d'] || keyStates['D']) {
        player.body.speed[0] = 5;
    }
    if (!keyStates['d'] && !keyStates['D'] && player.body.speed[0] > 0.0) {
        player.body.speed[0] = 0.0;
    }
    if (keyStates[27]/*ESC*/) {
        exit(1);
    }
}

int main(int argc, char **argv) {

    player.name="Manolo";
    for (int i = 0; i < 15; i++) {
        enemies.push_back(Entity("enemigo", random_range(10,world.W),random_range (10,world.H), 2));
    }
    loadLevel("Levels/level0.txt");

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

    glColor3f(1.0, 0.0, 0.0);
    drawFilledCircle(player.getPosition()[0], player.getPosition()[1], 3);

    glColor3f(1.0f, 1.0f, 0.0f);
    drawFilledCircle(player.head.position[0], player.head.position[1], 1.5);


    //Dibujo la accion, si la hay
    if (player.actualAction != -1) {

        glColor3f(0.8f, 0.8f, 0.0f);
        int cosaQuePlayerHace = player.actualAction - 1;
        int frameDeLaCosa = player.actualFrame;

        for (int j = 0; j < player.actions[cosaQuePlayerHace].animacion.frames.at((unsigned int)frameDeLaCosa).numDots; j++) {
            glVertex2f(player.actions[cosaQuePlayerHace].animacion.frames.at((unsigned int)frameDeLaCosa).dots.at((unsigned int)j).x + player.body.position[0],
                       player.actions[cosaQuePlayerHace].animacion.frames.at((unsigned int)frameDeLaCosa).dots.at((unsigned int)j).y + player.body.position[1]);
        }

        if (frameDeLaCosa == player.actions[cosaQuePlayerHace].animacion.numFrames - 1) {
            player.actualAction=-1;
            player.actualFrame=-1;
        } else {
            player.actualFrame++;
        }
    }
}

void drawEnemies() {
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 15; i++) {
        glVertex2f(enemies[i].body.position[0], enemies[i].body.position[1]);
    }
    glEnd();
}

void drawPlataforms(){
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < world.getPlatforms().size(); i++){
        glVertex2f(world.getPlatforms()[i].center[0]+world.getPlatforms()[i].halfSize[0], world.getPlatforms()[i].center[1]+world.getPlatforms()[i].halfSize[1]);
        glVertex2f(world.getPlatforms()[i].center[0]-world.getPlatforms()[i].halfSize[0], world.getPlatforms()[i].center[1]+world.getPlatforms()[i].halfSize[1]);
        glVertex2f(world.getPlatforms()[i].center[0]-world.getPlatforms()[i].halfSize[0], world.getPlatforms()[i].center[1]-world.getPlatforms()[i].halfSize[1]);

        glVertex2f(world.getPlatforms()[i].center[0]+world.getPlatforms()[i].halfSize[0], world.getPlatforms()[i].center[1]+world.getPlatforms()[i].halfSize[1]);
        glVertex2f(world.getPlatforms()[i].center[0]-world.getPlatforms()[i].halfSize[0], world.getPlatforms()[i].center[1]-world.getPlatforms()[i].halfSize[1]);
        glVertex2f(world.getPlatforms()[i].center[0]+world.getPlatforms()[i].halfSize[0], world.getPlatforms()[i].center[1]-world.getPlatforms()[i].halfSize[1]);
    }

    glEnd();
}

void drawEntity() {
    drawPlayer();
    drawEnemies();
    drawPlataforms();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawEntity();
    glutSwapBuffers();
}


void logic() {
    int x = 200 + sin(grados) * 120;
    int y = 200 + cos(grados) * 120;
    double vAngular = 8.0/120.0;
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].moveToPoint(x+random_range(-100,100),y+random_range(-100,100),random_range(6,10));
        if(abs(enemies[i].getPosition()[0]-player.getPosition()[0]) < 3 && abs(enemies[i].getPosition()[1]-player.getPosition()[1]) < 3){
            cout << "Estas muerto tt\n";
        }
    }
    if(grados >= 360){
        grados == 0;
    }
    grados-=vAngular;
}

void playerUpdate(){
    player.newFrameMovePoints(world);
}

void update(int value) {
    keyOperations();
    //keySpecialOperations();
    playerUpdate();
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

int random_range(int min, int max){
    return min + (rand() % (int)(max - min + 1));
}


void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle
    GLfloat twicePi = (GLfloat)(2.0f * 4*atan(1));
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for(i = 0; i <= triangleAmount;i++) {
        glVertex2f(
                (GLfloat)(x + (radius * cos(i *  twicePi / triangleAmount))),
                (GLfloat)(y + (radius * sin(i * twicePi / triangleAmount)))
        );
    }
    glEnd();
}

void loadLevel(string level){
    std::fstream file;
    file.open(level);
    double x, a, b, c, d;
    file >> x;
    for(int i = 0; i < x; i++){
        file >> a >> b >> c >> d;
        world.getPlatforms().push_back(Box({a,b},{c,d}));
    }
    file.close();
}