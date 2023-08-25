#include "VulkanInstance.h"

#include "VulkanInstance.h"

VulkanInstance::VulkanInstance(bool debug, const char* app_name)
{
    if (debug && !check_validation_support(validation_layers)) {
        throw runtime_error("validation layers requested, but not available!");
    }

    //App Info
    VkApplicationInfo appInfo;

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = 0;

    appInfo.pApplicationName = app_name;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";

    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    appInfo.apiVersion = VK_API_VERSION_1_3;

    //Instance Create Info
    VkInstanceCreateInfo instance_info;

    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = &appInfo;

    instance_info.flags = 0;

    //Extensions
    vector<const char*> extensions = instance_extensions;

    if (debug) { extensions.insert(extensions.end(), debug_instance_extensions.begin(), debug_instance_extensions.end()); }

    instance_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instance_info.ppEnabledExtensionNames = extensions.data();

    //Debugger

    VkDebugUtilsMessengerCreateInfoEXT debugger_info;

    debugger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugger_info.flags = 0;

    debugger_info.pNext = nullptr;
    debugger_info.pUserData = nullptr;

    debugger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    debugger_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    debugger_info.pfnUserCallback = debug_callback;


    if (debug) {

        instance_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        instance_info.ppEnabledLayerNames = validation_layers.data();

        instance_info.pNext = &debugger_info;
    }
    else {

        instance_info.enabledLayerCount = 0;
        instance_info.ppEnabledLayerNames = nullptr;

        instance_info.pNext = nullptr;
    }

    //Instance Creation
    CHECK_VK(vkCreateInstance(&instance_info, nullptr, &instance));

    //Create debugger
    CHECK_VK(CreateDebugUtilsMessengerEXT(instance, &debugger_info, nullptr, &debug_messenger));
}

VulkanInstance::~VulkanInstance()
{
    DestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
    vkDestroyInstance(instance, nullptr);
}

bool VulkanInstance::check_validation_support(const vector<const char*>& requested_validation)
{
    //First list all of the available layers
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    //Next, check if all of the layers in validationLayers exist in the availableLayers list. 

    for (const char* layerName : requested_validation) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
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

VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}