options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require _scene_helpers
require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require vulkan/vulkan_commands public
require spirv/spirv_shader public
require spirv/spirv_reflect public
require vulkan/vulkan_gltf
require daslib/math_boost
require math

// Hardware ray-tracing smoke: BLAS + TLAS build, reflection-driven layouts, a 3-stage RT pipeline
// with its shader binding table, and one vkCmdTraceRaysKHR into a storage image — every RT rail in
// one exact-pixel gate. The scene is a single triangle covering exactly the x >= 0 half of the ray
// square at z = 1: left-half rays miss (blue), right-half rays hit (red, with the TLAS instance's
// custom index in green — proving the hand-packed VkAccelerationStructureInstanceKHR bitfields
// reached the driver intact). Soft-skips where the device has no RT (CI lavapipe).


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

[test(description = "a BLAS, a TLAS and one ray-tracing pipeline trace into a storage image with left-half rays missing and right-half rays hitting", timeout = 1200)]
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

[test(description = "the fixture triangle renders red, blue and green corners with a blended centroid", timeout = 1200)]
def test_triangle(t : T?) {
    var pixels <- render_triangle()
    let top = px(pixels, 128, 80)
    let bl = px(pixels, 75, 190)
    let br = px(pixels, 180, 190)
    let mid = px(pixels, 128, 150)
    t |> success(top.x > top.y + 40 && top.x > top.z + 40, "top vertex region is red")
    t |> success(bl.z > bl.x + 40 && bl.z > bl.y + 40, "bottom-left vertex region is blue")
    t |> success(br.y > br.x + 40 && br.y > br.z + 40, "bottom-right vertex region is green")
    t |> success(mid.x > 20 && mid.y > 20 && mid.z > 20, "centroid shows the interpolated blend")
    delete pixels
}

[test(description = "a reflection-built textured quad samples the four texels in the right quadrants with the push-constant tint applied", timeout = 1200)]
def test_phase4_textured_quad(t : T?) {
    var pixels <- render_textured_quad()
    let outside = px(pixels, 70, 128)   // left of the right-shifted quad -> clear color
    let red_q   = px(pixels, 140, 90)   // u<0.5, v<0.5 -> texel (0,0) red
    let green_q = px(pixels, 230, 90)   // u>0.5, v<0.5 -> texel (1,0) green (also: quad reaches right)
    let blue_q  = px(pixels, 140, 170)  // u<0.5, v>0.5 -> texel (0,1) blue, tinted black
    let white_q = px(pixels, 230, 170)  // u>0.5, v>0.5 -> texel (1,1) white, tinted yellow

    // MVP translate (column 3) applied: the quad's left edge moved right, so (70,128) is clear.
    t |> success(outside.z > outside.x && outside.x < 80 && outside.y < 80 && outside.z < 90,
        "outside the right-shifted quad is the clear color (MVP translate applied)")
    // sampler reads four distinct texels at four screen positions (UV mapping correct).
    t |> success(red_q.x > 150 && red_q.y < 80 && red_q.z < 80, "top-left quadrant samples the red texel")
    t |> success(green_q.y > 150 && green_q.x < 80 && green_q.z < 80, "top-right quadrant samples the green texel")
    t |> success(blue_q.x < 60 && blue_q.y < 60 && blue_q.z < 60, "bottom-left (blue) texel tinted to black (blue zeroed)")
    // push constant delivered: the white texel loses its blue channel -> yellow.
    t |> success(white_q.x > 150 && white_q.y > 150 && white_q.z < 80, "bottom-right (white) texel tinted yellow (push-constant tint applied)")
    delete pixels
}

def require_dynamic_rendering_panics() : bool {
    var panicked = false
    try {
        require_dynamic_rendering("gate test")
    } recover {
        panicked = true
    }
    return panicked
}

[test(description = "dynamic rendering support follows the instance and device version and require_dynamic_rendering panics only without the pair")]
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

[test(description = "rendering_formats heads the pNext chain, keeps what the chain held and names no color format for a depth-only pipeline")]
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

var @flat @out @location = 0 di_draw : int

[vertex_shader(name="draw_id_vert_spv"), marker(no_coverage)]
def draw_id_vert {
    let did = gl_DrawID
    let xmin = float(did) * 0.5f - 1.0f
    let xmax = xmin + 0.5f
    let i = gl_VertexIndex
    var xy = float2(xmin, -1.0f)
    if (i == 1) { xy = float2(xmax, -1.0f) }
    if (i == 2) { xy = float2(xmax,  1.0f) }
    if (i == 3) { xy = float2(xmin, -1.0f) }
    if (i == 4) { xy = float2(xmax,  1.0f) }
    if (i == 5) { xy = float2(xmin,  1.0f) }
    di_draw = did
    gl_Position = float4(xy, 0.0f, 1.0f)
}

var @in @flat @location = 0 fi_draw : int
var @out @location = 0 di_color : float4

[fragment_shader(name="draw_id_frag_spv"), marker(no_coverage)]
def draw_id_frag {
    var c = float4(0.0f, 0.0f, 0.0f, 1.0f)
    if (fi_draw == 0) { c = float4(1.0f, 0.0f, 0.0f, 1.0f) }
    if (fi_draw == 1) { c = float4(0.0f, 1.0f, 0.0f, 1.0f) }
    if (fi_draw == 2) { c = float4(0.0f, 0.0f, 1.0f, 1.0f) }
    if (fi_draw == 3) { c = float4(1.0f, 1.0f, 0.0f, 1.0f) }
    di_color = c
}

// ===== Host render =====
let TEX_W = 256
let TEX_H = 256
let N_DRAW = 4

def private render_draw_id() : array<uint8> {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var pixels : array<uint8>
    var inscope instance <- create_instance("dasVulkan test gl_DrawID", make_api_version(1u, 1u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    // multiDrawIndirect (base features) + shaderDrawParameters (1.1 features).
    var inscope device <- create_device_draw_parameters(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)
    let fmt = VkFormat.R8G8B8A8_UNORM

    var inscope target <- build_offscreen_target(device, phys, TEX_W, TEX_H, fmt)
    var inscope render_pass <- create_render_pass_single_color(device, fmt)
    var fbci = FramebufferCreateInfo(renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(target.view)],
        width = uint(TEX_W), height = uint(TEX_H), layers = 1u)
    var inscope framebuffer <- create_framebuffer(device, fbci)

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope cmd_pool <- create_command_pool(device, poolci)

    // Indirect draw buffer: N_DRAW VkDrawIndirectCommand entries (16 bytes each).
    let stride = 16u
    let buf_bytes = uint64(stride) * uint64(N_DRAW)
    var indirect_usage : VkBufferUsageFlags
    indirect_usage.indirect_buffer = true
    var inscope indirect_buf <- create_host_buffer(device, phys, buf_bytes, indirect_usage)
    var indirect_bytes : array<uint8>
    indirect_bytes |> resize(int64(buf_bytes))
    for (k in range(N_DRAW)) {
        // every draw: 6 vertices, 1 instance; the shader keys off gl_DrawID + gl_VertexIndex, so first{Vertex,Instance} stay 0.
        write_u32_at(indirect_bytes, k * int(stride) + 0,  6u)    // vertexCount
        write_u32_at(indirect_bytes, k * int(stride) + 4,  1u)    // instanceCount
        write_u32_at(indirect_bytes, k * int(stride) + 8,  0u)    // firstVertex
        write_u32_at(indirect_bytes, k * int(stride) + 12, 0u)    // firstInstance
    }
    upload_bytes(indirect_buf, device, indirect_bytes)
    delete indirect_bytes

    var reflections <- [decode_reflection(draw_id_vert_spv_reflect), decode_reflection(draw_id_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)
    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope vert <- create_shader_module(device, draw_id_vert_spv)
    var inscope frag <- create_shader_module(device, draw_id_frag_spv)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, pipe_layout, vert, frag, TEX_W, TEX_H)

    let buf_size = uint64(TEX_W * TEX_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    run_cmd_sync(device, cmd_pool, queue) $(cmd) {
        record_render_pass(cmd, render_pass, framebuffer, full_area(TEX_W, TEX_H), clear_color(0.0f, 0.0f, 0.0f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            cmd_draw_indirect(cmd, indirect_buf.buffer, 0ul, uint(N_DRAW), stride)
        }
        copy_image_to_buffer(cmd, target.image, readback, TEX_W, TEX_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

def private write_u32_at(var bytes : array<uint8>; off : int; v : uint) {
    var probe = fixed_array(v)
    unsafe {
        let src = addr<uint8 const?>(probe[0])
        for (b in range(4)) {
            bytes[off + b] = src[b]
        }
    }
}

//! needs multiDrawIndirect and shaderDrawParameters, both enabled
//! by create_device_draw_parameters
[test(description = "gl_DrawID selects a distinct color for each of four indirect draws", timeout = 1200)]
def test_gl_draw_id_indirect(t : T?) {
    var pixels <- render_draw_id()
    let c0 = px(pixels, 32, 128)
    let c1 = px(pixels, 96, 128)
    let c2 = px(pixels, 160, 128)
    let c3 = px(pixels, 224, 128)
    t |> success(c0.x > 200 && c0.y < 60 && c0.z < 60,
        "strip 0 (gl_DrawID=0) is red (got R={c0.x} G={c0.y} B={c0.z})")
    t |> success(c1.y > 200 && c1.x < 60 && c1.z < 60,
        "strip 1 (gl_DrawID=1) is green (got R={c1.x} G={c1.y} B={c1.z})")
    t |> success(c2.z > 200 && c2.x < 60 && c2.y < 60,
        "strip 2 (gl_DrawID=2) is blue (got R={c2.x} G={c2.y} B={c2.z})")
    t |> success(c3.x > 200 && c3.y > 200 && c3.z < 60,
        "strip 3 (gl_DrawID=3) is yellow (got R={c3.x} G={c3.y} B={c3.z})")
    delete pixels
}

// vulkan_gltf integration: load glTF fixtures from the daslang tree (tests/dasGLTF/models),
// upload + render offscreen, and property-assert the pixels — Box exercises the rigid pipeline
// (push constants + red PBR material), SimpleSkin the skinned pipeline (joint-palette UBO).


let GLTF_TEST_W = 256
let GLTF_TEST_H = 256
let GLTF_COLOR_FMT = VkFormat.R8G8B8A8_UNORM   // the shader sRGB-encodes; an _SRGB target would double-encode
let GLTF_DEPTH_FMT = VkFormat.D32_SFLOAT

def private model_path(name : string) : string => "{get_das_root()}/tests/dasGLTF/models/{name}"

// perspective_rh_0_to_1 gives Vulkan's z in [0,1]; Vulkan NDC additionally has +Y down, so flip Y
def private perspective_vk_01(fovy, aspect, znear, zfar : float) : float4x4 {
    var m = perspective_rh_0_to_1(fovy, aspect, znear, zfar)
    m[1] = float4(m[1].x, -m[1].y, m[1].z, m[1].w)
    return m
}

def private gltf_px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * GLTF_TEST_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}

// render `scene` offscreen with an auto-framed camera; self-contained context per call
def private render_gltf_scene(var scene : GltfScene) : array<uint8> {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan gltf test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)

    var inscope color <- build_offscreen_target(device, phys, GLTF_TEST_W, GLTF_TEST_H, GLTF_COLOR_FMT)
    var inscope depth <- build_offscreen_depth(device, phys, GLTF_TEST_W, GLTF_TEST_H, GLTF_DEPTH_FMT)
    var inscope render_pass <- create_render_pass_color_depth(device, GLTF_COLOR_FMT, GLTF_DEPTH_FMT)
    var inscope framebuffer <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(color.view), weak_copy(depth.view)],
        width = uint(GLTF_TEST_W),
        height = uint(GLTF_TEST_H),
        layers = 1u))
    let buf_size = uint64(GLTF_TEST_W * GLTF_TEST_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    var inscope model <- gltf_vk_upload(device, phys, queue, pool, render_pass, scene)

    // frame the scene's world AABB from a 3/4 view
    let (bmin, bmax) = gltf_scene_bounds(scene)
    let center = (bmin + bmax) * 0.5
    let radius = max(length(bmax - bmin) * 0.5, 0.001)
    let eye = center + normalize(float3(0.6, 0.4, 1.0)) * (radius * 2.6)
    let view = look_at_rh(eye, center, float3(0.0, 1.0, 0.0))
    let proj = perspective_vk_01(45.0 * PI / 180.0, float(GLTF_TEST_W) / float(GLTF_TEST_H), radius * 0.05, radius * 10.0)
    gltf_vk_update(device, model, scene, identity4x4(), view, proj, eye, GltfPbrLight())

    var pixels : array<uint8>
    run_cmd_sync(device, pool, queue) $(cmd) {
        var clears <- [clear_color(0.05f, 0.07f, 0.1f, 1.0f), clear_depth(1.0f)]
        record_render_pass(cmd, render_pass, framebuffer, full_area(GLTF_TEST_W, GLTF_TEST_H), clears) {
            gltf_vk_render(cmd, model, scene, identity4x4(), GLTF_TEST_W, GLTF_TEST_H)
        }
        delete clears
        copy_image_to_buffer(cmd, color.image, readback, GLTF_TEST_W, GLTF_TEST_H)
    }
    map_memory_to_array(device, readback.memory, buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

[test(description = "Box.glb renders through the rigid PBR path as a lit red box over the clear color", timeout = 1200)]
def test_gltf_box(t : T?) {
    t |> run("Box.glb renders through the rigid PBR path") @(t : T?) {
        var scene <- load_gltf(model_path("Box.glb"))
        t |> equal(length(scene.meshes), 1)
        var pixels <- render_gltf_scene(scene)
        t |> equal(length(pixels), GLTF_TEST_W * GLTF_TEST_H * 4)
        // corner = clear color (dark blue-grey), center = the lit red Box material (0.8, 0, 0)
        let corner = gltf_px(pixels, 2, 2)
        t |> success(corner.x < 90 && corner.z >= corner.x, "corner should be the clear color, got {corner}")
        let center = gltf_px(pixels, GLTF_TEST_W / 2, GLTF_TEST_H / 2)
        t |> success(center.x > 70, "center should be lit red, got {center}")
        t |> success(center.x > center.y * 2 && center.x > center.z * 2, "center should be dominantly red, got {center}")
        delete pixels
        delete scene
    }
}

[test(description = "SimpleSkin.gltf renders through the skinned path with a lit center that differs from the clear color", timeout = 1200)]
def test_gltf_simple_skin(t : T?) {
    t |> run("SimpleSkin.gltf renders through the skinned path") @(t : T?) {
        var scene <- load_gltf(model_path("SimpleSkin.gltf"))
        t |> equal(length(scene.skins), 1)
        // pose mid-animation so the strip is bent by the joint palette, then render skinned
        evaluate_animation(scene, 0, 2.75)
        var pixels <- render_gltf_scene(scene)
        // the untextured default-material strip must cover the center with a non-background tone
        let center = gltf_px(pixels, GLTF_TEST_W / 2, GLTF_TEST_H / 2)
        t |> success(center.x > 40 && center.y > 40 && center.z > 40, "center should be the lit default material, got {center}")
        let corner = gltf_px(pixels, 2, 2)
        t |> success(center.x > corner.x + 20, "center should differ from the clear color, got center {center} corner {corner}")
        delete pixels
        delete scene
    }
}
