#include "VulkanDevice.h"


VulkanDevice::VulkanDevice(shared_ptr<VulkanInstance> instance, shared_ptr<Surface> surface)
{
    physical_device = select_physical_device(instance->handle(), surface->handle(), device_extensions);
    multisampling_flags = usable_sample_count(physical_device);

    family_indices = find_queue_families(physical_device, surface->handle());
    swapchain_support = query_swapchain_support(physical_device, surface->handle());

    depth_format = find_depth_format(
        physical_device,
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );

    //This structure describes the number of queues we want for a single queue family.

    vector<VkDeviceQueueCreateInfo> queue_create_infos;

    set<uint32_t> uniqueQueueFamilies = { family_indices.graphicsFamily.value(), family_indices.presentFamily.value() };

    for (uint32_t queueFamily : uniqueQueueFamilies) {

        VkDeviceQueueCreateInfo queueCreateInfo{};

        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &HIGH_PRIORITY;

        queue_create_infos.push_back(queueCreateInfo);
    }


    // These are the features that we queried support for with vkGetPhysicalDeviceFeatures in the previous chapter,
    // like geometry shaders.

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo device_info;

    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext = nullptr;
    device_info.flags = 0;


    //Add pointers to the queue creation info and device features structs
    device_info.queueCreateInfoCount = queue_create_infos.size();
    device_info.pQueueCreateInfos = queue_create_infos.data();

    device_info.pEnabledFeatures = &device_features;

    device_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    device_info.ppEnabledExtensionNames = device_extensions.data();

    //validation layers are deprecated on devices,
    //so they are not specified (specify in case of compatability issues)
    device_info.enabledLayerCount = 0;
    device_info.ppEnabledLayerNames = nullptr;

    CHECK_VK(vkCreateDevice(physical_device, &device_info, nullptr, &device));

    vkGetDeviceQueue(device, family_indices.graphicsFamily.value(), 0, &graphics_queue);
    vkGetDeviceQueue(device, family_indices.presentFamily.value(), 0, &present_queue);
}

VulkanDevice::~VulkanDevice()
{
    vkDestroyDevice(device, nullptr);
}

VkPhysicalDevice VulkanDevice::select_physical_device(VkInstance instance, VkSurfaceKHR surface, const vector<const char*>& requested_extensions)
{
    VkPhysicalDevice selected_device = nullptr;

    //Listing the graphics cards starts with querying just the number.
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw runtime_error("Failed to find GPUs with Vulkan support!");
    }

    //Allocate an array to hold all of the VkPhysicalDevice handles.
    vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    //Check if any of the physical devices meet the requirements that we'll add to that functio
    for (const auto& device : devices) {
        if (is_device_suitable(device, surface, requested_extensions)) {
            selected_device = device;
            break;
        }
    }

    //ADD A SUITABILITY COUNTER LATER
    if (selected_device == VK_NULL_HANDLE) {
        throw runtime_error("failed to find a suitable GPU!");
    }

    return selected_device;
}

bool VulkanDevice::is_device_suitable(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const vector<const char*>& requested_extensions)
{

    QueueFamilyIndices indices = find_queue_families(physical_device, surface);
    bool deviceExtensionSupport = check_extension_support(physical_device, requested_extensions);

    bool swapChainAdequate = false;

    if (deviceExtensionSupport) {

        SwapchainSupportInfo swapChainSupport = query_swapchain_support(physical_device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(physical_device, &supportedFeatures);

    return indices.is_complete() && deviceExtensionSupport && swapChainAdequate && supportedFeatures.samplerAnisotropy;;
}

uint VulkanDevice::find_memory_index(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw runtime_error("failed to find suitable memory type!");
}

QueueFamilyIndices VulkanDevice::find_queue_families(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamilyCount, queueFamilies.data());

    //The VkQueueFamilyProperties struct contains some details about the queue family,
    //including the type of operations that are supported and the number of queues that can be created based on that family.
    //We need to find at least one queue family that supports VK_QUEUE_GRAPHICS_BIT.

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.is_complete()) {
            break;
        }

        i++;
    }

    return indices;
}

bool VulkanDevice::check_extension_support(VkPhysicalDevice physical_device, const vector<const char*>& requested_extensions)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensionCount, nullptr);

    vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensionCount, availableExtensions.data());

    set<string> extension_set(requested_extensions.begin(), requested_extensions.end());

    for (const auto& extension : availableExtensions) {
        extension_set.erase(extension.extensionName);
    }


    return extension_set.empty();
}

VkFormat VulkanDevice::find_depth_format(VkPhysicalDevice physical_device, const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physical_device, format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw runtime_error("failed to find supported format!");
}

SwapchainSupportInfo VulkanDevice::query_swapchain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    SwapchainSupportInfo details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);

    //querying the supported surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formatCount, details.formats.data());
    }

    //Querying the supported presentation modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentModeCount, details.presentModes.data());
    }


    return details;
}

VkSampleCountFlagBits VulkanDevice::usable_sample_count(VkPhysicalDevice physical_device)
{
    VkPhysicalDeviceProperties physical_device_properties;
    vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);

    VkSampleCountFlags counts = physical_device_properties.limits.framebufferColorSampleCounts &
        physical_device_properties.limits.framebufferDepthSampleCounts;

    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}