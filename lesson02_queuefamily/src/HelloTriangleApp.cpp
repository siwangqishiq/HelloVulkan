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
}

QueueFamilyIndices HelloTriangleApp::findQueueFamilies(VkPhysicalDevice device){
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device , &queueFamilyCount , nullptr);
     std::cout << " queueFamilyCount " << queueFamilyCount << std::endl;
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device , &queueFamilyCount , queueFamilies.data());

    for(int i = 0 ; i < queueFamilies.size();i++){
        VkQueueFamilyProperties queueFamilyProp = queueFamilies[i];
        std::cout << " queueCount " << queueFamilyProp.queueCount << std::endl;
        std::cout << " queueFlags " << queueFamilyProp.queueFlags << std::endl;
        std::cout << " timestampValidBits " << queueFamilyProp.timestampValidBits << std::endl;
        VkExtent3D extent3d = queueFamilyProp.minImageTransferGranularity;
        std::cout << " minImageTransferGranularity width " << extent3d.width << std::endl;
        std::cout << " minImageTransferGranularity height " << extent3d.height << std::endl;
        std::cout << " minImageTransferGranularity depth " << extent3d.depth << std::endl;

        if(queueFamilyProp.queueCount > 0 && queueFamilyProp.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = i;
            break;
        }
    }

    return indices;
}

bool HelloTriangleApp::isDeviceSuitable(VkPhysicalDevice device){
    QueueFamilyIndices indices = findQueueFamilies(device);
    return indices.isComplete();
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

