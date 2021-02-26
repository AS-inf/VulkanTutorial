//
// Created by xxx on 26.02.2021.
//

#ifndef MAIN_CPP_VULKANAPP_H
#define MAIN_CPP_VULKANAPP_H

#include "libs.h"

class vulkanApp
{

public:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }
private:
    GLFWwindow* window;
    VkInstance instance;
    void initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "AS", nullptr, nullptr);
        
    }
    
    void initVulkan()
    {
        createInstance();
    }
    void mainLoop()
    {
        while(!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }
    
    void cleanup()
    {
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void createInstance()
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
        
    }

};


#endif //MAIN_CPP_VULKANAPP_H
