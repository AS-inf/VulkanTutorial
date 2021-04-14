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
    
    const std::vector<Vertex> vertices =
    {
        {{-1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
        {{1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}},
        {{1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{-1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        
        {{-.9f, -.9f}, {0.0f, 0.0f, 0.0f}},
        {{.9f, -.9f}, {0.0f, 0.0f, 0.0f}},
        {{.9f, .9f},  {0.0f, 0.0f, 0.0f}},
        {{-.9f, .9f}, {0.0f, 0.0f, 0.0f}},
        
        
        {{-.9f, -.9f}, {0.0f, 0.0f, 0.0f}},
        {{0.0f, -.9f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{-.9f, 0.0f}, {1.0f, .0f, .0f}},
        
        {{0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
        {{.9f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{.9f, .9f}, {0.0f, 0.0f, .00f}},
        {{0.0f, .9f}, {0.0f, 0.0f, 1.0f}}
    };
    const std::vector<uint16_t> indices =
            {
            0, 1, 2, 2, 3, 0,
            0+4, 1+4, 2+4, 2+4, 3+4, 0+4,
            0+8, 1+8, 2+8, 2+8, 3+8, 0+8,
            0+12, 1+12, 2+12, 2+12, 3+12, 0+12
            };
};


#endif //TRIANGLE_SQUARE_H
