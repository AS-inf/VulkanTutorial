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
    window=glfwCreateWindow(WIDTH, HEIGHT, "AS", nullptr, nullptr); // create window and store in vulkanApp.window
}

// create instance and debug layers
void vulkanApp::initVulkan()
{
    
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createGraphicsPipeline();
    
}

// main app loop
void vulkanApp::mainLoop()
{
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}

// clean memory
void vulkanApp::cleanup()
{
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    for(auto imageView : swapChainImageViews) vkDestroyImageView(device, imageView, nullptr);
    vkDestroySwapchainKHR(device, swapChain, nullptr);
    vkDestroyDevice(device, nullptr);
    if(enableValidationLayers) DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);       // disable validation layers
    vkDestroySurfaceKHR(instance, surface, nullptr);                                                   // destroy glfwSurface
    vkDestroyInstance(instance, nullptr);                                                              // destroy API instance
    glfwDestroyWindow(window);                                                                                  // destroy GLF window
    glfwTerminate();                                                                                            // glfw cleanup + exit(0)
}

//create info -> instance
void vulkanApp::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType=VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName="Triangle";
    appInfo.applicationVersion=VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName="First Triangle";
    appInfo.apiVersion=VK_API_VERSION_1_0;
    
    uint32_t glfwExtensionCount=0;
    const char **glfwExtensions;
    glfwExtensions=glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    VkInstanceCreateInfo createInfo{};
    createInfo.sType=VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo=&appInfo;
    createInfo.enabledExtensionCount=glfwExtensionCount;
    createInfo.ppEnabledExtensionNames=glfwExtensions;
    createInfo.enabledExtensionCount=0;
    
    auto extensions=getRequiredExtensions();
    createInfo.enabledExtensionCount=static_cast<unsigned int>(extensions.size());
    createInfo.ppEnabledExtensionNames=extensions.data();
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if(enableValidationLayers)
    {
        if(!checkValidationLayerSupport()) throw std::runtime_error("validation layers not available");
        createInfo.enabledLayerCount=static_cast<unsigned int>(validationLayers.size());
        createInfo.ppEnabledLayerNames=validationLayers.data();
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext=(VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    } else
    {
        createInfo.enabledLayerCount=0;
        createInfo.pNext=nullptr;
    }
    
    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance of vulkanApp!");
    }
    
}

void vulkanApp::createSurface()
{
    if(glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
        throw std::runtime_error("cant create surface");
}

void vulkanApp::createLogicalDevice()
{
    if(physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("NO PHYSICAL DEVICE!");
    
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    float queuePriority = 1.0f;
    for(uint32_t queueFamily: uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos= queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    

    if(enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames= validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }
    
    if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
        throw std::runtime_error("failed to create logical device");
    
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void vulkanApp::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount+1;
    if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    
    //creation time
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface= surface;
    createInfo.minImageCount= imageCount;
    createInfo.imageFormat= surfaceFormat.format;
    createInfo.imageColorSpace= surfaceFormat.colorSpace;
    createInfo.imageExtent= extent;
    createInfo.imageArrayLayers= 1;
    createInfo.imageUsage= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    QueueFamilyIndices indices= findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[]= {indices.graphicsFamily.value(),
                                     indices.presentFamily.value()};
    
    if (indices.graphicsFamily!= indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    
    createInfo.preTransform= swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode= presentMode;
    createInfo.clipped= VK_TRUE;
    createInfo.oldSwapchain= VK_NULL_HANDLE;
    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain)!= VK_SUCCESS)
    {
        throw std::runtime_error("filed to create swap chain");
    }
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
    
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void vulkanApp::createImageViews()
{
    swapChainImageViews.resize(swapChainImages.size());
    for(size_t i =0; i<swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        if(vkCreateImageView(device, &createInfo, nullptr,  &swapChainImageViews[i])!= VK_SUCCESS)
        {
            throw std::runtime_error("cant create Image Views!");
        }
        
    }
    
}

void vulkanApp::createGraphicsPipeline()
{
    std::vector<char> vertShaderCode = readFile("../src/shaders/vert.spv");
    std::vector<char> fragShaderCode = readFile("../src/shaders/frag.spv");
    
    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
    
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";
    
    VkPipelineShaderStageCreateInfo fragShaderInfo{};
    fragShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderInfo.module = fragShaderModule;
    fragShaderInfo.pName = "main";
    
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderInfo};
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions=nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions =nullptr;
    
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.height = (float)swapChainExtent.height;
    viewport.width = (float )swapChainExtent.width;
    viewport.maxDepth = 1.0f;
    viewport.minDepth = 0.0f;
    
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent =  swapChainExtent;
    
    
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = & viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
    
    VkPipelineRasterizationStateCreateInfo  rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;
    
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional
    
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                          VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT |
                                          VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; //Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; //Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; //Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; //    Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
    
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional
    
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
    
    
    
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
    
}

void vulkanApp::setupDebugMessenger()
{
    if(!enableValidationLayers) return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);
    
    if(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger");
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
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    for(const auto &device : devices)
    {
        if(isDeviceSuitable(device))
        {
            physicalDevice=device;
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
    QueueFamilyIndices indices = findQueueFamilies(device);
    bool extensionsSupported = checkDeviceExtensionSupport(device);
    bool swapChainAdequate = false;
    if(extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport= querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    return indices.isComplete() && extensionsSupported && swapChainAdequate && !std::strcmp(deviceProperties.deviceName, "GeForce 840M"); //Intel(R) HD Graphics 4600 (HSW GT2)

}

bool vulkanApp::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionsCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionsCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, availableExtensions.data());
    std::set<std::string> requiedExtensions{deviceExtensions.begin(), deviceExtensions.end()};
    for(const auto& extension :availableExtensions )
    {
        requiedExtensions.erase(extension.extensionName);
    }
    
    return requiedExtensions.empty();
}

vulkanApp::QueueFamilyIndices vulkanApp::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount=0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    int i=0;
    for(const auto &queueFamily : queueFamilies)
    {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if(presentSupport) indices.presentFamily=i;
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphicsFamily=i;
        if (indices.isComplete()) break;
        i++;
    }
    return indices;
}

vulkanApp::SwapChainSupportDetails vulkanApp::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
    
    uint32_t formatCount =0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface,
        &formatCount, details.formats.data());
    }
    
    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }
    
    return details;
}

VkSurfaceFormatKHR vulkanApp::chooseSwapSurfaceFormat(const std::vector <VkSurfaceFormatKHR> &availableFormats)
{
    for(const auto& format : availableFormats)
        if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return format;
    return availableFormats[0];
}

VkPresentModeKHR vulkanApp::chooseSwapPresentMode(const std::vector <VkPresentModeKHR> &availablePresentModes)
{
    for(const auto& presentMode: availablePresentModes)
        if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return presentMode;
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D vulkanApp::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if(capabilities.currentExtent.width != UINT32_MAX)
        return capabilities.currentExtent;
    else
    {
        VkExtent2D actualExtent {WIDTH, HEIGHT};
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

VkShaderModule vulkanApp::createShaderModule(const std::vector<char> &code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    VkShaderModule shaderModule;
    if(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule)!=VK_SUCCESS)
    {
        throw std::runtime_error("Shader Module assemble failed");
    }
    return shaderModule;
}

void vulkanApp::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo={};
    createInfo.sType=VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity=VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType=VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback=debugCallback;
    
}

bool vulkanApp::checkValidationLayerSupport()
{
    unsigned int layerCount=0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    
    for(const char *layerName : validationLayers)
    {
        bool layerFound = false;
        for(const auto &layerProperties : availableLayers)
        {
            if(strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound=true;
                break;
            }
        }
        if(!layerFound)
        {
            return false;
        }
    }
    
    return true;
}

std::vector<const char *> vulkanApp::getRequiredExtensions()
{
    uint32_t glfwExtensionsCount=0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    std::vector<const char* > extensions(glfwExtensions, glfwExtensions+glfwExtensionsCount);
    if(enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vulkanApp::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void *pUserData)
{
    std::cerr<<"MSev: "<<messageSeverity<<"  Mtype: "<<messageType<<"   validation layer: "<<pCallbackData->pMessage<<std::endl;
    return VK_FALSE;            //always return false (true for errors during callback)
}

std::vector<char> vulkanApp::readFile(const std::string& filename)
{
    
    std::ifstream file(filename, std::ios::ate| std::ios::binary);
    if(!file.is_open())
    {
        std::string message("file open failed: ");
        message+=filename;
        throw std::runtime_error(message.c_str());
    }
    size_t fileSize =(size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}




