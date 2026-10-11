options gen2
options _comment_hygiene = true
options indenting = 4

// The host side of dynamic rendering: both halves of the device gate (instance version, device
// features) and the 1.4 and shader-object gates layered on it, the require_dynamic_rendering record a
// creator leaves behind, a feature struct handed back to its caller with the caller's own pNext after
// the 1.3 pair rode behind it, and the pipeline-format chain keeping what it is put in front of. No
// pixels - the tutorials prove the frames.

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost

def require_dynamic_rendering_panics() : bool {
    try {
        require_dynamic_rendering("gate test")
    } recover {
        return true
    }
    return false
}

[test]
def test_dynamic_rendering_gate(t : T?) {
    t |> success(volkInitialize() == 0, "volkInitialize succeeds")
    {
        var inscope instance <- create_instance("dasVulkan gate test 1.1", make_api_version(1u, 1u, 0u))
        volkLoadInstance(boost_value_to_vk(instance))
        let phys = select_physical_device(instance)
        t |> success(!dynamic_rendering_supported(phys), "a 1.1 instance never reports the pair")
        t |> success(!vulkan14_supported(phys), "a 1.1 instance never reports Vulkan 1.4")
        t |> success(!shader_object_supported(phys), "a 1.1 instance never reports shader objects, which draw inside a dynamic rendering")
        var inscope device <- create_device(phys, select_graphics_queue_family(phys))
        t |> success(require_dynamic_rendering_panics(), "require_dynamic_rendering panics after a plain creator ran without the pair")
    }
    var inscope instance <- create_instance("dasVulkan gate test 1.3", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    var props : VkPhysicalDeviceProperties
    vkGetPhysicalDeviceProperties(phys, props)
    let device_13 = props.apiVersion >= make_api_version(1u, 3u, 0u)
    t |> success(dynamic_rendering_supported(phys) == device_13, "a 1.3 instance reports the pair exactly when the device is 1.3")
    var q13 = VkPhysicalDeviceVulkan13Features()
    var q2 = VkPhysicalDeviceFeatures2()
    q2.pNext = unsafe(addr<void?>(q13))
    if (device_13) {
        vkGetPhysicalDeviceFeatures2(phys, q2)
    }
    t |> success(dynamic_rendering_supported(phys) == (device_13 && q13.dynamicRendering != 0u && q13.synchronization2 != 0u),
        "dynamic_rendering_supported matches an independent feature query")

    let gfx = select_graphics_queue_family(phys)
    let no_ext : array<string>
    var f12 : VkPhysicalDeviceVulkan12Features
    {
        var inscope device <- create_device(phys, gfx, no_ext, f12)
        t |> success(f12.pNext == null, "the features12 overload hands the struct back with the null pNext it got")
        if (device_13) {
            t |> success(!require_dynamic_rendering_panics(), "require_dynamic_rendering is silent after a creator that got the pair")
        }
    }
    {
        var inscope device <- create_device(phys, gfx, no_ext, f12)
        t |> success(f12.pNext == null, "a reused features12 struct carries no chain from the first creation")
    }
    if (device_13) {
        var f13 : VkPhysicalDeviceVulkan13Features
        f13.sType = VkStructureType.PHYSICAL_DEVICE_VULKAN_1_3_FEATURES
        var chained : VkPhysicalDeviceVulkan12Features
        chained.pNext = unsafe(addr<void?>(f13))
        var inscope device <- create_device(phys, gfx, no_ext, chained)
        t |> success(chained.pNext == unsafe(addr<void?>(f13)), "a caller's own chain comes back untouched")
    }
}

[test]
def test_rendering_formats_chain(t : T?) {
    var tail : VkPipelineRenderingCreateInfo
    var gp = VkGraphicsPipelineCreateInfo()
    gp.pNext = unsafe(addr<void?>(tail))
    var fmts = rendering_formats(VkFormat.B8G8R8A8_UNORM, VkFormat.D32_SFLOAT)
    attach_rendering_formats(gp, fmts)
    t |> success(gp.pNext == unsafe(addr<void?>(fmts.info)), "the formats struct heads the chain")
    t |> success(fmts.info.pNext == unsafe(addr<void?>(tail)), "what the chain held follows it")
    t |> success(fmts.info.colorAttachmentCount == 1u, "one color format")
    t |> success(fmts.info.depthAttachmentFormat == VkFormat.D32_SFLOAT, "the depth format rides along")

    var depth_only = rendering_formats(VkFormat.UNDEFINED, VkFormat.D32_SFLOAT)
    var gp_depth = VkGraphicsPipelineCreateInfo()
    attach_rendering_formats(gp_depth, depth_only)
    t |> success(depth_only.info.colorAttachmentCount == 0u, "a depth-only pipeline names no color format")
    t |> success(depth_only.info.pNext == null, "an empty chain stays empty")
}
