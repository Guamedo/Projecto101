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
#include "Cacho.h"
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
const unsigned int interval = 1000 / 10;
static World world = World("El mundo de J", 400, 400, 2);
static double grados = 0.0;
static vector<Box> plataformas;




class Entity {
public:
    string nombre;
    vector<Action> actions;
    int accion = -1; //-1 libre. 1 ataque. 2 wave.
    int frameActual = -1;
    int tipo;

    Cacho head;
    Cacho body;

    double distance(double point1x, double point1y, double point2x, double point2y) {
        double rectHeight = point1y - point2y;
        double rectWidth = point1x - point2x;
        return pow(pow(rectHeight, 2) + pow(rectWidth, 2), 0.5);
    }

    array<double,2> moveToPoint(double point1x, double point1y, double point2x, double point2y){
        double dirVecX, dirVecY;
        point2x = point2x - point1x;
        point2y = point2y - point1y;
        double distancia = distance(0, 0, point2x, point2y);
        if (distancia>0) {
            dirVecX = point2x / distancia;
            dirVecY = point2y / distancia;
        }else{
            dirVecX = 0;
            dirVecY = 1;
        }
        array<double, 2> cosa;
        cosa[0] = dirVecX;
        cosa[1] = dirVecY;
        return cosa;
    }

    void newFrameMovePoints() {
        cout << "antes de nada... \n";
        setPosition(getPosition()[0]+body.speed[0],getPosition()[1]+body.speed[1]);
        head.position[0] = head.position[0] + head.speed[0];
        head.position[1] = head.position[1] + head.speed[1];

        double airFriction = 0.98;
        double grav = 0.5;
        body.speed[0] = body.speed[0] * airFriction;
        body.speed[1] = (body.speed[1] * airFriction)-grav;
        head.speed[0] = head.speed[0] * airFriction;
        head.speed[1] = (head.speed[1] * airFriction);

        double dist = distance(head.position[0], head.position[1], body.position[0], body.position[1]+6);
        array<double,2> rtrn = moveToPoint(head.position[0], head.position[1], body.position[0], body.position[1]);
        cout << rtrn[0] << " " << rtrn[1] << "\n";
        double getToDiag = 2;

        double valor = 0.5;
        setPosition(body.position[0] + (getToDiag - dist) * rtrn[0] * valor, body.position[1] + (getToDiag - dist) * rtrn[1] * valor);
        body.speed[0] = body.speed[0] + (getToDiag - dist) * rtrn[0] * valor;
        body.speed[1] = body.speed[1] + (getToDiag - dist) * rtrn[1] * valor;
        head.position[0] = head.position[0] - (getToDiag - dist) * rtrn[0] * valor;
        head.speed[0] = head.speed[0] - (getToDiag - dist) * rtrn[0] * valor;
        head.position[1] = head.position[1] - (getToDiag - dist) * rtrn[1] * valor;
        head.speed[1] = head.speed[1] - (getToDiag - dist) * rtrn[1] * valor;

    }

    /*void moveToPoint(double x, double y,double speed){
        double vecX = x - this->body.position[0];
        double vecY = y - this->body.position[1];
        double mod = sqrt(pow(vecX,2)+pow(vecY,2));
        vecX = (vecX*speed)/mod;
        vecY = (vecY*speed)/mod;
        this->setposx(this->body.position[0]+vecX);
        this->setposy(this->body.position[1]+vecY);
    }*/

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

    /*void setposx(double x) {
        if (world.inrangex(x)) {
            body.position[0] = x;
        }
    }

    void setposy(double y) {
        if (world.inrangey(y)) {
            body.position[1] = y;
        }
    }*/

    void setPosition(double x, double y){
        bool b1 = true, b2 = true;
        if (!world.inrangey(y)) {
            b1 = false;
        }
        if (!world.inrangex(x)) {
            b2 = false;
        }
        if (b1 and b2){
            body.position[0] = x;
            body.position[1] = y;
        }else{
            cout << "1 o las 2 coordenadas son incorrectas\n";
        }

    }

    array<double, 2> getPosition(){
        return body.position;
    }

    /*void moveR() {
        int random = (rand() % 4) + 1;
        switch (random) {
            case 1:
                setposx(getPosition()[0] + 1);
                break;
            case 2:
                setposx(getPosition()[0] - 1);
                break;
            case 3:
                setposy(getPosition()[1] + 1);
                break;
            case 4:
                setposy(getPosition()[1] - 1);
                break;
            default:
                break;
        }
    }*/

    Entity() {

    }
    Entity(string nombreEntidad, double x, double y, int tipoo) {
        nombre = nombreEntidad;
        setPosition(x, y);
        body.speed[0] = 0;
        body.speed[1] = 0;
        head.speed[0] = 0;
        head.speed[1] = 0;
        head.position[0] = x;
        head.position[1] = y+6;
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
    Enemy(string nombreEntidad, double x, double y, int tipoo) {
        nombre = nombreEntidad;
        setPosition(x, y);
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

/*void keySpecialOperations(void) {
    if (keySpecialStates[GLUT_KEY_UP]) {
        player.setposy(player.body.position[1] + 1);
    }
    if (keySpecialStates[GLUT_KEY_DOWN]) {
        player.setposy(player.body.position[1] - 1);
    }
    if (keySpecialStates[GLUT_KEY_LEFT]) {
        player.setposx(player.body.position[0] - 1);
    }
    if (keySpecialStates[GLUT_KEY_RIGHT]) {
        player.setposx(player.body.position[0] + 1);
    }
}*/

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
            x = (int)player.body.position[0] - i;
            y = (int)player.body.position[1] - j;
            if (eucDist(x, y, player.body.position[0], player.body.position[1]) <= 3) {
                glVertex2d(x, y);
            }
        }
    }
    //head
    glColor3f(1.0f, 1.0f, 0.0f);
    for (int i = -3; i <= 3; i++) {
        for (int j = -3; j <= 3; j++) {
            x = (int) player.head.position[0] - i;
            y = (int) player.head.position[1] - j;
            if (eucDist(x, y, player.head.position[0], player.head.position[1]) <= 1.5) {
                glVertex2d(x, y);
            }
        }
*/
    glColor3f(1.0, 0.0, 0.0);
    drawFilledCircle(player.posx, player.posy, 3);


    //Dibujo la accion, si la hay
    if (player.accion != -1) {

        glColor3f(0.8f, 0.8f, 0.0f);
        int cosaQuePlayerHace = player.accion - 1;
        int frameDeLaCosa = player.frameActual;

        for (int j = 0; j < player.actions[cosaQuePlayerHace].animacion.frames.at((unsigned int)frameDeLaCosa).numDots; j++) {
            glVertex2f(player.actions[cosaQuePlayerHace].animacion.frames.at((unsigned int)frameDeLaCosa).dots.at((unsigned int)j).x + player.body.position[0],
                       player.actions[cosaQuePlayerHace].animacion.frames.at((unsigned int)frameDeLaCosa).dots.at((unsigned int)j).y + player.body.position[1]);
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
        glVertex2f(enemies[i].body.position[0], enemies[i].body.position[1]);
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
    //keySpecialOperations();
    playerUpdate();
    //logic();
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