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

const int WIDTH = 800;
const int HEIGHT = 600;

class HelloTriangleApp{
public:
    void run();

private:
    GLFWwindow* window;

    //Vulkan实例
    VkInstance vkInstance;
    
    //物理设备
    VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

    void createInstance();

    void pickPhysicsDevice();

    bool isDeviceSuitable(VkPhysicalDevice device);
};