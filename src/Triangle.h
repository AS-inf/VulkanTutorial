//
// Created by xxx on 09.04.2021.
//

#ifndef TRIANGLE_TRIANGLE_H
#define TRIANGLE_TRIANGLE_H
#include "libs.h"
#include "globals.h"
#include "Vertex.h"

using namespace std;

class Triangle
{
public:
    
    const vector<Vertex> vertices =
    {
        {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
};


#endif //TRIANGLE_TRIANGLE_H
