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
    VkSurfaceKHR surface;

public:
    void run();
  
private:
    void initWindow();          //
    void initVulkan();
    void mainLoop();
    void cleanup();
    
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    
    bool isDeviceSuitable(VkPhysicalDevice device);
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static bool checkValidationLayerSupport();
    static std::vector<const char*> getRequiredExtensions();
    struct QueueFamilyIndices
    {
        
        std::optional<uint32_t>graphicsFamily;
        std::optional<uint32_t>presentFamily;
        
        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        };
    };
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
    (
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void *pUserData
    );
    
};


#endif //MAIN_CPP_VULKANAPP_H
