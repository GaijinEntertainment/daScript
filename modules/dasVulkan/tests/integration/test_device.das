options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost
require vulkan/vulkan_structs

[test(description = "the instance enumerates a physical device with a graphics queue family, memory, and a consistent cooperative-matrix2 ladder")]
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

    // the cm2 capability ladder: each rung implies the one below it, and the top rung implies
    // the extension it names (lavapipe answers false all the way down; an NV box climbs it)
    let cm2 = cooperative_matrix2_supported(phys)
    let fa = cooperative_matrix2_fa_supported(phys)
    let dv = cooperative_matrix2_decode_vector_supported(phys)
    t |> success(!fa || cm2, "the fa trio implies coopmat2")
    t |> success(!dv || cm2, "the four-wide decode implies coopmat2")
    t |> success(!dv || device_extension_available(phys, "VK_NV_cooperative_matrix_decode_vector"),
        "the four-wide decode implies its extension")
}

[test(description = "try_find_memory_type answers -1 for an empty mask and find_memory_type agrees with it for a fitting type")]
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

[test(description = "the OS video-memory query keeps usage within budget and reads zeros for a device with no LUID")]
def test_os_video_memory(t : T?) {
    if (volkInitialize() != 0) {
        t |> failure("no Vulkan loader")
        return
    }
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    var idp = VkPhysicalDeviceIDProperties()
    var p2 = VkPhysicalDeviceProperties2()
    p2.pNext = unsafe(addr<void?>(idp))
    vkGetPhysicalDeviceProperties2(phys, p2)
    let os = os_video_memory(phys)
    let answered = os.budget > 0ul
    if (idp.deviceLUIDValid == 0u) {
        t |> success(!answered && os.usage == 0ul && os.adapter_dedicated == 0ul && os.adapter_shared == 0ul, "a device with no LUID reads 0, 0, 0, 0 (no answer)")
        return
    }
    t |> success(os.usage <= os.budget, "the process's usage never passes its ceiling ({int64(os.usage)} <= {int64(os.budget)})")
    t |> success(!answered || os.adapter_dedicated >= os.usage, "the adapter's usage is at least the process's ({int64(os.adapter_dedicated)} >= {int64(os.usage)})")
    to_log(LOG_INFO, "os video memory: budget {int64(os.budget) / 1_000_000l} MB, usage {int64(os.usage) / 1_000_000l} MB, adapter {int64(os.adapter_dedicated) / 1_000_000l} MB dedicated + {int64(os.adapter_shared) / 1_000_000l} MB system{answered ? "" : " (no OS answer on this platform)"}\n")
}

[test(description = "a debug-utils messenger is created with the default vk_debug_callback and destroyed")]
def test_debug_messenger(t : T?) {
    if (volkInitialize() != 0) {
        t |> failure("no Vulkan loader")
        return
    }
    if (!instance_extension_available("VK_EXT_debug_utils")) {
        t |> success(true, "skipped: VK_EXT_debug_utils not available")
        return
    }
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u),
        ["VK_EXT_debug_utils"])
    volkLoadInstance(boost_value_to_vk(instance))

    var ci = VkDebugUtilsMessengerCreateInfoEXT()
    ci.messageSeverity.warning_ext = true
    ci.messageSeverity.error_ext = true
    ci.messageType.general_ext = true
    ci.messageType.validation_ext = true
    ci.messageType.performance_ext = true
    ci.pfnUserCallback = vk_debug_callback()
    t |> success(ci.pfnUserCallback != null, "vk_debug_callback yields a non-null C callback")

    var messenger : VkDebugUtilsMessengerEXT
    let rc = vkCreateDebugUtilsMessengerEXT(boost_value_to_vk(instance), ci, null, messenger)
    t |> success(rc == VkResult.SUCCESS, "debug-utils messenger created with the default callback")
    if (rc == VkResult.SUCCESS) {
        vkDestroyDebugUtilsMessengerEXT(boost_value_to_vk(instance), messenger, null)
    }
}

[test(description = "an owning image handle finalizes once while its weak_copy stays non-owning")]
def test_handles(t : T?) {
    t |> success(volkInitialize() == 0, "volkInitialize succeeds")
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))

    // create an owning image; weak_copy must be non-owning and must not disturb the owner
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = VkFormat.R8G8B8A8_UNORM,
        mipLevels = 1u, arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = 32u
    ici.extent.height = 32u
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.color_attachment = true
    {
        var inscope img <- create_image(device, ici)
        t |> success(img._needs_delete, "created image owns its handle")
        t |> success(img._vk != 0ul, "created image has a valid handle")
        let w = weak_copy(img)
        t |> success(!w._needs_delete, "weak_copy is non-owning")
        t |> success(img._needs_delete, "weak_copy left the owner owning")
        t |> success(boost_value_to_vk(w) == boost_value_to_vk(img), "weak_copy refers to the same handle")
        // img finalizes here exactly once (vkDestroyImage); w does not
    }
    t |> success(true, "owning image created and finalized without crash")
}

[test(description = "the sType-prefilling constructors carry the right sType and the driver fills real data")]
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

//! canary: adding re-view support flips this test, so update it
//! deliberately
[test(description = "a vk view consumes the opt-ptr field so a second view of the same struct sees it as absent")]
def test_opt_ptr_view_semantics(t : T?) {
    var vp : PipelineViewportStateCreateInfo
    var vport = Viewport(width = 1.0f, height = 1.0f, maxDepth = 1.0f)
    vp.pViewports |> push(vport)

    var gp : GraphicsPipelineCreateInfo
    var st : PipelineShaderStageCreateInfo      // null module -- never handed to a driver
    st.stage.vertex = true
    gp.pStages <- [st]
    gp.pViewportState = unsafe(addr(vp))
    t |> success(gp.pViewportState != null, "opt-ptr field set before viewing")

    let vk1 = vk_view_create_unsafe(gp)
    t |> equal(int(vk1.stageCount), 1)          // stageCount = 0 falls back to length(pStages)
    t |> success(vk1.pViewportState != null, "the view carries the cloned viewport state")
    t |> success(gp.pViewportState == null, "vk_view_create_unsafe consumes the opt-ptr field")
    t |> success(gp._vk_view_pViewportState_present, "scratch clone is marked present")
    vk_view_destroy(gp)
    t |> success(!gp._vk_view_pViewportState_present, "vk_view_destroy releases the scratch clone")

    // the canary proper: a second view of the same struct no longer sees the state
    let vk2 = vk_view_create_unsafe(gp)
    t |> success(vk2.pViewportState == null, "a re-view sees the consumed field as absent")
    vk_view_destroy(gp)

    // count-from-length on the batch allocate-info shape
    let dsl_a, dsl_b : DescriptorSetLayout
    var dsai = DescriptorSetAllocateInfo(pSetLayouts <- [dsl_a, dsl_b])
    let vdsai = vk_view_create_unsafe(dsai)
    t |> equal(int(vdsai.descriptorSetCount), 2)
    vk_view_destroy(dsai)

    // fixed-array member copy: blendConstants float[4] must land in the raw struct element-exact and in
    // order. Host-side and driver-independent, so it must pass on EVERY CI lane -- the marshal half of
    // test_pipeline_boost_structs' CONSTANT_COLOR oracle, whose GPU half llvmpipe fails driver-side.
    var cb : PipelineColorBlendStateCreateInfo
    cb.blendConstants = fixed_array(0.25f, 0.5f, 0.75f, 1.0f)
    let vcb = vk_view_create_unsafe(cb)
    t |> equal(vcb.blendConstants[0], 0.25f)
    t |> equal(vcb.blendConstants[1], 0.5f)
    t |> equal(vcb.blendConstants[2], 0.75f)
    t |> equal(vcb.blendConstants[3], 1.0f)
    vk_view_destroy(cb)
}
