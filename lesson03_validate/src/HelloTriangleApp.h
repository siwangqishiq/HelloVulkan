#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

// #define NDEBUG

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance , 
                                      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo , 
                                      const VkAllocationCallbacks *pAllocator, 
                                      VkDebugUtilsMessengerEXT *pDebugMessenger);

void DestoryDebugUtilsMessengerEXT(VkInstance instance,VkDebugUtilsMessengerEXT debugMessenger , const VkAllocationCallbacks *pAllocator);

//队列族查询返回
struct QueueFamilyIndices{
    int graphicsFamily = -1;

    bool isComplete(){
        return graphicsFamily >= 0;
    }
};

class HelloTriangleApp{
public:
    void run();

private:
    GLFWwindow* window;

    //Vulkan实例
    VkInstance vkInstance;
    
    //物理设备
    VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;

    //调试信息
    VkDebugUtilsMessengerEXT debugMessenger;

    void initWindow();

    void initVulkan();

    void setupDebugMessenger();

    void mainLoop();

    void cleanup();

    void createInstance();

    void pickPhysicsDevice();

    bool isDeviceSuitable(VkPhysicalDevice device);

    

    //寻找合适的队列
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    //
    std::vector<const char*> getRequiredExtension();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    bool checkValidationLayerSupport();
    
    //
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
                    VkDebugUtilsMessageTypeFlagsEXT messageType, 
                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }
};