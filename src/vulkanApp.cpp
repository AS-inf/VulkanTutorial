//  AS-inf
// vulkanApp cpp file
//
#include "vulkanApp.h"

// main vulkanAPI function
void vulkanApp::run()
{
    initWindow();       // window creation
    initVulkan();       // API startup config
    mainLoop();         // app main loop
    cleanup();          // API cleanup; window cleanup and close
}
// GLFW window creation
void vulkanApp::initWindow()
{
    glfwInit();                                                                         //API Init
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);                                       //set non OpenGL api
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);                                         //set window as not resizable
    window = glfwCreateWindow(WIDTH, HEIGHT, "AS", nullptr, nullptr); // create window and store in vulkanApp.window
}
// create instance and debug layers
void vulkanApp::initVulkan()
{
    createInstance();
    setupDebugMessenger();
    pickPhysicalDevice();
}
// main app loop
void vulkanApp::mainLoop()
{
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}
//
void vulkanApp::cleanup()
{
    if(enableValidationLayers) DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);       // disable validation layers
    vkDestroyInstance(instance, nullptr);                                                              // destroy API instance
    glfwDestroyWindow(window);                                                                                  // destroy GLF window
    glfwTerminate();                                                                                            // glfw cleanup + exit(0)
}
//create info -> instance
void vulkanApp::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType= VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName="Triangle";
    appInfo.applicationVersion= VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName= "First Triangle";
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions= glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount= glfwExtensionCount;
    createInfo.ppEnabledExtensionNames= glfwExtensions;
    createInfo.enabledExtensionCount= 0;
    
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<unsigned int>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if(enableValidationLayers)
    {
        if (!checkValidationLayerSupport()) throw std::runtime_error("validation layers not available");
        createInfo.enabledLayerCount= static_cast<unsigned int>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext= (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext =nullptr;
    }
    
    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance of vulkanApp!");
    }
    
    
    //_____________--Extension List--_______________
    uint32_t extensionCount= 0;
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
    populateDebugMessengerCreateInfo(createInfo);
    
    if(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("file to set up debug messenger");
    }
}
void vulkanApp::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if(deviceCount == 0)
    {
        throw std::runtime_error("NO PHYSICAL DEVICE DETECTED");
    }
    std::vector<VkPhysicalDevice> devices (deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    for(const auto& device : devices )
    {
        if(isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
        
    }
    if(physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("NO SUITABLE DEVICE FOUND");
    }
}

bool vulkanApp::isDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    std::cout<<device<<std::endl;
    std::cout<<deviceProperties.deviceName<<std::endl;
    QueueFamilyIndices indices= findQueueFamilies(device);
    return  indices.isComplete() && !std::strcmp(deviceProperties.deviceName, "GeForce 840M"); //Intel(R) HD Graphics 4600 (HSW GT2)
    //return !std::strcmp(deviceProperties.deviceName, "GeForce 840M"); //Intel(R) HD Graphics 4600 (HSW GT2)
}

vulkanApp::QueueFamilyIndices vulkanApp::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    int i=0;
    for(const auto& queueFamily : queueFamilies)
    {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphicsFamily= i;
        i++;
    }
    return indices;
}


void vulkanApp::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    
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
    uint32_t glfwExtensionsCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    std::vector<const char*> extensions (glfwExtensions, glfwExtensions + glfwExtensionsCount);
    if(enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}
VKAPI_ATTR VkBool32 VKAPI_CALL vulkanApp::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void *pUserData)
{
    std::cerr<<"MSev: "<<messageSeverity<<"  Mtype: "<<messageType<<"   validation layer: "<<pCallbackData->pMessage<<std::endl;
    return VK_FALSE;            //always return false (true for errors during callback)
}

