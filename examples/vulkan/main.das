require vulkan

[export]
def main
    // enums
    assert(VkResult VK_SUCCESS == VkResult VK_SUCCESS)
    assert(VkResult VK_SUCCESS != VkResult VK_NOT_READY)

    // structs
    let offset2d = [[VkOffset2D x=1]]
    let offset3d = [[VkOffset3D x=2]]
    assert(offset2d.x * 2 == offset3d.x)

    return true
