//
// Created by xxx on 11.04.2021.
//

#ifndef TRIANGLE_SQUARE_H
#define TRIANGLE_SQUARE_H

#include "libs.h"
#include "globals.h"
#include "Vertex.h"

using namespace std;

class Square
{
public:
    
    const vector<Vertex> vertices =
    {
            {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
};


#endif //TRIANGLE_SQUARE_H
