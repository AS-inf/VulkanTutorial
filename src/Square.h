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
        {{-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        
        {{0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
    };
    const std::vector<uint16_t> indices =
            {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
            };
};


#endif //TRIANGLE_SQUARE_H
