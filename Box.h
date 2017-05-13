//
// Created by GPA on 13/05/2017.
//

#ifndef PROJECT101_BOX_H
#define PROJECT101_BOX_H


#include <array>

class Box {
public:
    Box();
    Box(std::array<int, 2> c, std::array<int, 2> h);
    bool Overlaps(Box other);

    std::array<int, 2> center;
    std::array<int, 2> halfSize;
};


#endif //PROJECT101_BOX_H
