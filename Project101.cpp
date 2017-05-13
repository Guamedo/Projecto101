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

#include "World.h"
#include "Dot.h"
#include "Frame.h"
#include "Animacion.h"
#include "Action.h"
#include "Box.h"

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
static World world = World("El mundo de J", 400, 400, 2);
static double grados = 0.0;
static vector<Box> plataformas;

class Entity {
public:
    string nombre;
    int posx, posy, tipo;
    vector<Action> actions;
    int accion = -1; //-1 libre. 1 ataque. 2 wave.
    int frameActual = -1;
    bool interrumpible;

    int distance(int point1x, int point1y, int point2x, int point2y) {
        int rectHeight = point1y - point2y;
        int rectWidth = point1x - point2x;
        return pow(pow(rectHeight, 2) + pow(rectWidth, 2), 0.5);
    }

    array<int,2> moveToPoint(int point1x, int point1y, int point2x, int point2y){
        int dirVecX, dirVecY;
        point2x = point2x - point1x;
        point2y = point2y - point1y;
        int distancia = distance(0, 0, point2x, point2y);
        if (distancia>0) {
            dirVecX = point2x / distancia;
            dirVecY = point2y / distancia;
        }else{
            dirVecX = 0;
            dirVecY = 1;
        }
        array<int, 2> cosa;
        cosa[0] = dirVecX;
        cosa[1] = dirVecY;
        return cosa;
    }

    void moveToPoint(int x, int y,int speed){
        int vecX = x - this->posx;
        int vecY = y - this->posy;
        double mod = sqrt(pow(vecX,2)+pow(vecY,2));
        vecX = (vecX*speed)/mod;
        vecY = (vecY*speed)/mod;
        this->setposx(this->posx+vecX);
        this->setposy(this->posy+vecY);
    }

    /*codigo de movimiento guay*/
    /*
    Technobabble #2
    I won't go into how to make a box collide with rectangles, beacuse I know that you can find a lot of stuff like that all over, and my solutions for this are a bit horribad. I will also assume that you know what a vector is, and that you know how to implement gravity and stuff like that. What I'm going to show you is how to create "atoms" or whatever you want to call them, that is points that are connected to each other to create a simple physics simulation.

    So, assume that you have two points, Ax, Ay and Bx, By. These points also have velocity vectors, sAx, sAy and sBx, sBy. By adding the vectors to the positions you get movement, as you probably know.

    You now need to create two functions, one to determine the distance between two points(I'm going to call mine diag, from diagonal) and one to create direction vector pointing towards a point. That is a vector with a total length of 1.0, only indicating direction. This can also be used for a lot of fun stuff, such as gravity wells and so on. I'm gonna call mine moveToPoint. These functions are heavy on the processor, so use them sparsely.

    So this is what we have so far:

            function diag(point1x, point1y, point2x, point2y)
    rectHeight = ABSOLUTE(point1y - point2y)
    rectWidth = ABSOLUTE(point1x - point2x)
    diagonal = SQUAREROOT((rectHeight * rectHeight) + (rectWidth * rectWidth))
    return diagonal
            function end

            function moveToPoint(point1x, point1y, point2x, point2y)
    point2x = point2x - point1x
    point2y = point2y - point1y
    diag = diag(0,0, point2x, point2y)
    if diag>0 then
            dirVecX = point2x/diag
    dirVecY = point2y/diag
    else
    dirVecX = 0
    dirVecY = 1
    end if
    return [dirVecX, dirVecY]
    function end

    Alright, let's move on. This part is simple, every frame do this:

    function newFrameMovePoints()
    Ax = Ax + sAx
    Ay = Ay + sAy
    Bx = Bx + sBx
    By = By + sBy

    sAx = sAx*0.98
    sAy = (sAy*0.98) + 1.2
    sBx = sBx*0.98
    sBy = (sBy*0.98) + 1.2

    diag = diag(Ax, Ay, Bx, By)
    rtrn = moveToPoint(Ax, Ay, Bx, By)
    dirX = rtrn[1]
    dirY = rtrn[2]
    getToDiag = 17

    Ax = Ax - (getToDiag-diag)*dirX*0.5
    sAx = sAx - (getToDiag-diag)*dirX*0.5
    Ay = Ay - (getToDiag-diag)*dirY*0.5
    sAy = sAy - (getToDiag-diag)*dirY*0.5
    Bx = Bx + (getToDiag-diag)*dirX*0.5
    sBx = sBx + (getToDiag-diag)*dirX*0.5
    By = By + (getToDiag-diag)*dirY*0.5
    sBy = sBy + (getToDiag-diag)*dirY*0.5
    function end

    What you see is, separated by the empty rows: Adding the velocities to the positions, applying air friction and gravity to the velocities, retrieving the data necessary for the "binding" of the two points, and finally applying the bond. The points are now freely moving, but will always keep the same distance to each other, and forces applied to one point will realistically transfer into the other. As long as you draw the points after this has been done, they will always be displayed with the correct distance between them.

    You see the "0.5"s after each row in the last section? Those mean that if there is a difference between the desired distance and the actual distance between the points, this point will stand for  50% of the movement made to correct the distance. If you tilt those numbers, so that for example point A stands for 85% of the movement and point B for 15% it will appear that B is significally heavier than A. If you make it so that the sum is less than 100% you will get an elastic bond, like a rubber band. You might want to tone down the movement of the actual positions in this case, and focus on the velocities. Other interesting choices you can toy with is to only affect the points if they are further away from each other than the desired distance, or only if they are closer.
    */

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
    int jump = 0;
    Box BBox;
    Player(string nombreEntidad, int x, int y, int tipoo) : Entity(nombreEntidad, x, y, tipoo) {
        BBox = Box({x,y}, {3,3});
    }
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

Player player = Player("player", 20, 200, 1);
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
        if(player.jump == 0 || player.jump == 2){
            player.speed[1] = 10;
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
        player.speed[0] = -5;
    }
    if (!keyStates['a'] && !keyStates['A'] && player.speed[0] < 0.0) {
        player.speed[0] = 0.0;
    }
    if (keyStates['d'] || keyStates['D']) {
        player.speed[0] = 5;
    }
    if (!keyStates['d'] && !keyStates['D'] && player.speed[0] > 0.0) {
        player.speed[0] = 0.0;
    }
    if (keyStates[27]/*ESC*/) {
        exit(1);
    }
}

void keySpecialOperations(void) {
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
    player.nombre = "Manolo";
    for (int i = 0; i < 15; i++) {
        enemies.push_back(Enemy("enemigo", random_range(10,world.W),random_range (10,world.H), 2));
    }
    loadLevel("Levels/level0.txt");
/*    plataformas.push_back(Box({50,25},{50,25}));
    plataformas.push_back(Box({133,50},{33,50}));
    plataformas.push_back(Box({266,75},{34,75}));
    plataformas.push_back(Box({350,100},{50,100}));
    plataformas.push_back(Box({350,300},{50,50}));*/
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
/*    glColor3f(1.0f, 0.0f, 0.0f);
    int x, y;
    for (int i = -3; i <= 3; i++) {
        for (int j = -3; j <= 3; j++) {
            x = player.posx - i;
            y = player.posy - j;
            if (eucDist(x, y, player.posx, player.posy) <= 3) {
                glVertex2d(x, y);
            }
        }
    }*/
    glColor3f(1.0, 0.0, 0.0);
    drawFilledCircle(player.posx, player.posy, 3);
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
    glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < 15; i++) {
        glVertex2f(enemies[i].posx, enemies[i].posy);
    }
    glEnd();
}

void drawPlataforms(){
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < plataformas.size(); i++){
        glVertex2f(plataformas[i].center[0]+plataformas[i].halfSize[0], plataformas[i].center[1]+plataformas[i].halfSize[1]);
        glVertex2f(plataformas[i].center[0]-plataformas[i].halfSize[0], plataformas[i].center[1]+plataformas[i].halfSize[1]);
        glVertex2f(plataformas[i].center[0]-plataformas[i].halfSize[0], plataformas[i].center[1]-plataformas[i].halfSize[1]);

        glVertex2f(plataformas[i].center[0]+plataformas[i].halfSize[0], plataformas[i].center[1]+plataformas[i].halfSize[1]);
        glVertex2f(plataformas[i].center[0]-plataformas[i].halfSize[0], plataformas[i].center[1]-plataformas[i].halfSize[1]);
        glVertex2f(plataformas[i].center[0]+plataformas[i].halfSize[0], plataformas[i].center[1]-plataformas[i].halfSize[1]);
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
        if(abs(enemies[i].posx-player.posx) < 3 && abs(enemies[i].posy-player.posy) < 3){
            cout << "Estas muerto tt\n";
        }
    }
    if(grados >= 360){
        grados == 0;
    }
    grados-=vAngular;
}

void playerUpdate(){
    int newX, newY;
    bool colX = false;
    int colY = -1;
    newX = player.posx + player.speed[0];
    newY = player.posy + player.speed[1];
    for(int i = 0; i < plataformas.size(); i++){
        if(plataformas[i].Overlaps(Box({newX,player.posy},{3,3}))){
            colX = true;
        }
        if(plataformas[i].Overlaps(Box({player.posx,newY},{3,3}))){
            colY = i;
/*            player.posy = newY;
            if(player.posy <= 20){
                player.speed[1] = 0;
                player.jump = 0;
                player.posy = 20;
            }else{
                player.speed[1]-=world.gravity;
            }*/
        }/*else{
            player.posy = plataformas[i].center[1]+plataformas[i].halfSize[1]+4;
            player.jump = 0;
            player.speed[1] = 0;
        }*/
    }
    if(!colX){
        player.setposx(newX);
    }
    if(colY == -1){
        player.posy = newY;
        player.speed[1]-=world.gravity;
    }else{
        if(player.posy > plataformas[colY].center[1]){
            player.posy = plataformas[colY].center[1]+plataformas[colY].halfSize[1]+4;
            player.jump = 0;
            player.speed[1] = 0;
        }else{
            player.posy = plataformas[colY].center[1]-plataformas[colY].halfSize[1]-4;
            player.speed[1] = 0;
        }
    }
    // Esto esta muy mal hecho pero por ahora se queda
    if(player.posy < -100){
        cout << "Estas muerto tt\n";
        player.posx = 20;
        player.posy = 60;
    }
    cout << "X = " << player.posx << "    Y = " << player.posy << "\n";
}

void update(int value) {
    keyOperations();
    keySpecialOperations();
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
    int x, a, b, c, d;
    file >> x;
    for(int i = 0; i < x; i++){
        file >> a >> b >> c >> d;
        plataformas.push_back(Box({a,b},{c,d}));
    }
    file.close();
}