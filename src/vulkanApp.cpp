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
    setupDebugMessenger();
}
void vulkanApp::mainLoop()
{
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}



bool vulkanApp::checkValidationLayerSupport()
{
    unsigned int layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    
    for(const char* layerName : validationLayers)
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

std::vector<const char*> vulkanApp::getRequiredExtensions()
{
    unsigned int glfwExtensionsCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    std::vector<const char*> extensions (glfwExtensions, glfwExtensions + glfwExtensionsCount);
    if(enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
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
    
    if(enableValidationLayers && !checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers not available");
    }
    
    if(enableValidationLayers)
    {
        createInfo.enabledLayerCount= static_cast<unsigned int>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
         createInfo.enabledLayerCount = 0;
    }
    
    
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<unsigned int>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    
    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance of vulkanApp!");
    }
    
    
    unsigned int extensionCount= 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensionsEnumerate(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionsEnumerate.data());

    for(const auto& extension: extensionsEnumerate)
    {
        std::cout<<'\t'<<extension.extensionName<<'\n';
    }

}

void vulkanApp::setupDebugMessenger()
{
    if(!enableValidationLayers) return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
    
    if(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("file to set up debug messenger");
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanApp::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void *pUserData)
{
    std::cerr<<"validation layer: "<<pCallbackData->pMessage<<std::endl;
    return VK_FALSE;
}

void vulkanApp::cleanup()
{
    if(enableValidationLayers) DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}