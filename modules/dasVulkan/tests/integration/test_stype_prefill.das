options gen2
options _comment_hygiene = true
options indenting = 4

// #57: generated get_*/enumerate wrappers construct their out-structs through the
// sType-prefilling ctors. Each result must carry the correct sType (a zero sType
// means the prefill regressed and the driver was handed an invalid struct) AND
// real data (proving the driver actually filled it).

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost

[test]
def test_stype_prefill(t : T?) {
    if (volkInitialize() != 0) {
        t |> failure("no Vulkan loader")
        return
    }
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = vk_value_to_boost(select_physical_device(instance))

    let props2 = get_physical_device_properties2(phys)
    t |> success(props2.sType == VkStructureType.PHYSICAL_DEVICE_PROPERTIES_2, "properties2 sType prefilled")
    t |> success(props2.properties.apiVersion != 0u, "properties2 carries real data (apiVersion)")

    let feats2 = get_physical_device_features2(phys)
    t |> success(feats2.sType == VkStructureType.PHYSICAL_DEVICE_FEATURES_2, "features2 sType prefilled")

    let mem2 = get_physical_device_memory_properties2(phys)
    t |> success(mem2.sType == VkStructureType.PHYSICAL_DEVICE_MEMORY_PROPERTIES_2, "memory_properties2 sType prefilled")
    t |> success(mem2.memoryProperties.memoryHeapCount >= 1u, "memory_properties2 carries real data (heaps)")

    // the array family: every element of the enumerate-style out-list is prefilled,
    // and the list length matches the v1 API's count
    var v1count = 0u
    vkGetPhysicalDeviceQueueFamilyProperties(boost_value_to_vk(phys), v1count, null)
    var qf2 <- get_physical_device_queue_family_properties2(phys)
    t |> equal(length(qf2), int(v1count))
    var bad_stype = 0
    var have_queue = false
    for (qf in qf2) {
        if (qf.sType != VkStructureType.QUEUE_FAMILY_PROPERTIES_2) {
            bad_stype++
        }
        if (qf.queueFamilyProperties.queueCount >= 1u) {
            have_queue = true
        }
    }
    t |> success(bad_stype == 0, "every queue_family_properties2 element has its sType prefilled")
    t |> success(have_queue, "queue_family_properties2 carries real data (a family with queues)")
    delete qf2
}
