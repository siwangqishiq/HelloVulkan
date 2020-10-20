#include "HelloTriangleApp.h"

void HelloTriangleApp::run(){
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApp::initWindow(){
    glfwInit();

    glfwInitHint(GLFW_CLIENT_API , GLFW_NO_API);
    glfwInitHint(GLFW_RESIZABLE , GLFW_FALSE);

    window = glfwCreateWindow(WIDTH , HEIGHT , "Hello Vulkan" , nullptr , nullptr);
}

void HelloTriangleApp::initVulkan(){
    createInstance();
}

void HelloTriangleApp::createInstance(){
    VkApplicationInfo appInfo = {};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1 , 0 , 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1 , 0 , 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtension;
    glfwExtension = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtension;

    createInfo.enabledLayerCount = 0;
    if(vkCreateInstance(&createInfo , nullptr, &vkInstance) != VK_SUCCESS){
        throw std::runtime_error("failed to create vulkan instance!");
    }
}

void HelloTriangleApp::mainLoop(){
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void HelloTriangleApp::cleanup(){
    vkDestroyInstance(vkInstance , nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
}

