#include "HelloTriangleApp.h"

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance , const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo , 
const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger){
    // return vkCreateDebugUtilsMessengerEXT(instance , pCreateInfo , pAllocator , pDebugMessenger);
    // return VK_SUCCESS;
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance , "vkCreateDebugUtilsMessengerEXT");
    std::cout << "vkGetInstanceProcAddr vkCreateDebugUtilsMessengerEXT " << std::endl;
    if(func != nullptr){
        std::cout << "vkCreateDebugUtilsMessengerEXT func success" << std::endl;
        return func(instance , pCreateInfo , pAllocator , pDebugMessenger);
    }else{
        std::cout << "vkGetInstanceProcAddr vkCreateDebugUtilsMessengerEXT error" << std::endl;
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestoryDebugUtilsMessengerEXT(VkInstance instance,VkDebugUtilsMessengerEXT debugMessenger , const VkAllocationCallbacks *pAllocator){

    // vkDestroyDebugUtilsMessengerEXT(instance , debugMessenger , pAllocator);
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance , "vkDestroyDebugUtilsMessengerEXT");
    
    if(func != nullptr){
        std::cout << "Get vkDestroyDebugUtilsMessengerEXT func success" << std::endl;
        func(instance , debugMessenger , pAllocator);
    }
}

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
    setupDebugMessenger();
    pickPhysicsDevice();
}

void HelloTriangleApp::setupDebugMessenger(){
    if(!enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    
    if(CreateDebugUtilsMessengerEXT(vkInstance , &createInfo , nullptr , &debugMessenger) != VK_SUCCESS){
        throw std::runtime_error("failed to setup debug messenger!");
    }
}

std::vector<const char*> getRequiredExtension(){
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions ,  glfwExtensions + glfwExtensionCount);
    if(enableValidationLayers){
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void HelloTriangleApp::createInstance(){
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

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
    
    std::vector<const char*> extensions = getRequiredExtension();
    std::cout << "getRequiredExtension : " << std::endl;
    for(int i = 0 ; i < extensions.size() ;i++){
        std::cout << extensions[i] << std::endl;
    }//end for i

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if(enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    }else{
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }
    
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
    if(enableValidationLayers){
        DestoryDebugUtilsMessengerEXT(vkInstance , debugMessenger , nullptr);
    }

    vkDestroyInstance(vkInstance , nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
}


std::vector<const char*> HelloTriangleApp::getRequiredExtension() {
    std::vector<const char*> extensions;
    
    unsigned int glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for(unsigned int i = 0 ; i < glfwExtensionCount ; i++){
        extensions.push_back(glfwExtensions[i]);
    }//end for i
    //VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    if(enableValidationLayers){
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void HelloTriangleApp::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
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

bool HelloTriangleApp::checkValidationLayerSupport(){
    uint32_t layerCount;

    vkEnumerateInstanceLayerProperties(&layerCount , nullptr);
    std::cout << "layer Count = " << layerCount << std::endl;

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount , availableLayers.data());

    for(VkLayerProperties &prop : availableLayers){
        std::cout << prop.layerName << std::endl;
    }//end for each
    
    for (const char* layerName : validationLayers) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                std::cout << "Found layer " << layerName << std::endl;
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

