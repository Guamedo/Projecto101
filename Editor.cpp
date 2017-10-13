//
// Created by zztop9 on 12-Oct-17.
//

#include <iostream>
#include "Vector2.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"
#include <cmath>
#include <vector>

using namespace std;

class Lienzo {
public:
    Vector2 tam;
    Vector2 posRaton;
    Vector2 posVentana;
    Vector2 mipos;

    Lienzo(int x, int y){
        tam[0] = x;
        tam[1] = y;
        mipos[0] = 0;
        mipos[1] = 0;
    }
}lienzo(1800, 800);

class Casilla {
public:
    int estado;

    Casilla(int est){
        /*
         * 0 = free
         * 1 = solid
         */
        estado = est;
    }

    void nada(){}
};

class Mapa {
public:
    Vector2 tam;
    Vector2 posRaton;
    Vector2 noVisto;
    Vector2 Visto;
    Vector2 tamC;
    Vector2 numC;
    Vector2 sobreC;
    //Casilla **data;

    Mapa(Vector2 numCasillas, Vector2 tamaCasilla, Lienzo lienz){
        numC = numCasillas;
        tamC = tamaCasilla;
        tam[0] = numC[0] * tamC[0];
        tam[1] = numC[1] * tamC[1];
        Visto[0] = lienz.tam[0];
        Visto[1] = lienz.tam[1];
        noVisto[0] = 0;
        noVisto[1] = 0;

        /*for(int i = 0; i<numC[0]; i++){
            for(int j = 0; j<numC[1]; j++){
                data[i][j] = Casilla(0);
            }
        }*/
    }

    void draw(){

        //marca el (0,0)
        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0,0);
        glVertex2f(1,0);
        glVertex2f(-1,0);
        glVertex2f(0,1);
        glVertex2f(0,-1);
        glVertex2f(0,2);
        glVertex2f(0,-2);
        glVertex2f(2,0);
        glVertex2f(-2,0);
        glEnd();

        //Cuadricula
        for (int i = floor(noVisto[0]/tamC[0]); i < numC[0]; i++) {
            for (int j = floor(noVisto[1]/tamC[1]); j < numC[1]; j++){
                glBegin(GL_LINES);
                glColor4f(1.0f, 1.0f, 1.0f, 0.25f);

                //topline
                glVertex2f(i*tamC[0],
                           (-1)*(j*tamC[1]) + lienzo.tam[1]-1);

                glVertex2f(i*tamC[0] + tamC[0],
                           (-1)*(j*tamC[1]) + lienzo.tam[1]-1);

                //rightline
                glVertex2f(i*tamC[0] + tamC[0],
                           (-1)*(j*tamC[1]) + lienzo.tam[1]-1);
                glVertex2f(i*tamC[0] + tamC[0],
                           (-1)*(j*tamC[1] + tamC[1]) + lienzo.tam[1]-1);

                //bottomLines
                glVertex2f(i*tamC[0] + tamC[0],
                           (-1)*(j*tamC[1] + tamC[1]) + lienzo.tam[1]-1);
                glVertex2f(i*tamC[0],
                           (-1)*(j*tamC[1] + tamC[1]) + lienzo.tam[1]-1);

                //leftline
                glVertex2f(i*tamC[0],
                           (-1)*(j*tamC[1] + tamC[1]) + lienzo.tam[1]-1);
                glVertex2f(i*tamC[0],
                           (-1)*(j*tamC[1]) + lienzo.tam[1]-1);

                glEnd();
            }
        }

        //Contorno del mapa
        glBegin(GL_LINES);
        glColor3f(0,0,1);
        glVertex2f(0, lienzo.tam[1]-1);
        glVertex2f(0, (-1)*(tamC[1]*numC[1])+lienzo.tam[1]-1);

        glVertex2f(0, (-1)*(tamC[1]*numC[1])+lienzo.tam[1]-1);
        glVertex2f(tamC[0]*numC[0], (-1)*(tamC[1]*numC[1])+lienzo.tam[1]-1);

        glVertex2f(tamC[0]*numC[0], (-1)*(tamC[1]*numC[1])+lienzo.tam[1]-1);
        glVertex2f(tamC[0]*numC[0], lienzo.tam[1]-1);

        glVertex2f(tamC[0]*numC[0], lienzo.tam[1]-1);
        glVertex2f(0, lienzo.tam[1]-1);
        glEnd();

        //Resaltar sobreC
        int i = sobreC[0];
        int j = sobreC[1];
        glBegin(GL_TRIANGLES);
        glColor3f(1,1,0);
        //triangulo superior izquierdo
        glVertex2f(i*tamC[0],
                   (-1)*(j*tamC[1]) + lienzo.tam[1]-1);

        glVertex2f(i*tamC[0] + tamC[0],
                   (-1)*(j*tamC[1]) + lienzo.tam[1]-1);

        glVertex2f(i*tamC[0],
                   (-1)*(j*tamC[1] + tamC[1]) + lienzo.tam[1]-1);

        //triangulo inferior derecho
        glVertex2f(i*tamC[0] + tamC[0],
                   (-1)*(j*tamC[1]) + lienzo.tam[1]-1);
        glVertex2f(i*tamC[0] + tamC[0],
                   (-1)*(j*tamC[1] + tamC[1]) + lienzo.tam[1]-1);
        glVertex2f(i*tamC[0],
                   (-1)*(j*tamC[1] + tamC[1]) + lienzo.tam[1]-1);
        glEnd();
    }

    Vector2 CasillaSeleccionada(){
        return Vector2(floor(posRaton[0]/tamC[0]),floor(posRaton[1]/tamC[1]));
    }
}mapa(Vector2(423, 28), Vector2(8,8), lienzo); //mario 423 * 28 (8*8)

void draw();
void resize(int width, int height);
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void update(int value);
void updateCamera(int width, int height);
void enable2D(int width, int height);
void keyOperations(void);
void mouse(int x, int y);
void mouseClicking(int x, int y);

bool *keyStates = new bool[256];
const unsigned int interval = 1000 / 60;

int main (int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(lienzo.tam[0], lienzo.tam[1]);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Editor");

    // To draw with RGBA
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::fill_n(keyStates, 256, false);

    glutMotionFunc(mouseClicking);
    glutPassiveMotionFunc(mouse);

    glutDisplayFunc(draw);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutTimerFunc(interval, update, 0);
    enable2D(lienzo.tam[0], lienzo.tam[1]);
    glutMainLoop();

    return 0;
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    mapa.draw();

    glutSwapBuffers();
}

void update(int value) {
    keyOperations();
    updateCamera(lienzo.tam[0], lienzo.tam[1]);
    glutTimerFunc(interval, update, 0);
    glutPostRedisplay();
}

void mouse(int x, int y){
    //cout << x << " " << y << "\n";
    mapa.posRaton[0] = x + mapa.noVisto[0];
    mapa.posRaton[1] = y + mapa.noVisto[1];
    Vector2 cs = mapa.CasillaSeleccionada();
    cout << cs[0] << " " << cs[1] << "\n";
    mapa.sobreC = cs;
}

void mouseClicking(int x, int y){
    cout << x << " " << y << " click\n";
}

void keyOperations(void) {
    if (keyStates[32]/*SPACE*/) {

    }
    if (!keyStates[32]/*SPACE_UP*/) {

    }
    if (keyStates['r'] || keyStates['R']) {

    }
    if (keyStates['a'] || keyStates['A']) {
        //lienzo.mipos[0] -= mapa.tamC[0];
        if (mapa.noVisto[0] >= mapa.tamC[0]){
            mapa.noVisto[0] -= mapa.tamC[0];
            cout << mapa.noVisto[0] << "\n";
        }

    }
    if (keyStates['d'] || keyStates['D']) {
        //lienzo.mipos[0] += mapa.tamC[0];
        if (mapa.noVisto[0] <= mapa.tamC[0]*mapa.numC[0]) {
            mapa.noVisto[0] += mapa.tamC[0];
            cout << mapa.noVisto[0] << "\n";
        }

    }
    if (keyStates['w'] || keyStates['W']) {
        //lienzo.mipos[1] += mapa.tamC[1];
        if (mapa.noVisto[1] >= mapa.tamC[1]){
            mapa.noVisto[1] -= mapa.tamC[1];
            cout << mapa.noVisto[1] << "\n";
        }

    }
    if (keyStates['s'] || keyStates['S']) {
        //lienzo.mipos[1] -= mapa.tamC[1];
        if (mapa.noVisto[1] <= mapa.tamC[1]*mapa.numC[1]) {
            mapa.noVisto[1] += mapa.tamC[1];
            cout << mapa.noVisto[1] << "\n";
        }

    }
    if (keyStates[27]/*ESC*/) {
        exit(1);
    }
}

void resize(int width, int height) {
    lienzo.tam[0] = width;
    lienzo.tam[1] = height;
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
}

void updateCamera(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //float newX = lienzo.mipos[0];// - (world.W/2);
    //float newY = lienzo.mipos[1];// - (world.H/2) + 150;mapa.noVisto[1]
    float newX = mapa.noVisto[0];
    float newY = (-1)*mapa.noVisto[1];
    glOrtho(0.0f + newX, width + newX, 0.0f + newY, height + newY, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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