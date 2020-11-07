require vulkan

[export]
def main
    assert(VkResult VK_SUCCESS == VkResult VK_SUCCESS)
    assert(VkResult VK_SUCCESS != VkResult VK_NOT_READY)
    return true
