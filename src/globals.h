//
// Created by xxx on 05.03.2021.
//

#ifndef VULKANTUTORIAL_GLOBALS_H
#define VULKANTUTORIAL_GLOBALS_H

#include "libs.h"

//FUNCTIONS

VkResult CreateDebugUtilsMessengerEXT(
     VkInstance instance,
     const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
     const VkAllocationCallbacks* pAllocator,
     VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator);



//VARIABLES
extern const unsigned int WIDTH;
extern const  unsigned int  HEIGHT;

extern const std::vector <const char*> validationLayers;

extern const bool enableValidationLayers;



#endif //VULKANTUTORIAL_GLOBALS_H
