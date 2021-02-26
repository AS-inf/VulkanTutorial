//
// Created by xxx on 26.02.2021.
//

#ifndef MAIN_CPP_LIBS_H
#define MAIN_CPP_LIBS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <iostream>

extern const unsigned int WIDTH;
extern const  unsigned int  HEIGHT;

extern const std::vector <const char*> validationLayers;

extern const bool enabledValidationLayers;




#endif //MAIN_CPP_LIBS_H
