//
// Created by xxx on 26.02.2021.
//

#ifndef MAIN_CPP_VULKANAPP_H
#define MAIN_CPP_VULKANAPP_H

#include "libs.h"

class vulkanApp
{

public:
    void run();
  
private:
	
    GLFWwindow* window;
    VkInstance instance;
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
    bool checkValidationLayerSupport();
    void createInstance();
};


#endif //MAIN_CPP_VULKANAPP_H
