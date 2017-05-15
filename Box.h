//
// Created by GPA on 13/05/2017.
//

#ifndef PROJECT101_BOX_H
#define PROJECT101_BOX_H

#include <array>
#include "Vector2.h"

class Box {
public:
    Box();
    Box(Vector2 c, Vector2 h);
    bool Overlaps(Box other);

    Vector2 center;
    Vector2 halfSize;
};


#endif //PROJECT101_BOX_H
