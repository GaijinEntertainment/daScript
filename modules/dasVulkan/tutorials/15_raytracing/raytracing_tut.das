options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 15 -- hardware ray tracing: the host side.
//
// build_rt_view assembles the whole RT stack once and keeps it resident: BLAS + TLAS over the
// scene tables (build_blas / build_tlas), descriptor + pipeline layouts decoded straight from the
// shaders' reflection, the 3-stage-plus-shadow-miss pipeline (create_ray_tracing_pipeline), its
// shader binding table (build_shader_binding_table), and the storage image the raygen writes.
// record_trace re-fires the whole frame from one push constant -- the camera orbit angle -- so the
// windowed viewer traces the scene live every frame.
//
// render_raytracing is the one-shot offscreen entry the pixel-oracle test uses; its oracle twin
// render_raytracing_cpu traces the SAME scene with the SAME shading functions
// (raytracing_tut_shaders) in the interpreter -- one source, two execution tiers, and the test
// asserts they agree pixel-for-pixel (up to a small silhouette budget where hardware traversal
// and the CPU intersector round differently).

module raytracing_tut public

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect public
require raytracing_tut_shaders public
require math public

//! True iff a Vulkan loader, a physical device, and the whole RT feature set are present.
//! The tutorial test soft-skips when false (CI lavapipe has no RT pipeline support).
def rt_available : bool {
    if (volkInitialize() != 0) {
        return false
    }
    var inscope instance <- create_instance("dasVulkan tutorial 15 probe", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    var count = 0u
    vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, null)
    if (count == 0u) {
        return false
    }
    let phys = select_physical_device(instance)
    return rt_supported(phys)
}

//! the resident RT stack: acceleration structures, layouts, pipeline, SBT, and the target image
struct RtView {
    blas : AccelStructure
    tlas : AccelStructure
    set_layouts : array<DescriptorSetLayout>
    pipe_layout : PipelineLayout
    desc_pool : DescriptorPool
    dsets : array<DescriptorSet>
    pipeline : Pipeline
    sbt : ShaderBindingTable
    image : Image
    memory : DeviceMemory
    view : ImageView
    width : int
    height : int
}

def finalize(var rv : RtView) {
    delete rv.sbt
    delete rv.pipeline
    delete rv.desc_pool   // frees the sets allocated from it
    delete rv.dsets
    delete rv.pipe_layout
    delete rv.set_layouts
    delete rv.view
    delete rv.image
    delete rv.memory
    delete rv.tlas
    delete rv.blas
}

//! Build the resident RT stack. The device must come from create_device_ray_tracing; pool/queue
//! are used (and waited idle) for the BLAS/TLAS builds.
def build_rt_view(device : Device; phys : VkPhysicalDevice; pool : CommandPool; queue : VkQueue;   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
                  w, h : int) : RtView {
    let dev = boost_value_to_vk(device)

    // acceleration structures over the scene tables
    var verts <- rt_scene_vertices()
    var idx <- rt_scene_indices()
    var inscope blas <- build_blas(device, phys, pool, queue, verts, idx)
    delete verts
    delete idx
    var insts <- [make_accel_instance(blas.address)]
    var inscope tlas <- build_tlas(device, phys, pool, queue, insts)
    delete insts

    // layouts straight from shader reflection: set 0 = { binding 0 TLAS, binding 1 storage image },
    // plus the raygen's `angle` push-constant range
    var reflections <- [decode_reflection(raytracing_rgen_spv_reflect),
        decode_reflection(raytracing_miss_spv_reflect),
        decode_reflection(raytracing_shadow_miss_spv_reflect),
        decode_reflection(raytracing_chit_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)
    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

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

    // the storage image the raygen writes (TRANSFER_SRC for both the readback and the blit path)
    let fmt = VkFormat.R8G8B8A8_UNORM
    var ici = ImageCreateInfo(imageType = VkImageType._2D, format = fmt, mipLevels = 1u,
        arrayLayers = 1u, tiling = VkImageTiling.OPTIMAL, initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(w)
    ici.extent.height = uint(h)
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

    // pipeline (raygen + [sky miss, shadow miss] + closest-hit) + its SBT; the shader modules may
    // be destroyed once the pipeline exists, so they stay scope-local
    var inscope rgen <- create_shader_module(device, raytracing_rgen_spv)
    var inscope miss_sky <- create_shader_module(device, raytracing_miss_spv)
    var inscope miss_shadow <- create_shader_module(device, raytracing_shadow_miss_spv)
    var inscope chit <- create_shader_module(device, raytracing_chit_spv)
    var miss_list <- [weak_copy(miss_sky), weak_copy(miss_shadow)]
    var inscope pipeline <- create_ray_tracing_pipeline(device, pipe_layout, rgen, miss_list, chit, 2u)
    delete miss_list
    var inscope sbt <- build_shader_binding_table(device, phys, pipeline, 2u, 1u)

    return <- RtView(blas <- blas, tlas <- tlas, set_layouts <- set_layouts,
        pipe_layout <- pipe_layout, desc_pool <- desc_pool, dsets <- dsets,
        pipeline <- pipeline, sbt <- sbt, image <- image, memory <- imem, view <- view,
        width = w, height = h)
}

//! Record one full trace of the scene into `cmd` for the given camera orbit angle. The caller
//! owns the image layout: it must be GENERAL before this call, and the barriers around the
//! raygen's writes are the caller's (see render_raytracing / the windowed viewer).
def record_trace(cmd : CommandBuffer; rv : RtView; angle : float) {
    cmd_bind_pipeline(cmd, rv.pipeline, VkPipelineBindPoint.RAY_TRACING_KHR)
    var sets : array<DescriptorSet>
    sets |> push(weak_copy(rv.dsets[0]))
    var no_dyn : array<uint>
    cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.RAY_TRACING_KHR, rv.pipe_layout, 0u, sets, no_dyn)
    pc.angle = angle
    raytracing_rgen_push_constants(cmd, rv.pipe_layout)
    cmd_trace_rays(cmd, rv.sbt, uint(rv.width), uint(rv.height))
    delete sets
}

//! Everything a headless offscreen trace needs, owned together: its own instance/device plus the
//! resident RtView and a readback buffer. Built once (build_rt_offscreen), traced per frame
//! (render_rt_frame) -- the shape both the pixel-oracle test and the recording driver share.
struct RtOffscreen {
    instance : Instance
    device : Device
    pool : CommandPool
    rv : RtView
    readback : HostBuffer
    queue : VkQueue
    phys : VkPhysicalDevice
    width : int
    height : int
}

def finalize(var ro : RtOffscreen) {
    delete ro.readback
    delete ro.rv
    delete ro.pool
    delete ro.device
    delete ro.instance
}

def build_rt_offscreen(w, h : int) : RtOffscreen {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 15", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    var phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device_ray_tracing(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope rv <- build_rt_view(device, phys, pool, queue, w, h)
    var inscope readback <- create_host_buffer(device, phys, uint64(w * h * 4))
    return <- RtOffscreen(instance <- instance, device <- device, pool <- pool, rv <- rv,
        readback <- readback, queue = queue, phys = phys, width = w, height = h)
}

//! Trace one frame at the given orbit angle and return its RGBA8 pixels. The storage image is
//! discarded (UNDEFINED) at the top of every frame, so frames are independent.
def render_rt_frame(var ro : RtOffscreen; angle : float) : array<uint8> {
    run_cmd_sync(ro.device, ro.pool, ro.queue) $(cmd) {
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
        transition_image(cmd, ro.rv.image, VkImageLayout.UNDEFINED, VkImageLayout.GENERAL,
            none, shader_write, top, rt_stage)
        record_trace(cmd, ro.rv, angle)
        transition_image(cmd, ro.rv.image, VkImageLayout.GENERAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            shader_write, to_read, rt_stage, xfer)
        copy_image_to_buffer(cmd, ro.rv.image, ro.readback, ro.width, ro.height)
    }
    var pixels : array<uint8>
    map_memory_to_array(ro.device, ro.readback.memory, uint64(ro.width * ro.height * 4)) $(m) {
        pixels := m
    }
    return <- pixels
}

//! One-shot offscreen render: trace the scene at `angle` into a w*h image and return its RGBA8
//! pixels. Creates (and tears down) its own instance/device; the pixel-oracle test's GPU half.
def render_raytracing(angle : float; w, h : int) : array<uint8> {
    var inscope ro <- build_rt_offscreen(w, h)
    return <- render_rt_frame(ro, angle)
}

// ===== the CPU reference tracer (the test's oracle) =====

// Moller-Trumbore over every triangle -- no BVH, the scene is 14 triangles. Returns the closest
// hit's t and primitive index.
def private cpu_trace(verts : array<float3>; idx : array<uint>; origin, dir : float3;
                      tmin, tmax : float; var out_t : float&; var out_prim : int&) : bool {
    var best = tmax
    var best_prim = -1
    let ntri = length(idx) / 3
    for (i in range(ntri)) {
        let v0 = verts[int(idx[i * 3])]
        let v1 = verts[int(idx[i * 3 + 1])]
        let v2 = verts[int(idx[i * 3 + 2])]
        let e1 = v1 - v0
        let e2 = v2 - v0
        let pv = cross(dir, e2)
        let det = dot(e1, pv)
        if (abs(det) < 1e-8) {
            continue
        }
        let inv_det = 1.0 / det
        let tv = origin - v0
        let u = dot(tv, pv) * inv_det
        if (u < 0.0 || u > 1.0) {
            continue
        }
        let qv = cross(tv, e1)
        let v = dot(dir, qv) * inv_det
        if (v < 0.0 || u + v > 1.0) {
            continue
        }
        let t = dot(e2, qv) * inv_det
        if (t > tmin && t < best) {
            best = t
            best_prim = i
        }
    }
    if (best_prim < 0) {
        return false
    }
    out_t = best
    out_prim = best_prim
    return true
}

def private unorm8(c : float) : uint8 => uint8(clamp(c, 0.0, 1.0) * 255.0 + 0.5)

//! The same frame render_raytracing produces, traced in the interpreter with the same scene
//! tables and shading functions. The test compares the two images pixel-for-pixel.
def render_raytracing_cpu(angle : float; w, h : int) : array<uint8> {
    var verts <- rt_scene_vertices()
    var idx <- rt_scene_indices()
    var pixels : array<uint8>
    pixels |> resize(w * h * 4)
    let origin = rt_camera_origin(angle)
    for (py in range(h)) {
        for (px in range(w)) {
            let dir = rt_camera_dir(origin, float(px), float(py), float(w), float(h))
            var color : float3
            var t = 0.0
            var prim = -1
            if (cpu_trace(verts, idx, origin, dir, 0.001, 100.0, t, prim)) {
                let n = rt_normal(prim)
                var albedo = rt_box_albedo()
                if (prim < 2) {
                    albedo = rt_ground_albedo()
                }
                let p = origin + dir * t
                var lit = 1.0
                var st = 0.0
                var sprim = -1
                if (cpu_trace(verts, idx, p + n * 0.001, rt_to_light(), 0.001, 100.0, st, sprim)) {
                    lit = 0.0
                }
                color = shade_lambert(n, albedo, lit)
            } else {
                color = sky_color(dir)
            }
            let o = (py * w + px) * 4
            pixels[o] = unorm8(color.x)
            pixels[o + 1] = unorm8(color.y)
            pixels[o + 2] = unorm8(color.z)
            pixels[o + 3] = 255u8
        }
    }
    delete verts
    delete idx
    return <- pixels
}
