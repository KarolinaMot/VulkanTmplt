#include "Swapchain.h"

Swapchain::Swapchain(shared_ptr<VulkanDevice> device, shared_ptr<Surface> surface, shared_ptr<GLFW_Window> window)
{
    owning_device = device;

    SwapchainSupportInfo swapchain_support = device->get_swapchain_support_info();
    QueueFamilyIndices indices = device->get_queue_family_indices();

    surface_format = select_format(swapchain_support);
    extent = select_extent(swapchain_support, window->handle());

    VkSwapchainCreateInfoKHR swapchain_info;

    swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_info.flags = 0;
    swapchain_info.pNext = nullptr;

    swapchain_info.surface = surface->handle();

    //Image count
    uint image_count = swapchain_support.capabilities.minImageCount + 1;
    if (swapchain_support.capabilities.maxImageCount > 0 && image_count > swapchain_support.capabilities.maxImageCount) {
        image_count = swapchain_support.capabilities.maxImageCount;
    }

    swapchain_info.minImageCount = image_count;

    //format
    swapchain_info.imageFormat = surface_format.format;
    swapchain_info.imageColorSpace = surface_format.colorSpace;
    swapchain_info.imageExtent = extent;

    swapchain_info.imageArrayLayers = 1;
    swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    //Queue Families (only one)
    swapchain_info.queueFamilyIndexCount = 1;
    swapchain_info.pQueueFamilyIndices = &indices.graphicsFamily.value();

    if (indices.graphicsFamily != indices.presentFamily) {
        //An image is owned by one queue family at a time and ownership must be explicitly transferred before using
        //it in another queue family. This option offers the best performance.
        swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_info.queueFamilyIndexCount = 2;

        uint family_indices[2] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
        swapchain_info.pQueueFamilyIndices = family_indices;

    }
    else {

        // Images can be used across multiple queue families without explicit ownership transfers.
        swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_info.queueFamilyIndexCount = 0; // Optional
        swapchain_info.pQueueFamilyIndices = nullptr; // Optional
    }

    swapchain_info.preTransform = swapchain_support.capabilities.currentTransform;
    swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    //present mode
    swapchain_info.presentMode = VK_PRESENT_MODE_FIFO_KHR; //or VK_PRESENT_MODE_MAILBOX_KHR

    //If the clipped member is set to VK_TRUE then that means that we don't care about the color of pixels that are obscured,
    //for example because another window is in front of them. 
    swapchain_info.clipped = VK_TRUE;

    //With Vulkan it's possible that your swap chain becomes invalid or unoptimized while your application is running,
    //for example because the window was resized.
    //In that case the swap chain actually needs to be recreated from scratch and a reference to the old one must be specified in this field.

    swapchain_info.oldSwapchain = VK_NULL_HANDLE;

    CHECK_VK(vkCreateSwapchainKHR(device->handle(), &swapchain_info, nullptr, &swapchain));

    //Retrieving the handles of the VkImages
    swapchain_images.resize(image_count);
    vkGetSwapchainImagesKHR(device->handle(), swapchain, &image_count, swapchain_images.data());

    swapchain_views.resize(image_count);

    //Creating all the image views
    for (uint i = 0; i < image_count; i++) {

        VkImageViewCreateInfo view_info;

        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.flags = 0;
        view_info.pNext = nullptr;

        view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;

        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;

        view_info.image = swapchain_images[i];
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;

        view_info.format = surface_format.format;

        CHECK_VK(vkCreateImageView(device->handle(), &view_info, nullptr, &swapchain_views[i]));
    }


}

Swapchain::~Swapchain()
{
    for (auto&& view : swapchain_views) {
        vkDestroyImageView(owning_device->handle(), view, nullptr);
    }

    vkDestroySwapchainKHR(owning_device->handle(), swapchain, nullptr);
}

VkSurfaceFormatKHR Swapchain::select_format(const SwapchainSupportInfo& support_info)
{
    //For the color space we'll use SRGB if it is available, because it results in more accurate perceived colors
    for (const auto& availableFormat : support_info.formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    //If that also fails then we could start ranking the available formats based on how "good" they are,
    //but in most cases it's okay to just settle with the first format that is specified.
    return support_info.formats[0];
}

VkExtent2D Swapchain::select_extent(const SwapchainSupportInfo& support_info, GLFWwindow* window_handle)
{
    if (support_info.capabilities.currentExtent.width != numeric_limits<uint32_t>::max()) {
        return support_info.capabilities.currentExtent;
    }
    else {
        int width, height;
        glfwGetFramebufferSize(window_handle, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(
            actualExtent.width,
            support_info.capabilities.minImageExtent.width,
            support_info.capabilities.maxImageExtent.width);

        actualExtent.height = std::clamp(
            actualExtent.height,
            support_info.capabilities.minImageExtent.height,
            support_info.capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
