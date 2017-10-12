#include <string>
#include <stdio.h>
#include <stdlib.h>
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
#include "Utiles.h"
#include "EntityPrime.h"

void draw();
void drawEntity();
void drawPlataforms();
void resize(int width, int height);
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void update(int value);
void updateCamera(int width, int height);
void enable2D(int width, int height);
void keyOperations(void);
void logic();

bool *keyStates = new bool[256];
const unsigned int interval = 1000 / 40;
World world = World("El mundo de JPrime", 1900, 500, 1);
int timeSinceStart = 0;
int timeEspecial = 0;
EntityPrime sombra = EntityPrime(Vector2(230, 200), Vector2(0,-10), 1, 3, &world, 1, 0.66, 7331);
EntityPrime player = EntityPrime(Vector2(200,200), Vector2(0,0), 1, 3, &world, 1, 0.66, 1337);
int frames = 0;

int main(int argc, char **argv) {
    world.loadLevel("Levels/level1.txt");

    srand((unsigned int) time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(world.H, world.W);
    glutInitWindowPosition(0, 100);
    glutCreateWindow(world.name.c_str());

    // To draw with RGBA
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::fill_n(keyStates, 256, false);

    glutDisplayFunc(draw);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);

    glutTimerFunc(interval, update, 0);

    enable2D(world.W, world.H);

    timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    std::cout << timeSinceStart << "\n";


    glutMainLoop();
    return 0;
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawEntity();
    glutSwapBuffers();
}

void drawEntity() {
    drawPlataforms();
    player.drawEntity();
    sombra.drawEntity();
    renderString(player.getPosition()[0]-75,player.getPosition()[1]+10, player.getTonterias());

    std::ostringstream strInfo;
    strInfo << "SPEED " << player.getVelocity()[0] << "  " << player.getVelocity()[1] << " | " <<
            "POSITION " << player.getPosition()[0] << "  " << player.getPosition()[1] << " | " <<
            "PISANDO " << player.getPisando() << " | " << "FPS " << player.getTonterias();
    //renderString(player.getPosition()[0]-400,player.getPosition()[1]+250, strInfo.str());
}

void drawPlataforms() {
    glBegin(GL_TRIANGLES);
    glColor3f(0.42f, 0.6f, 0.5f);
    for (unsigned int i = 0; i < world.getPlatforms()->size(); i++) {
        glVertex2f(world.getPlatforms()->at(i).center[0] + world.getPlatforms()->at(i).halfSize[0],
                   world.getPlatforms()->at(i).center[1] + world.getPlatforms()->at(i).halfSize[1]);
        glVertex2f(world.getPlatforms()->at(i).center[0] - world.getPlatforms()->at(i).halfSize[0],
                   world.getPlatforms()->at(i).center[1] + world.getPlatforms()->at(i).halfSize[1]);
        glVertex2f(world.getPlatforms()->at(i).center[0] - world.getPlatforms()->at(i).halfSize[0],
                   world.getPlatforms()->at(i).center[1] - world.getPlatforms()->at(i).halfSize[1]);

        glVertex2f(world.getPlatforms()->at(i).center[0] + world.getPlatforms()->at(i).halfSize[0],
                   world.getPlatforms()->at(i).center[1] + world.getPlatforms()->at(i).halfSize[1]);
        glVertex2f(world.getPlatforms()->at(i).center[0] - world.getPlatforms()->at(i).halfSize[0],
                   world.getPlatforms()->at(i).center[1] - world.getPlatforms()->at(i).halfSize[1]);
        glVertex2f(world.getPlatforms()->at(i).center[0] + world.getPlatforms()->at(i).halfSize[0],
                   world.getPlatforms()->at(i).center[1] - world.getPlatforms()->at(i).halfSize[1]);
    }
    glEnd();
}

void update(int value) {
    keyOperations();
    updateCamera(world.W,world.H);
    logic();
    glutTimerFunc(interval, update, 0);
    glutPostRedisplay();
    timeEspecial = glutGet(GLUT_ELAPSED_TIME) - timeSinceStart;
    if (timeEspecial > 1000){
        std::ostringstream strng;
        strng << frames;
        player.setTonterias(strng.str());
        frames = 0;
        timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    }
    frames++;
}

void logic() {
    sombra.IA(player);
    player.newFrameMovement();
    sombra.newFrameMovement();
}

void keyOperations(void) {
    if (keyStates[32]/*SPACE*/) {
        if (player.getJump() == 0 || player.getJump() == 2) {
            player.jumpAction();
        }
    }
    if (!keyStates[32]/*SPACE_UP*/) {
        if (player.getJump() == 1) {
            player.setJump(player.getJump()+1);
        }
    }
    if (keyStates['r'] || keyStates['R']) {
        player.reset();
        sombra.reset();
    }
    if (keyStates['a'] || keyStates['A']) {
        if (player.getSprint() == 0)
            player.setVelocity(-6, player.getVelocity()[1]);
        else
            player.setVelocity(-12, player.getVelocity()[1]);

    }
    if (keyStates['d'] || keyStates['D']) {
        if (player.getSprint() == 0)
            player.setVelocity(6, player.getVelocity()[1]);
        else
            player.setVelocity(12, player.getVelocity()[1]);

    }
    if (keyStates[27]/*ESC*/) {
        exit(1);
    }
    if (keyStates['j'] || keyStates['J']) {
        player.setSprint(1);
    }
    if (!keyStates['j'] && !keyStates['J']) {
        player.setSprint(0);
    }
}

void updateCamera(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float newX = player.getPosition().x() - (world.W/2);
    float newY = player.getPosition().y() - (world.H/2) + 150;
    glOrtho(0.0f + newX, width + newX, 0.0f + newY, height + newY, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void resize(int width, int height) {
    world.W = width;
    world.H = height;
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
}

void enable2D(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyPressed(unsigned char key, int x, int y) {
    keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}



