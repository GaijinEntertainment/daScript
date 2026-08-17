options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost

[test]
def test_enumerate(t : T?) {
    t |> success(volkInitialize() == 0, "volkInitialize succeeds")
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))

    var count = 0u
    vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, null)
    t |> success(count >= 1u, "at least one physical device")
    if (count == 0u) {
        return
    }
    let phys = select_physical_device(instance)

    var qcount = 0u
    vkGetPhysicalDeviceQueueFamilyProperties(phys, qcount, null)
    t |> success(qcount >= 1u, "at least one queue family")

    let gfx = select_graphics_queue_family(phys)
    t |> success(gfx >= 0u, "a graphics queue family is found")

    var mem : VkPhysicalDeviceMemoryProperties
    vkGetPhysicalDeviceMemoryProperties(phys, mem)
    t |> success(mem.memoryHeapCount >= 1u, "at least one memory heap")
    t |> success(mem.memoryTypeCount >= 1u, "at least one memory type")
}
