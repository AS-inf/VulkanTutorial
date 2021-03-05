//
// Created by xxx on 26.02.2021.
//

#ifndef MAIN_CPP_VULKANAPP_H
#define MAIN_CPP_VULKANAPP_H

#include "libs.h"
#include "globals.h"



class vulkanApp
{

public:
    void run();
  
private:
	
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
    bool checkValidationLayerSupport();
    void createInstance();
    void setupDebugMessenger();
    std::vector<const char*> getRequiredExtensions();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void *pUserData);

};


#endif //MAIN_CPP_VULKANAPP_H
