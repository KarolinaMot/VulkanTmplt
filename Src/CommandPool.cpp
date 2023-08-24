#include "CommandPool.h"

CommandPool::CommandPool(shared_ptr<VulkanDevice> device, uint allocated_buffers)
{
    owning_device = device;

    QueueFamilyIndices queueFamilyIndices = device->get_queue_family_indices();
    VkCommandPoolCreateInfo poolInfo{};

    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.pNext = nullptr;

    //VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT: Allow command buffers to be rerecorded individually,
    //without this flag they all have to be reset together
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    CHECK_VK(vkCreateCommandPool(device->handle(), &poolInfo, nullptr, &command_pool));

    command_buffers.resize(allocated_buffers);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;

    allocInfo.commandPool = command_pool;

    //The level parameter specifies if the allocated command buffers are primary or secondary command buffers.
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = command_buffers.size();

    CHECK_VK(vkAllocateCommandBuffers(device->handle(), &allocInfo, command_buffers.data()));
}

CommandPool::~CommandPool()
{
    vkFreeCommandBuffers(owning_device->handle(), command_pool, command_buffers.size(), command_buffers.data());
    vkDestroyCommandPool(owning_device->handle(), command_pool, nullptr);
}

VkCommandBuffer CommandPool::allocate_one_time_buffer()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;

    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(owning_device->handle(), &allocInfo, &commandBuffer);
    return commandBuffer;
}

void CommandPool::free_one_time_buffer(VkCommandBuffer to_free)
{
    vkFreeCommandBuffers(owning_device->handle(), command_pool, 1, &to_free);
}
