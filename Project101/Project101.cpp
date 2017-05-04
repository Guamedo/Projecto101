#include "stdafx.h"
#include <string>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "GL/freeglut.h"

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
void keyboard();

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
		}
	}
	Entity() {

	}
	Entity(string nombreEntidad, int x, int y, int tipoo) {
		nombre = nombreEntidad;
		posx = x;
		posy = y;
		tipo = tipoo;
		actions.push_back(Action("actionAttack", "attack.anim"));
		actions.push_back(Action("actionWave", "wave.anim"));
	}
};

class Player : public Entity {
public:
	using Entity::Entity;
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

int main(int argc, char** argv)
{
	for (int i = 0; i < 15; i++) {
		enemies.push_back(Enemy("enemigo", (rand() % 170) + 12, (rand() % 170) + 12, 2));
	}

	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(world.H, world.W);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(world.name);

	glutDisplayFunc(draw);
	glutTimerFunc(interval, update, 0);

	enable2D(world.W, world.H);

	glutMainLoop();
	return 0;
}

void drawPlayer() {
	//Dibujo el jugador
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(player.posx, player.posy);

	//Dibujo la accion, si la hay
	if (player.accion != -1) {

		glColor3f(0.8f, 0.8f, 0.0f);
		int cosaQuePlayerHace = player.accion - 1;
		int frameDeLaCosa = player.frameActual;

		for (int j = 0; j < player.actions[cosaQuePlayerHace].animacion.frames.at(frameDeLaCosa).numDots; j++) {
			glVertex2f(player.actions[cosaQuePlayerHace].animacion.frames.at(frameDeLaCosa).dots.at(j).x + player.posx, player.actions[cosaQuePlayerHace].animacion.frames.at(frameDeLaCosa).dots.at(j).y + player.posy);
		}

		if (frameDeLaCosa == player.actions[cosaQuePlayerHace].animacion.numFrames - 1) {
			player.accion = -1;
			player.frameActual = -1;
		}
		else {
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

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glBegin(GL_POINTS);

	drawEntity();

	glEnd();
	glutSwapBuffers();
}

void logic() {
	//player.moveR();
	for (int i = 0; i < 15; i++) {
		enemies[i].moveR();
	}
}

void update(int value)
{
	keyboard();
	logic();
	glutTimerFunc(interval, update, 0);
	glutPostRedisplay();
}

void enable2D(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawText(float x, float y, std::string text)
{
	glRasterPos2d(x, y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44
#define VK_J 0x4A
void keyboard()
{
	if (GetAsyncKeyState(VK_J)) {
		if (rand() % 2 == 1) {
			player.attack();
		}
		else {
			player.wave();
		}
	}
	if (GetAsyncKeyState(VK_W)) {
		player.setposy(player.posy + 1);
	}
	if (GetAsyncKeyState(VK_S)) {
		player.setposy(player.posy - 1);
	}
	if (GetAsyncKeyState(VK_A)) {
		player.setposx(player.posx - 1);
	}
	if (GetAsyncKeyState(VK_D)) {
		player.setposx(player.posx + 1);
	}
}

