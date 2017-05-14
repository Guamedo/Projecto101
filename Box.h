//
// Created by GPA on 13/05/2017.
//

#ifndef PROJECT101_BOX_H
#define PROJECT101_BOX_H

#include <array>

class Box {
public:
    Box();
    Box(std::array<double, 2> c, std::array<double, 2> h);
    bool Overlaps(Box other);

    std::array<double, 2> center;
    std::array<double, 2> halfSize;
};


#endif //PROJECT101_BOX_H
