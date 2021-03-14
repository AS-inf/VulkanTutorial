//
// Created by xxx on 26.02.2021.
//

#ifndef MAIN_CPP_VULKANAPP_H
#define MAIN_CPP_VULKANAPP_H

#include "libs.h"
#include "globals.h"



class vulkanApp
{
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;

public:
    void run();
  
private:
    void initWindow();          //
    void initVulkan();
    void mainLoop();
    void cleanup();
    
    void createInstance();
    void setupDebugMessenger();
    void pickPhysicalDevice();
    
    void createLogicalDevice();
    
    
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static bool isDeviceSuitable(VkPhysicalDevice device);
    static bool checkValidationLayerSupport();
    
    static std::vector<const char*> getRequiredExtensions();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void *pUserData);
    
    struct QueueFamilyIndices
    {
        std::optional<uint32_t>graphicsFamily;
        bool isComplete() {return graphicsFamily.has_value();};
    };
    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

};


#endif //MAIN_CPP_VULKANAPP_H
