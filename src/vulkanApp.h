//
// Created by xxx on 26.02.2021.
//

#ifndef MAIN_CPP_VULKANAPP_H
#define MAIN_CPP_VULKANAPP_H

#include "libs.h"
#include "globals.h"
#include "Vertex.h"
#include "Triangle.h"

class vulkanApp
{
    GLFWwindow* window;                                                                             // glfw window handle init in initWindow() rm in cleanup()
    VkInstance instance;                                                                            // holds main apl info init in createInstance()
    VkDebugUtilsMessengerEXT debugMessenger;                                                        //
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;                                               // handle for physical device
    VkDevice device;                                                                                // handle for logical device
    VkQueue graphicsQueue;                                                                          //
    VkSurfaceKHR surface;                                                                           //
    VkQueue presentQueue;                                                                           //
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};            //
    VkSwapchainKHR swapChain;                                                                       //
    std::vector<VkImage> swapChainImages;                                                           //
    VkFormat swapChainImageFormat;                                                                  //
    VkExtent2D  swapChainExtent;                                                                    //
    std::vector<VkImageView> swapChainImageViews;                                                   //
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame{0};
    bool framebufferResized{false};
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    Triangle triangle;
    

public:
    void run();
  
private:
    void initWindow();          //
    void initVulkan();
    void mainLoop();
    void cleanup();
    
    void cleanupSwapChain();
    
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createCommandPool();
    void createVertexBuffer();
    void createCommandBuffers();
    void createSyncObjects();
    
    void drawFrame();
    
    void recreateSwapChain();
    
    bool isDeviceSuitable(VkPhysicalDevice physDevice);
    bool checkDeviceExtensionSupport(VkPhysicalDevice physDevice);
    
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    
    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    
    static std::vector<char> readFile(const std::string& filename);
    static std::vector<const char*> getRequiredExtensions();
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static bool checkValidationLayerSupport();
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    
    struct QueueFamilyIndices
    {
        std::optional<uint32_t>graphicsFamily;
        std::optional<uint32_t>presentFamily;
        
        [[nodiscard]] bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        };
    };
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physDevice);
    
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physDevice);
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        [[maybe_unused]] void *pUserData);
    static void show( std::bitset<8> z, const char* s);
    
    
};


#endif //MAIN_CPP_VULKANAPP_H
