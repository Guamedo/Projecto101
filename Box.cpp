//
// Created by GPA on 13/05/2017.
//
#include <stdlib.h>
#include "Box.h"

Box::Box() {}

Box::Box(std::array<int, 2> c, std::array<int, 2> h) {
    this->center = c;
    this->halfSize = h;
}

bool Box::Overlaps(Box other) {
    if ( abs(center[0] - other.center[0]) > halfSize[0] + other.halfSize[0] ){
        return false;
    }
    if ( abs(center[1] - other.center[1]) > halfSize[1] + other.halfSize[1] ){
        return false;
    }
    return true;
}