options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost

[test]
def test_memory_types(t : T?) {
    t |> success(volkInitialize() == 0, "volkInitialize succeeds")
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    var count = 0u
    vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, null)
    if (count == 0u) {
        t |> skip("no physical device")
        return
    }
    let phys = select_physical_device(instance)
    var mem : VkPhysicalDeviceMemoryProperties
    vkGetPhysicalDeviceMemoryProperties(phys, mem)
    let all_types = mem.memoryTypeCount >= 32u ? 0xFFFFFFFFu : (1u << mem.memoryTypeCount) - 1u
    let none : VkMemoryPropertyFlags
    // the non-panicking form answers -1 where no type fits, and the fitting type otherwise
    t |> equal(-1, try_find_memory_type(phys, 0u, none), "an empty type mask fits no memory type")
    let any = try_find_memory_type(phys, all_types, none)
    t |> success(any >= 0 && any < int(mem.memoryTypeCount), "the full mask with no property wanted fits the first type ({any})")
    t |> equal(uint(any), find_memory_type(phys, all_types, none), "the panicking form picks the same type")
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    let visible = try_find_memory_type(phys, all_types, want)
    t |> success(visible >= 0, "every device carries a host-visible type")
    if (visible >= 0) {
        t |> success(mem.memoryTypes[visible].propertyFlags.host_visible, "the picked type carries the wanted property")
    }
}
