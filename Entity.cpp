#include "Entity.h"

double Entity::distance(double point1x, double point1y, double point2x, double point2y) {
    double rectHeight = abs(point1y - point2y);
    double rectWidth = abs(point1x - point2x);
    return sqrt(pow(rectHeight, 2.0) + pow(rectWidth, 2.0));
}

array<double,2> Entity::moveToPoint(double point1x, double point1y, double point2x, double point2y){
    double dirVecX, dirVecY;
    point2x = point2x - point1x;
    point2y = point2y - point1y;
    double distancia = distance(point1x, point1y, point2x, point2y);
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

void Entity::newFrameMovePoints(World world) {
    bool colX = false;
    double colY = -1;
    double newX, newY;
    vector<Box> platforms = world.getPlatforms();

    newX = getPosition()[0] + body.speed[0];
    newY = getPosition()[1] + body.speed[1];

    head.position[0] = head.position[0] + head.speed[0];
    head.position[1] = head.position[1] + head.speed[1];

    double airFriction = 0.98;
    double grav = 0.5;
    body.speed[0] = body.speed[0] * airFriction;
    body.speed[1] = (body.speed[1] * airFriction)-grav;

    for(int i = 0; i < platforms.size(); i++){
        if(platforms[i].Overlaps(Box({newX,getPosition()[1]},{3,3}))){
            colX = true;
        }
        if(platforms[i].Overlaps(Box({getPosition()[0],newY},{3,3}))) {
            colY = i;
        }
    }
    if(!colX){
        body.position[0] = newX;
    }
    if(colY == -1){
        body.position[1] = newY;
    }else{
        //Si esta pisando la plataforma
        if(getPosition()[1] > platforms[colY].center[1]){
            setPosition(getPosition()[0], platforms[colY].center[1]+platforms[colY].halfSize[1]+4);
            jump = 0;
            body.speed[1] = 0;
        }else{ //Si la esta tocando por abajo
            setPosition(getPosition()[1], platforms[colY].center[1]-platforms[colY].halfSize[1]-4);
            body.speed[1] = 0;

        }
    }
    if(getPosition()[1] < -100){
        body.position = {20,200};
        head.position = {20.206};
    }
    head.moveToPoint(getPosition()[0],getPosition()[1]+6, 1.0);
}

void Entity::moveToPoint(double x, double y,double speed){
    double vecX = x - this->body.position[0];
    double vecY = y - this->body.position[1];
    double mod = sqrt(pow(vecX,2)+pow(vecY,2));
    vecX = (vecX*speed)/mod;
    vecY = (vecY*speed)/mod;
    this->setPosition(this->body.position[0]+vecX, this->body.position[1]+vecY);
}

void Entity::attack() {
    if (actualAction == -1) {
        actualAction = 1;
        actualFrame = 0;
    }
}

void Entity::wave() {
    if (actualAction == -1) {
        actualAction = 2;
        actualFrame = 0;
    }
}

void Entity::setPosition(double x, double y){
        body.position[0] = x;
        body.position[1] = y;
}

array<double, 2> Entity::getPosition(){
    return body.position;
}

Entity::Entity() {

}
Entity::~Entity(){

}
Entity::Entity(string entityName, double x, double y, int entityType) {
    this->name = entityName;
    this->setPosition(x, y);
    this->body.speed[0] = 0;
    this->body.speed[1] = 0;
    this->head.speed[0] = 0;
    this->head.speed[1] = 0;
    this->head.position[0] = x;
    this->head.position[1] = y+6;
    this->BBox = Box({x,y}, {3,3});
    this->type = entityType;
    this->actions.push_back(Action("actionAttack", (char*)"Animaciones/attack.anim"));
    this->actions.push_back(Action("actionWave", (char*)"Animaciones/wave.anim"));
}

void Entity::setSpeed(double x, double y){
    body.speed[0] = x;
    body.speed[1] = y;
}
