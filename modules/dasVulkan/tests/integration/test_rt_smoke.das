options gen2
options _comment_hygiene = true
options indenting = 4

// Hardware ray-tracing smoke: BLAS + TLAS build, reflection-driven layouts, a 3-stage RT pipeline
// with its shader binding table, and one vkCmdTraceRaysKHR into a storage image — every RT rail in
// one exact-pixel gate. The scene is a single triangle covering exactly the x >= 0 half of the ray
// square at z = 1: left-half rays miss (blue), right-half rays hit (red, with the TLAS instance's
// custom index in green — proving the hand-packed VkAccelerationStructureInstanceKHR bitfields
// reached the driver intact). Soft-skips where the device has no RT (CI lavapipe).

require dastest/testing_boost public
require scene_helpers
require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public

let RT_W = 64
let RT_H = 64
let RT_CUSTOM_INDEX = 51u   // survives exactly in UNORM8: green byte == 51

// ===== shaders =====

var @set = 0 @binding = 0 smoke_tlas : accelerationStructureEXT
var @set = 0 @binding = 1 smoke_img : image2D
var @ray_payload smoke_payload : float3

[vulkan_raygen_shader(name="rt_smoke_rgen_spv")]
def rt_smoke_rgen {
    let gid = gl_LaunchIDEXT
    let dim = gl_LaunchSizeEXT
    // pixel-center ray over [-1,1]^2, orthographic straight down +z; all coordinates dyadic, so
    // no ray ever lands on the triangle's x=0 edge
    let x = (float(gid.x) + 0.5) / float(dim.x) * 2.0 - 1.0
    let y = (float(gid.y) + 0.5) / float(dim.y) * 2.0 - 1.0
    smoke_payload = float3(0.0, 0.0, 0.0)
    traceRayEXT(smoke_tlas, 0x01u, 0xFFu, 0u, 0u, 0u,
        float3(x, y, -1.0), 0.001, float3(0.0, 0.0, 1.0), 100.0, smoke_payload)
    imageStore(smoke_img, int2(int(gid.x), int(gid.y)), float4(smoke_payload, 1.0))
}

var @incoming_ray_payload smoke_miss_prd : float3

[vulkan_miss_shader(name="rt_smoke_miss_spv")]
def rt_smoke_miss {
    smoke_miss_prd = float3(0.0, 0.0, 1.0)
}

var @incoming_ray_payload smoke_hit_prd : float3

[vulkan_closest_hit_shader(name="rt_smoke_chit_spv")]
def rt_smoke_chit {
    smoke_hit_prd = float3(1.0, float(gl_InstanceCustomIndexEXT) * (1.0 / 255.0), 0.0)
}

// ===== gate =====

def rt_available : bool {
    if (volkInitialize() != 0) {
        return false
    }
    var inscope instance <- create_instance("dasVulkan rt_smoke probe", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    var count = 0u
    vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, null)
    if (count == 0u) {
        return false
    }
    let phys = select_physical_device(instance)
    return rt_supported(phys)
}

[test]
def test_rt_smoke(t : T?) {   // nolint:STYLE038 - flat test cell
    if (!rt_available()) {
        feint("VK_KHR_ray_tracing_pipeline not supported by this device; skipping (CI lavapipe has no RT)\n")
        return
    }
    var inscope instance <- create_instance("dasVulkan rt_smoke", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device_ray_tracing(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    // BLAS: one triangle whose only edge crossing the ray square is the line x = 0 — it covers
    // exactly the x >= 0 half of [-1,1]^2 at z = 1
    var verts <- [float3(0.0, -8.0, 1.0), float3(0.0, 8.0, 1.0), float3(8.0, 0.0, 1.0)]
    var idx <- [0u, 1u, 2u]
    var inscope blas <- build_blas(device, phys, pool, queue, verts, idx)
    delete verts
    delete idx
    t |> success(blas.address != 0ul, "BLAS has a device address")

    var insts <- [make_accel_instance(blas.address, RT_CUSTOM_INDEX)]
    var inscope tlas <- build_tlas(device, phys, pool, queue, insts)
    delete insts
    t |> success(tlas.address != 0ul, "TLAS has a device address")

    // layouts straight from shader reflection: set 0 = { binding 0 TLAS, binding 1 storage image }
    var reflections <- [decode_reflection(rt_smoke_rgen_spv_reflect),
        decode_reflection(rt_smoke_miss_spv_reflect),
        decode_reflection(rt_smoke_chit_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)
    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)

    var dpci = DescriptorPoolCreateInfo(maxSets = 1u,
        pPoolSizes <- [
            DescriptorPoolSize(type_ = VkDescriptorType.ACCELERATION_STRUCTURE_KHR, descriptorCount = 1u),
            DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
        ])
    var inscope desc_pool <- create_descriptor_pool(device, dpci)
    var dsai : DescriptorSetAllocateInfo
    dsai.descriptorPool = weak_copy(desc_pool)
    dsai.pSetLayouts |> push(weak_copy(set_layouts[0]))
    var inscope dsets <- allocate_descriptor_sets(device, dsai)
    t |> success(length(dsets) == 1, "one descriptor set allocated")

    // storage image (STORAGE for the raygen write + TRANSFER_SRC for readback), device-local + view
    let fmt = VkFormat.R8G8B8A8_UNORM
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt, mipLevels = 1u, arrayLayers = 1u,
        tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(RT_W)
    ici.extent.height = uint(RT_H)
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.storage = true
    ici.usage.transfer_src = true
    var inscope image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope imem <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(image), boost_value_to_vk(imem), 0ul), null)
    var vci = ImageViewCreateInfo(image = weak_copy(image), viewType = VkImageViewType._2D, format = fmt)
    vci.subresourceRange.aspectMask.color = true
    vci.subresourceRange.levelCount = 1u
    vci.subresourceRange.layerCount = 1u
    var inscope view <- create_image_view(device, vci)

    // descriptor writes: the TLAS through the pNext rail, the image through the boost path
    write_descriptor_acceleration_structure(device, dsets[0], 0u, tlas)
    var write = WriteDescriptorSet(dstSet = weak_copy(dsets[0]), dstBinding = 1u,
        descriptorType = VkDescriptorType.STORAGE_IMAGE, descriptorCount = 1u)
    write.pImageInfo |> push(DescriptorImageInfo(imageView = weak_copy(view), imageLayout = VkImageLayout.GENERAL))
    var writes : array<WriteDescriptorSet>
    writes |> emplace(write)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope rgen <- create_shader_module(device, rt_smoke_rgen_spv)
    var inscope miss <- create_shader_module(device, rt_smoke_miss_spv)
    var inscope chit <- create_shader_module(device, rt_smoke_chit_spv)
    var inscope pipeline <- create_ray_tracing_pipeline(device, pipe_layout, rgen, miss, chit)
    var inscope sbt <- build_shader_binding_table(device, phys, pipeline, 1u, 1u)

    var inscope readback <- create_host_buffer(device, phys, uint64(RT_W * RT_H * 4))
    run_cmd_sync(device, pool, queue) $(cmd) {
        let none : VkAccessFlags
        var shader_write : VkAccessFlags
        shader_write.shader_write = true
        var to_read : VkAccessFlags
        to_read.transfer_read = true
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var rt_stage : VkPipelineStageFlags
        rt_stage.ray_tracing_shader_khr = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        transition_image(cmd, image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL,
            none, shader_write, top, rt_stage)
        cmd_bind_pipeline(cmd, pipeline, VkPipelineBindPoint.RAY_TRACING_KHR)
        var sets : array<DescriptorSet>
        sets |> push(weak_copy(dsets[0]))
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.RAY_TRACING_KHR, pipe_layout, 0u, sets, no_dyn)
        cmd_trace_rays(cmd, sbt, uint(RT_W), uint(RT_H))
        transition_image(cmd, image, VkImageLayout.GENERAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            shader_write, to_read, rt_stage, xfer)
        copy_image_to_buffer(cmd, image, readback, RT_W, RT_H)
    }
    var pixels : array<uint8>
    map_memory_to_array(device, readback.memory, uint64(RT_W * RT_H * 4)) $(m) {
        pixels := m
    }
    assert_pixels_exact(t, pixels, RT_W, RT_H, "rt_smoke") $(x, _y) {
        return x < RT_W / 2 ? int4(0, 0, 255, 255) : int4(255, int(RT_CUSTOM_INDEX), 0, 255)
    }
    delete pixels
    delete reflections
}
