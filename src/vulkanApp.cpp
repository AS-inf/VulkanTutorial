//
// Created by xxx on 26.02.2021.
//

#include "vulkanApp.h"


void vulkanApp::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}
void vulkanApp::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "AS", nullptr, nullptr);
}

void vulkanApp::initVulkan()
{
    createInstance();
}
void vulkanApp::mainLoop()
{
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}

void vulkanApp::cleanup()
{
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool vulkanApp::checkValidationLayerSupport()
{
    unsigned int layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    
    for(const char* layerName :validationLayers)
    {
        bool layerFound = false;
        for(const auto& layerProperties : availableLayers)
        {
            if(strcmp(layerName, layerProperties.layerName)== 0)
            {
                layerFound = true;
                break;
            }
        }
        if (!layerFound)
        {
            return false;
        }
    }
    
    return true;
}
void vulkanApp::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType= VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName="Triangle";
    appInfo.applicationVersion= VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName= "First Triangle";
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions= glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount= glfwExtensionCount;
    createInfo.ppEnabledExtensionNames= glfwExtensions;
    createInfo.enabledExtensionCount= 0;
    
    unsigned int extensionCount= 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    
    for(const auto& extension: extensions)
    {
        std::cout<<'\t'<<extension.extensionName<<'\n';
    }
    
    
    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance of vulkanApp!");
    }
    if(enabledValidationLayers && !checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers not available");
    }
}
