#include "Entity.h"

float Entity::distance(float point1x, float point1y, float point2x, float point2y) {
    float rectHeight = abs(point1y - point2y);
    float rectWidth = abs(point1x - point2x);
    return sqrtf(powf(rectHeight, 2.0) + powf(rectWidth, 2.0));
}

Vector2 Entity::moveToPoint(float point1x, float point1y, float point2x, float point2y){
    float dirVecX, dirVecY;
    point2x = point2x - point1x;
    point2y = point2y - point1y;
    float distancia = distance(point1x, point1y, point2x, point2y);
    if (distancia>0) {
        dirVecX = point2x / distancia;
        dirVecY = point2y / distancia;
    }else{
        dirVecX = 0;
        dirVecY = 1;
    }
    Vector2 cosa;
    cosa[0] = dirVecX;
    cosa[1] = dirVecY;
    return cosa;
}
void Entity::newFrameMovePoints2(World world) {
    bool colX = false;
    float colY = -1;

    float Ax = body.position[0] + body.speed[0];
    float Ay = body.position[1] + body.speed[1];

    float Bx = head.position[0] + head.speed[0];
    float By = head.position[1] + head.speed[1];

    float sAx = body.speed[0] * 0.98f;
    float sAy = (body.speed[1]* 0.98f) - 0.5f;

    float sBx = head.speed[0] * 0.7f;
    float sBy = (head.speed[1] * 0.7f);

    array<float, 6> Cx;
    array<float, 6> Cy;
    array<float, 6> sCx;
    array<float, 6> sCy;
    for (int i=0; i<6; i++){
        Cx[i] = tail[i].position[0] +tail[i].speed[0];
        Cy[i] = tail[i].position[1] +tail[i].speed[1];
        sCx[i] = tail[i].speed[0] * 0.86f;
        sCy[i] = (tail[i].speed[1] *0.86f) - 0.02f;
    }

    float dist = distance(Ax, Ay+4, Bx, By);
    Vector2 rtrn = moveToPoint(Ax, Ay+4, Bx, By);
    float getToDiag = -50.0f;
    float valor = 1.0f;
    float correcionX = (getToDiag-dist)*rtrn.x()*valor;
    float correcionY = (getToDiag-dist)*rtrn.y()*valor;

    array<float,6> distM;
    array<Vector2,6> rtrnM;
    valor = 0.9;
    for (int i=0; i<6; i++) {
        if (i == 0) {
            distM[i] = distance(Bx, By, Cx[i], Cy[i]);
            rtrnM[i] = moveToPoint(Bx, By, Cx[i], Cy[i]);
        } else {
            distM[i] = distance(Cx[i - 1], Cy[i - 1], Cx[i], Cy[i]);
            rtrnM[i] = moveToPoint(Cx[i - 1], Cy[i - 1], Cx[i], Cy[i]);
        }
    }


   /* Ax = Ax - correcionX;
    sAx = sAx - correcionX;

    Ay = Ay - correcionY;
    sAy = sAy - correcionY;*/

    Bx = Bx + correcionX;
    sBx = sBx + correcionX;

    By = By + correcionY;
    sBy = sBy + correcionY;
    for (int i=0; i<6; i++){
        if (i==5){
            correcionX = (getToDiag-distM[i])*rtrnM[i][0]*valor;
            correcionY = (getToDiag-distM[i])*rtrnM[i][1]*valor;

            Cx[i] = Cx[i] + correcionX;
            sCx[i] = sCx[i] + correcionX;

            Cy[i] = Cy[i] + correcionY;
            sCy[i] = sCy[i] + correcionY;
        }
        else{
            correcionX = (getToDiag-distM[i])*rtrnM[i][0]*valor;
            correcionY = (getToDiag-distM[i])*rtrnM[i][1]*valor;

            float correcionX2 = (getToDiag-distM[i+1])*rtrnM[i+1][0]*valor;
            float correcionY2 = (getToDiag-distM[i+1])*rtrnM[i+1][1]*valor;

            Cx[i] = Cx[i] + correcionX - correcionX2;
            sCx[i] = sCx[i] + correcionX - correcionX2;

            Cy[i] = Cy[i] + correcionY - correcionY2;
            sCy[i] = sCy[i] + correcionY - correcionY2;
        }

    }

    for(int i = 0; i < world.platforms.size(); i++){
        if(world.platforms[i].Overlaps(Box({Ax,getPosition()[1]},{3,3}))){
            colX = true;
        }
        if(world.platforms[i].Overlaps(Box({getPosition()[0],Ay},{3,3}))) {
            colY = i;
        }
    }
    if(!colX){
        body.position[0] = Ax;
        body.speed[0] = sAx;

    }
    if(colY == -1){
        body.position[1] = Ay;
        body.speed[1] = sAy;

    }else{
        //Si esta pisando la plataforma
        if(getPosition()[1] > world.platforms[colY].center[1]){
            setPosition(getPosition()[0], world.platforms[colY].center[1]+world.platforms[colY].halfSize[1]+4);
            jump = 0;
            body.speed[1] = 0;
        }else{ //Si la esta tocando por abajo
            setPosition(getPosition()[1], world.platforms[colY].center[1]-world.platforms[colY].halfSize[1]-4);
            body.speed[1] = 0;
        }
    }

    head.position[0] = Bx;
    head.speed[0] = sBx;
    head.position[1] = By;
    head.speed[1] = sBy;
    for (int i =0 ; i<6; i++){
        if (i == 0){
            tail[i].moveToPoint(head.position[0],head.position[1], 0.8);
        }else{
            tail[i].moveToPoint(tail[i-1].position[0],tail[i-1].position[1], 0.8);
        }
    }
/*    for (int i=0; i<6; i++){
        tail[i].position[0] = Cx[i];
        tail[i].speed[0] = sCx[i];
        tail[i].position[1] = Cy[i];
        tail[i].speed[1] = sCy[i];
    }*/
}

void Entity::newFrameMovePoints(World world) {
    bool colX = false;
    float colY = -1;
    float newX, newY;
    vector<Box> platforms = *world.getPlatforms();

    newX = getPosition()[0] + body.speed[0];
    newY = getPosition()[1] + body.speed[1];

    head.position[0] = head.position[0] + head.speed[0];
    head.position[1] = head.position[1] + head.speed[1];

    float airFriction = 0.98f;
    body.speed[0] = body.speed[0] * airFriction;
    body.speed[1] = (body.speed[1] * airFriction)-world.gravity;

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
        head.position = {20,206};
    }else{
        if(!flip){
            head.moveToPoint(getPosition()[0],getPosition()[1]+6, 1.0);
        }else{
            if(flip == 1){
                flipGrades-=0.8;
            }else{
                flipGrades+=0.8;
            }
            head.position = Vector2(getPosition()[0] + cosf((float)flipGrades) * 6, getPosition()[1] + sinf((float)flipGrades) * 6);
            if(flipGrades >= 5*M_PI/2 || flipGrades <= -3*M_PI/2){
                flipGrades = M_PI/2;
                flip = 0;
            }
        }
    }
    //head.moveToPoint(getPosition()[0],getPosition()[1]+6, 1.0);
    for (int i =0 ; i<6; i++){
        if (i == 0){
            tail[i].moveToPoint(head.position[0],head.position[1], 0.8);
        }else{
            tail[i].moveToPoint(tail[i-1].position[0],tail[i-1].position[1], 0.8);
        }
    }
    for (int i =0 ; i<6; i++){
        if (i == 0){
            tailBody[i].moveToPoint(body.position[0],body.position[1], 0.8);
        }else{
            tailBody[i].moveToPoint(tailBody[i-1].position[0],tailBody[i-1].position[1], 0.8);
        }
    }
}

void Entity::moveToPoint(float x, float y, float speed){
    float vecX = x - this->body.position[0];
    float vecY = y - this->body.position[1];
    float mod = sqrtf(powf(vecX,2)+powf(vecY,2));
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

void Entity::setPosition(float x, float y){
        body.position.x() = x;
        body.position.y() = y;
}

Vector2 Entity::getPosition(){
    return body.position;
}

Entity::Entity() {

}
Entity::~Entity(){

}

void Entity::reset(){
    float x = 20;
    float y = 200;
    this->actualAction=-1;
    this->actualFrame=-1;
    this->setPosition(x, y);
    this->body.speed[0] = 0;
    this->body.speed[1] = 0;
    this->head.speed[0] = 0;
    this->head.speed[1] = 0;
    this->head.position[0] = x;
    this->head.position[1] = y+6;
}
Entity::Entity(string entityName, float x, float y, int entityType) {
    this->actualAction=-1;
    this->actualFrame=-1;
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
    for(int i=0; i<6 ; i++){
        this->tail.push_back(Cacho(1));
        tail[i].position[0] = x;
        tail[i].position[1] = y+6;
        tail[i].speed[0] = 0;
        tail[i].speed[1] = 0;
        this->tailBody.push_back(Cacho());
    }
    this->actions.push_back(Action("actionAttack", (char*)"Animaciones/attack.anim"));
    this->actions.push_back(Action("actionWave", (char*)"Animaciones/wave.anim"));
}

void Entity::setSpeed(float x, float y){
    body.speed.x() = x;
    body.speed.y() = y;
}
