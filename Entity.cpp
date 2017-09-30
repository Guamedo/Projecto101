#include "Entity.h"

float Entity::distance(float point1x, float point1y, float point2x, float point2y) {
    float rectHeight = abs(point1y - point2y);
    float rectWidth = abs(point1x - point2x);
    return sqrtf(powf(rectHeight, 2.0) + powf(rectWidth, 2.0));
}

Vector2 Entity::moveToPoint(float point1x, float point1y, float point2x, float point2y){
    float dirVecX, dirVecY;
    float p2x = point2x - point1x;
    float p2y = point2y - point1y;
    float distancia = distance(point1x, point1y, point2x, point2y);
    if (distancia>0) {
        dirVecX = p2x / distancia;
        dirVecY = p2y / distancia;
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

    float Ax = body.getPosition().x() + body.getSpeed().x();
    float Ay = body.getPosition().y() + body.getSpeed().y();

    float sAx = body.getSpeed().x() * 0.66f;
    float sAy = (body.getSpeed().y()* 0.98f) - world.gravity;

    //Colisiones del cuerpo principal con las plataformas
    for(int i = 0; i < world.platforms.size(); i++){
        if(world.platforms[i].Overlaps(Box({Ax,getPosition()[1]},{3,3}))){
            colX = true;
        }
        if(world.platforms[i].Overlaps(Box({getPosition()[0],Ay},{3,3}))) {
            colY = i;
        }
    }
    if(!colX){
        body.setPositionX(Ax);
        body.setSpeedX(sAx);
    }else{
        body.setSpeedX(0);
    }
    if(colY == -1){
        body.setPositionY(Ay);
        body.setSpeedY(sAy);
    }else{
        //Si esta pisando la plataforma
        if(getPosition()[1] > world.platforms[colY].center[1]){
            setPosition(getPosition()[0], world.platforms[colY].center[1]+world.platforms[colY].halfSize[1]+4);
            jump = 0;
            body.setSpeedY(0);
        }else{ //Si la esta tocando por abajo
            setPosition(getPosition()[0], world.platforms[colY].center[1]-world.platforms[colY].halfSize[1]-4);
            body.setSpeedY(0);
        }
    }

    float sBx = head.getSpeed().x() * 0.78f;
    float sBy = (head.getSpeed().y() * 0.78f);

    float Bx = head.getPosition().x() + head.getSpeed().x();
    float By = head.getPosition().y() + head.getSpeed().y();

    array<float, 6> Cx;
    array<float, 6> Cy;
    array<float, 6> sCx;
    array<float, 6> sCy;

    for (int i=0; i<6; i++){
        Cx[i] = tail[i].getPosition().x() +tail[i].getSpeed().x();
        Cy[i] = tail[i].getPosition().y() +tail[i].getSpeed().y();
        sCx[i] = tail[i].getSpeed().x() * 0.78f;
        sCy[i] = (tail[i].getSpeed().y() *0.78f) - world.gravity;
    }
    //Dist entre el punto en el que se encuentra la cabeza, y el punto en el que queremos que esté la cabeza
    float dist = distance(body.getPosition().x(), body.getPosition().y()+4, Bx, By);
    //Un vector que apunta de la posicion de la cabeza a la posicion en la que queremos que esté
    Vector2 rtrn = moveToPoint(body.getPosition().x(), body.getPosition().y()+4, Bx, By);
    //Valor de distancia a la que queremos que este la cabeza de el punto en el que queremos que este, al ser 0, significa que queremos que este justo en un punto concreto, si el valor es 1, queremos que este en cualquier punto que este a 1 de distance
    float getToDiag = 0.05f;
    //Factor por el que se aplica la fuerza calculada
    float valor = 0.55f;
    //Resultado final
    float correcionX = (getToDiag-dist)*rtrn.x()*valor;
    float correcionY = (getToDiag-dist)*rtrn.y()*valor;

    //Aplicar la correccion
    Bx = Bx + correcionX;
    sBx = sBx + correcionX;

    By = By + correcionY;
    sBy = sBy + correcionY;

    //Calculamos los mismos valores para la tail.
    array<float,6> distM;
    array<Vector2,6> rtrnM;

    getToDiag = 4;
    valor = 1;

    for (int i=0; i<6; i++) {
        if (i == 0) {

            distM[i] = distance(body.getPosition().x(), body.getPosition().y(), Cx[i], Cy[i]); //cambiado B por A
            rtrnM[i] = moveToPoint(body.getPosition().x(), body.getPosition().y(), Cx[i], Cy[i]);

        } else {

            distM[i] = distance(Cx[i - 1], Cy[i - 1], Cx[i], Cy[i]);
            rtrnM[i] = moveToPoint(Cx[i - 1], Cy[i - 1], Cx[i], Cy[i]);

        }
        correcionX = (getToDiag-distM[i])*rtrnM[i][0]*valor;
        correcionY = (getToDiag-distM[i])*rtrnM[i][1]*valor;

        Cx[i] = Cx[i] + correcionX;
        sCx[i] = sCx[i] + correcionX;

        Cy[i] = Cy[i] + correcionY;
        sCy[i] = sCy[i] + correcionY;

        colX = false;
        colY = -1;
    }

    head.setPosition(Vector2(Bx,By));
    head.setSpeed(Vector2(sBx,sBy));

    for (int i=0; i<6; i++){
        tail[i].setPositionX(Cx[i]);
        tail[i].setSpeedX(sCx[i]);
        tail[i].setPositionY(Cy[i]);
        tail[i].setSpeedY(sCy[i]);
    }
}

void Entity::newFrameMovePoints(World world) {
    bool colX = false;
    float colY = -1;
    float newX, newY;
    vector<Box> platforms = *world.getPlatforms();

    newX = body.getPosition().x() + body.getSpeed().x();
    newY = body.getPosition().y() + body.getSpeed().y();

    float airFriction = 0.98f;
    body.setSpeed(Vector2(body.getSpeed().x()*airFriction, (body.getSpeed().y() * airFriction) - world.gravity));

    for(int i = 0; i < platforms.size(); i++){
        if(platforms[i].Overlaps(Box({newX,getPosition()[1]},{3,3}))){
            colX = true;
        }
        if(platforms[i].Overlaps(Box({getPosition()[0],newY},{3,3}))) {
            colY = i;
        }
    }
    if(!colX){
        body.setPositionX(newX);
    }
    if(colY == -1){
        body.setPositionY(newY);
    }else{
        //Si esta pisando la plataforma
        if(getPosition()[1] > platforms[colY].center[1]){
            setPosition(getPosition()[0], platforms[colY].center[1]+platforms[colY].halfSize[1]+4);
            jump = 0;
            body.setSpeedY(0);
        }else{ //Si la esta tocando por abajo
            body.setPositionY(platforms[colY].center[1]-platforms[colY].halfSize[1]-4);
            //setPosition(getPosition()[0], platforms[colY].center[1]-platforms[colY].halfSize[1]-4);
            body.setSpeedY(0);

        }
    }
    if(getPosition()[1] < -100){
        body.setPosition(Vector2(20,200));
        head.setPosition(Vector2(20,206));
    }else{
        if(!flip){
            head.moveToPoint(getPosition()[0],getPosition()[1]+6, 1.0);
        }else{
            if(flip == 1){
                flipGrades-=0.8;
            }else{
                flipGrades+=0.8;
            }
            head.setPosition(Vector2(getPosition()[0] + cosf((float)flipGrades) * 6, getPosition()[1] + sinf((float)flipGrades) * 6));
            if(flipGrades >= 5*M_PI/2 || flipGrades <= -3*M_PI/2){
                flipGrades = M_PI/2;
                flip = 0;
            }
        }
    }
    //head.moveToPoint(getPosition()[0],getPosition()[1]+6, 1.0);
    for (int i =0 ; i<6; i++){
        if (i == 0){
            tail[i].moveToPoint(head.getPosition().x(),head.getPosition().y(), 0.8);
        }else{
            tail[i].moveToPoint(tail[i-1].getPosition().x(),tail[i-1].getPosition().y(), 0.8);
        }
    }
    for (int i =0 ; i<6; i++){
        if (i == 0){
            tailBody[i].moveToPoint(body.getPosition().x(),body.getPosition().y(), 0.8);
        }else{
            tailBody[i].moveToPoint(tailBody[i-1].getPosition().x(),tailBody[i-1].getPosition().y(), 0.8);
        }
    }
}

void Entity::moveToPoint(float x, float y, float speed){
    float vecX = x - this->body.getPosition().x();
    float vecY = y - this->body.getPosition().y();
    float mod = sqrtf(powf(vecX,2)+powf(vecY,2));
    vecX = (vecX*speed)/mod;
    vecY = (vecY*speed)/mod;
    this->setPosition(this->body.getPosition().x()+vecX, this->body.getPosition().y()+vecY);
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
        body.setPosition(Vector2(x,y));
        //body.position.x() = x;
        //body.position.y() = y;
}

Vector2 Entity::getPosition(){
    return body.getPosition();
}

int Entity::getDash(){
    return dash;
}
void Entity::setDash(int x){
    dash = x;
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
    this->body.setSpeed(Vector2(0,0));
    //this->body.speed[0] = 0;
    //this->body.speed[1] = 0;
    this->head.setSpeed(Vector2(0,0));
    //this->head.speed[0] = 0;
    //this->head.speed[1] = 0;
    this->head.setPosition(Vector2(x, y+6));
    //this->head.position[0] = x;
    //this->head.position[1] = y+6;
}
Entity::Entity(string entityName, float x, float y, int entityType) {
    this->actualAction=-1;
    this->actualFrame=-1;
    this->dash = 0;
    this->name = entityName;
    this->setPosition(x, y);
    this->body.setSpeed(Vector2(0,0));
    this->head.setSpeed(Vector2(0,0));
    this->head.setPosition(Vector2(x, y+6));
    this->BBox = Box({x,y}, {3,3});
    this->type = entityType;
    for(int i=0; i<6 ; i++){
        this->tail.push_back(Cacho(1));
        tail[i].setPosition(Vector2(x,y+6));
        tail[i].setSpeed(Vector2(0,0));
        this->tailBody.push_back(Cacho());
    }
    this->actions.push_back(Action("actionAttack", (char*)"Animaciones/attack.anim"));
    this->actions.push_back(Action("actionWave", (char*)"Animaciones/wave.anim"));
}

void Entity::setSpeed(float x, float y){
    body.setSpeed(Vector2(x,y));
}
