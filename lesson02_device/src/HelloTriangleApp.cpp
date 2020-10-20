#include "HelloTriangleApp.h"
#include <vector>

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
    pickPhysicsDevice();
}

void HelloTriangleApp::createInstance(){
    VkApplicationInfo appInfo = {};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
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
    
    for(int i = 0 ; i < glfwExtensionCount ;i++){
        std::cout << glfwExtension[i] << std::endl;
    }//end for i

    createInfo.enabledLayerCount = 0;
    if(vkCreateInstance(&createInfo , nullptr, &vkInstance) != VK_SUCCESS){
        throw std::runtime_error("failed to create vulkan instance!");
    }
}

void HelloTriangleApp::pickPhysicsDevice(){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance , &deviceCount , nullptr);

    std::cout << "deviceCount " << deviceCount << std::endl;
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance , &deviceCount , devices.data());

    for(const auto &device : devices){
        if(isDeviceSuitable(device)){
            vkPhysicalDevice = device;
            break;
        }
    }//end for each

    if(vkPhysicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("Fail to found a suitable GPU!");
    }

    int32_t familyQueueCount = 0;
}

bool HelloTriangleApp::isDeviceSuitable(VkPhysicalDevice device){
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device , &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device , &deviceFeatures);
    
    std::cout << "apiVersion    : " << deviceProperties.apiVersion << std::endl;
    std::cout << "deviceID      : " << deviceProperties.deviceID << std::endl;
    std::cout << "deviceName    : " << deviceProperties.deviceName << std::endl;
    std::cout << "deviceType    : " << deviceProperties.deviceType << std::endl;
    std::cout << "driverVersion : " << deviceProperties.driverVersion << std::endl;
    //std::cout << "pipelineCacheUUID : " << deviceProperties.pipelineCacheUUID << std::endl;
    //std::cout << "deviceName    : " << deviceProperties.deviceName << std::endl;

    std::cout << "geometryShader support : " << deviceFeatures.geometryShader << std::endl;
    return true;
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

