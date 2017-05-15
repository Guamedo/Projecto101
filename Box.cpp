//
// Created by GPA on 13/05/2017.
//
#include <stdlib.h>
#include <cmath>
#include "Box.h"

Box::Box() {}

Box::Box(Vector2 c, Vector2 h) {
    this->center = c;
    this->halfSize = h;
}

bool Box::Overlaps(Box other) {
    if ( fabs(center.x() - other.center.x()) > halfSize.x() + other.halfSize.x()){
        return false;
    }else if ( fabs(center.y() - other.center.y()) > halfSize.y() + other.halfSize.y() ){
        return false;
    }
    return true;
}