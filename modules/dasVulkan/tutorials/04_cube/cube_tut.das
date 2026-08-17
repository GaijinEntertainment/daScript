options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 04 render path: draw the synthwave neon cube at `time` (seconds, controls the breathing
// scale + texture UV scroll) and `camera_t` (in [0..1), drives the lemniscate orbit). Returns
// CUBE_W*CUBE_H RGBA8 pixels. A parametric frame(time, camera_t) -> image: the CI gate asserts a
// known frame, and the recording driver sweeps camera_t over a full lemniscate. Headline rails:
// vertex buffer with (pos, uv, normal) per-vertex attributes, UBO matrices, push-const model
// matrix, combined image sampler in the fragment, and a depth attachment (so back faces don't
// punch through front faces).

module cube_tut

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require cube_tut_shaders
require math

let public CUBE_W = 768
let public CUBE_H = 768
let CUBE_DEPTH_FMT = VkFormat.D32_SFLOAT
let CUBE_COLOR_FMT = VkFormat.R8G8B8A8_SRGB   // sRGB: gamma-encode the linear output on write (recording readback + window blit match)
let TEX_DIM = 256

let public N_VERTS = 24                    // 6 faces * 4 corners (per-face normals -> faceted look)
let public N_INDICES = 36                  // 6 faces * 2 tris * 3 indices

// ===== cube geometry =====

//! 24 vertices, 4 per face. Layout per vertex (32 bytes): pos.xyz (float3, +0), uv (float2, +12),
//! normal.xyz (float3, +20). CCW winding from outside (matches the pipeline's CCW front-face).
let private cube_vertices : array<float> <- [
    // +Z (front)
    -0.5, -0.5, 0.5,  0.0, 1.0,   0.0, 0.0, 1.0,
     0.5, -0.5, 0.5,  1.0, 1.0,   0.0, 0.0, 1.0,
     0.5, 0.5, 0.5,  1.0, 0.0,   0.0, 0.0, 1.0,
    -0.5, 0.5, 0.5,  0.0, 0.0,   0.0, 0.0, 1.0,
    // -Z (back)
     0.5, -0.5, -0.5,  0.0, 1.0,   0.0, 0.0, -1.0,
    -0.5, -0.5, -0.5,  1.0, 1.0,   0.0, 0.0, -1.0,
    -0.5, 0.5, -0.5,  1.0, 0.0,   0.0, 0.0, -1.0,
     0.5, 0.5, -0.5,  0.0, 0.0,   0.0, 0.0, -1.0,
    // +X (right)
     0.5, -0.5, 0.5,  0.0, 1.0,   1.0, 0.0, 0.0,
     0.5, -0.5, -0.5,  1.0, 1.0,   1.0, 0.0, 0.0,
     0.5, 0.5, -0.5,  1.0, 0.0,   1.0, 0.0, 0.0,
     0.5, 0.5, 0.5,  0.0, 0.0,   1.0, 0.0, 0.0,
    // -X (left)
    -0.5, -0.5, -0.5,  0.0, 1.0,  -1.0, 0.0, 0.0,
    -0.5, -0.5, 0.5,  1.0, 1.0,  -1.0, 0.0, 0.0,
    -0.5, 0.5, 0.5,  1.0, 0.0,  -1.0, 0.0, 0.0,
    -0.5, 0.5, -0.5,  0.0, 0.0,  -1.0, 0.0, 0.0,
    // +Y (top)
    -0.5, 0.5, 0.5,  0.0, 1.0,   0.0, 1.0, 0.0,
     0.5, 0.5, 0.5,  1.0, 1.0,   0.0, 1.0, 0.0,
     0.5, 0.5, -0.5,  1.0, 0.0,   0.0, 1.0, 0.0,
    -0.5, 0.5, -0.5,  0.0, 0.0,   0.0, 1.0, 0.0,
    // -Y (bottom)
    -0.5, -0.5, -0.5,  0.0, 1.0,   0.0, -1.0, 0.0,
     0.5, -0.5, -0.5,  1.0, 1.0,   0.0, -1.0, 0.0,
     0.5, -0.5, 0.5,  1.0, 0.0,   0.0, -1.0, 0.0,
    -0.5, -0.5, 0.5,  0.0, 0.0,   0.0, -1.0, 0.0]

//! 36 indices, two CCW triangles per face: 0-1-2 and 0-2-3 in face-local order, with face base
//! 0/4/8/12/16/20.
let private cube_indices : array<uint16> <- [
    uint16(0), uint16(1), uint16(2),  uint16(0), uint16(2), uint16(3),
    uint16(4), uint16(5), uint16(6),  uint16(4), uint16(6), uint16(7),
    uint16(8), uint16(9), uint16(10), uint16(8), uint16(10), uint16(11),
    uint16(12), uint16(13), uint16(14), uint16(12), uint16(14), uint16(15),
    uint16(16), uint16(17), uint16(18), uint16(16), uint16(18), uint16(19),
    uint16(20), uint16(21), uint16(22), uint16(20), uint16(22), uint16(23)]

// ===== matrix builders (column-major, 16 floats each) =====

//! Vulkan-style perspective: maps view-space z in (-near .. -far) to clip-space z in (0 .. 1) after
//! the perspective divide, and flips Y so the framebuffer is right-side-up. fov_y is in radians.
def perspective_vk(fov_y, aspect, znear, zfar : float) : float4x4 {
    let f = 1.0f / tan(fov_y * 0.5f)
    let nf = 1.0f / (znear - zfar)
    var m : float4x4
    m[0] = float4(f / aspect, 0.0f, 0.0f, 0.0f)
    m[1] = float4(0.0f, -f, 0.0f, 0.0f)                 // Y flip -- Vulkan NDC has +Y down
    m[2] = float4(0.0f, 0.0f, zfar * nf, -1.0f)
    m[3] = float4(0.0f, 0.0f, znear * zfar * nf, 0.0f)
    return m
}

def vec3_sub(a, b : float3) : float3 => float3(a.x - b.x, a.y - b.y, a.z - b.z)
def vec3_cross(a, b : float3) : float3 => float3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x)
def vec3_dot(a, b : float3) : float => a.x * b.x + a.y * b.y + a.z * b.z
def vec3_normalize(v : float3) : float3 {
    let len = sqrt(vec3_dot(v, v))
    return float3(v.x / len, v.y / len, v.z / len)
}

//! Right-handed lookAt, column-major.
def look_at_rh(eye, center, up : float3) : float4x4 {
    let f = vec3_normalize(vec3_sub(center, eye))
    let s = vec3_normalize(vec3_cross(f, up))
    let u = vec3_cross(s, f)
    var m : float4x4
    m[0] = float4(s.x, u.x, -f.x, 0.0f)
    m[1] = float4(s.y, u.y, -f.y, 0.0f)
    m[2] = float4(s.z, u.z, -f.z, 0.0f)
    m[3] = float4(-vec3_dot(s, eye), -vec3_dot(u, eye), vec3_dot(f, eye), 1.0f)
    return m
}

//! Uniform-scale rotation around the Y axis. Pure rotation+scale only -- the vertex shader uses a
//! transposed-cofactor-free normal transform that assumes this.
def model_matrix(angle, scale : float) : float4x4 {
    let c = cos(angle) * scale
    let s = sin(angle) * scale
    var m : float4x4
    m[0] = float4(c, 0.0f, -s, 0.0f)
    m[1] = float4(0.0f, scale, 0.0f, 0.0f)
    m[2] = float4(s, 0.0f, c, 0.0f)
    m[3] = float4(0.0f, 0.0f, 0.0f, 1.0f)
    return m
}

// ===== procedural synthwave texture =====

//! 256x256 RGBA8 synthwave horizon: deep-purple to magenta sky with a banded neon sun above the
//! horizon line, dark-indigo ground with a cyan perspective grid receding into the vanishing point.
//! Generated CPU-side at init -- no asset file, no compute pre-pass.
def gen_synthwave_texture(w, h : int) : array<uint8> {
    var pixels : array<uint8>
    pixels |> resize(w * h * 4)
    let horizon = h / 2 - 8                    // horizon line a bit above center
    let sun_cx = w / 2
    let sun_cy = horizon - 26
    let sun_r = 42
    for (y in range(h)) {
        for (x in range(w)) {
            var r = 0
            var g = 0
            var b = 0
            if (y < horizon) {
                // sky: deep purple at top, magenta near horizon
                let t = float(y) / float(horizon)
                r = int(40.0f + t * 215.0f)
                g = int(6.0f  + t * 64.0f)
                b = int(80.0f + t * 175.0f)
                // sun disk with horizontal scanline bands
                let dx = x - sun_cx
                let dy = y - sun_cy
                if (dx * dx + dy * dy < sun_r * sun_r) {
                    let band = (y / 4) % 2
                    if (band == 0) {
                        let glow = clamp(1.0f - float(dy + sun_r) / float(2 * sun_r), 0.0f, 1.0f)
                        r = 255
                        g = int(120.0f + glow * 100.0f)
                        b = int(60.0f  + glow * 80.0f)
                    }
                }
            } elif (y < horizon + 4) {
                // bright magenta horizon line
                r = 255
                g = 90
                b = 210
            } else {
                // ground: dark indigo with cyan perspective grid
                r = 6
                g = 6
                b = 26
                let dist = y - horizon - 4
                let max_dist = h - horizon - 4
                let scale = float(dist) / float(max_dist)
                // horizontal lines, spacing widens with distance from horizon
                let spacing = max(2, int(2.0f + scale * 18.0f))
                if (dist % spacing < 1) {
                    r = 0
                    g = 220
                    b = 255
                }
                // vertical lines converging to (w/2, horizon)
                let dx = abs(x - w / 2)
                let v_spacing = max(2, int(4.0f + scale * 40.0f))
                if (dx % v_spacing < 2 && scale > 0.02f) {
                    r = (r + 0) / 2
                    g = (g + 200) / 2
                    b = (b + 255) / 2
                }
            }
            let idx = (y * w + x) * 4
            pixels[idx + 0] = uint8(clamp(r, 0, 255))
            pixels[idx + 1] = uint8(clamp(g, 0, 255))
            pixels[idx + 2] = uint8(clamp(b, 0, 255))
            pixels[idx + 3] = 255u8
        }
    }
    return <- pixels
}

// ===== reusable per-frame context =====

//! All cube rendering state depending only on (device, phys, queue, pool): offscreen targets, render
//! pass, framebuffer, geometry/UBO buffers, procedural texture + sampler + descriptors, pipeline and a
//! host-visible readback. The driver passes those in, so one bundle serves the oracle or a swapchain blit.
struct public CubeResources {
    color       : OffscreenTarget
    depth       : OffscreenDepth
    render_pass : RenderPass
    framebuffer : Framebuffer
    vb          : HostBuffer
    ib          : HostBuffer
    ubo         : HostBuffer
    tex_image   : Image
    tex_memory  : DeviceMemory
    tex_view    : ImageView
    sampler     : Sampler
    set_layouts : array<DescriptorSetLayout>
    desc_pool   : DescriptorPool
    desc_set    : VkDescriptorSet
    pipe_layout : PipelineLayout
    pipeline    : Pipeline
    readback    : HostBuffer
    buf_size    : uint64
    push_size   : uint
}

//! Reverse-of-creation cleanup (the desc_set is owned by desc_pool so its raw handle just goes
//! away when the pool destroys; nothing to delete for it explicitly).
def public finalize(var r : CubeResources) {
    unsafe {
        delete r.readback
        delete r.pipeline
        delete r.pipe_layout
        delete r.desc_pool
        delete r.set_layouts
        delete r.sampler
        delete r.tex_view
        delete r.tex_image
        delete r.tex_memory
        delete r.ubo
        delete r.ib
        delete r.vb
        delete r.framebuffer
        delete r.render_pass
        delete r.depth
        delete r.color
    }
}

//! Self-contained one-shot context for the offscreen oracle + recording driver: owns its own
//! instance/device/queue/pool plus the full CubeResources bundle. Windowed drivers skip it -- they build
//! an instance (surface extensions) + device (VK_KHR_swapchain) themselves, then build_cube_resources.
struct public CubeContext {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : CubeResources
}

def public finalize(var ctx : CubeContext) {
    unsafe {
        delete ctx.res
        delete ctx.pool
        delete ctx.device
        delete ctx.instance
    }
}

//! Build the cube's render resources against a caller-owned (device, phys, queue, pool). Used by both
//! build_cube_context (offscreen test/recording) and window/show_cube.das (swapchain blit). The pool's
//! queue family must match `queue`'s -- the texture upload is recorded into pool and submitted on queue.
def public build_cube_resources(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool) : CubeResources {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    let dev = boost_value_to_vk(device)

    var inscope color <- build_offscreen_target(device, phys, CUBE_W, CUBE_H, CUBE_COLOR_FMT)
    var inscope depth <- build_offscreen_depth(device, phys, CUBE_W, CUBE_H, CUBE_DEPTH_FMT)
    var inscope render_pass <- create_render_pass_color_depth(device, CUBE_COLOR_FMT, CUBE_DEPTH_FMT)
    var inscope framebuffer <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(color.view), weak_copy(depth.view)],
        width = uint(CUBE_W),
        height = uint(CUBE_H),
        layers = 1u))

    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var verts_copy := cube_vertices
    var inscope vb <- create_host_buffer_from_bytes(device, phys, vb_usage, verts_copy)
    delete verts_copy
    var ib_usage : VkBufferUsageFlags
    ib_usage.index_buffer = true
    var indices_copy := cube_indices
    var inscope ib <- create_host_buffer_from_bytes(device, phys, ib_usage, indices_copy)
    delete indices_copy

    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo <- create_host_buffer(device, phys, 144ul, ubo_usage)        // view + proj + cam_time

    // procedural synthwave texture: 256x256 RGBA8 host data, staged into a device-local image
    var tex_pixels <- gen_synthwave_texture(TEX_DIM, TEX_DIM)
    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- create_host_buffer_from_bytes(device, phys, staging_usage, tex_pixels)
    delete tex_pixels

    var ici = ImageCreateInfo(
        imageType = VkImageType._2D,
        format = CUBE_COLOR_FMT,
        mipLevels = 1u,
        arrayLayers = 1u,
        tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(TEX_DIM)
    ici.extent.height = uint(TEX_DIM)
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.sampled = true
    ici.usage.transfer_dst = true
    var inscope tex_image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(tex_image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(
        allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope tex_memory <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(tex_image), boost_value_to_vk(tex_memory), 0ul), null)

    var tvci = ImageViewCreateInfo(
        image = weak_copy(tex_image),
        viewType = VkImageViewType._2D,
        format = CUBE_COLOR_FMT)
    tvci.subresourceRange.aspectMask.color = true
    tvci.subresourceRange.levelCount = 1u
    tvci.subresourceRange.layerCount = 1u
    var inscope tex_view <- create_image_view(device, tvci)

    let sci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.LINEAR,
        addressModeU = VkSamplerAddressMode.REPEAT,
        addressModeV = VkSamplerAddressMode.REPEAT,
        addressModeW = VkSamplerAddressMode.REPEAT,
        maxLod = 1.0f)
    var inscope sampler <- create_sampler(device, sci)

    var reflections <- [decode_reflection(cube_vert_spv_reflect), decode_reflection(cube_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)

    var dpci = DescriptorPoolCreateInfo(
        maxSets = 1u,
        pPoolSizes <- [
            DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u),
            DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)])
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(set_layouts[0])
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }

    var writes : array<WriteDescriptorSet>
    var w0 = WriteDescriptorSet(
        dstSet = vk_value_to_boost(raw_set),
        dstBinding = 0u,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER,
        descriptorCount = 1u)
    w0.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(ubo.buffer), range_ = 144ul))
    writes |> emplace(w0)
    var w1 = WriteDescriptorSet(
        dstSet = vk_value_to_boost(raw_set),
        dstBinding = 1u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER,
        descriptorCount = 1u)
    w1.pImageInfo |> push(DescriptorImageInfo(
        sampler = weak_copy(sampler),
        imageView = weak_copy(tex_view),
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL))
    writes |> emplace(w1)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    var push_size = 0u
    for (refl in reflections) {
        for (p in refl.push_constants) {
            push_size = uint(p.size)
        }
    }
    assert(push_size == 64u, "expected reflected push constant size 64 (model float4x4)")
    delete reflections

    var inscope vert <- create_shader_module(device, cube_vert_spv)
    var inscope frag <- create_shader_module(device, cube_frag_spv)

    var vbindings : array<VkVertexInputBindingDescription>
    var vb_desc : VkVertexInputBindingDescription
    vb_desc.binding = 0u
    vb_desc.stride = 32u
    vb_desc.inputRate = VkVertexInputRate.VERTEX
    vbindings |> push(vb_desc)
    var vattrs : array<VkVertexInputAttributeDescription>
    var attr_pos : VkVertexInputAttributeDescription
    attr_pos.location = 0u
    attr_pos.binding = 0u
    attr_pos.format = VkFormat.R32G32B32_SFLOAT
    attr_pos.offset = 0u
    vattrs |> push(attr_pos)
    var attr_uv : VkVertexInputAttributeDescription
    attr_uv.location = 1u
    attr_uv.binding = 0u
    attr_uv.format = VkFormat.R32G32_SFLOAT
    attr_uv.offset = 12u
    vattrs |> push(attr_uv)
    var attr_n : VkVertexInputAttributeDescription
    attr_n.location = 2u
    attr_n.binding = 0u
    attr_n.format = VkFormat.R32G32B32_SFLOAT
    attr_n.offset = 20u
    vattrs |> push(attr_n)
    var inscope pipeline <- create_graphics_pipeline_v3d(device, render_pass, pipe_layout, vert, frag,
        CUBE_W, CUBE_H, vbindings, vattrs)
    delete vbindings
    delete vattrs

    let buf_size = uint64(CUBE_W * CUBE_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    // one-shot texture upload: staging -> tex_image, transitioned UNDEFINED -> TRANSFER_DST -> SHADER_READ_ONLY
    run_cmd_sync(device, pool, queue) $(cmd) {
        var to_dst : VkAccessFlags
        to_dst.transfer_write = true
        let none : VkAccessFlags
        var top : VkPipelineStageFlags
        top.top_of_pipe = true
        var xfer : VkPipelineStageFlags
        xfer.transfer = true
        var frag_stage : VkPipelineStageFlags
        frag_stage.fragment_shader = true
        var to_read : VkAccessFlags
        to_read.shader_read = true
        transition_image(cmd, tex_image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, to_dst, top, xfer)
        var region : BufferImageCopy
        region.imageSubresource.aspectMask.color = true
        region.imageSubresource.layerCount = 1u
        region.imageExtent.width = uint(TEX_DIM)
        region.imageExtent.height = uint(TEX_DIM)
        region.imageExtent.depth = 1u
        var regions : array<BufferImageCopy>
        regions |> emplace(region)
        cmd_copy_buffer_to_image(cmd, staging.buffer, tex_image, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
        transition_image(cmd, tex_image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_dst, to_read, xfer, frag_stage)
    }

    return <- CubeResources(
        color <- color, depth <- depth, render_pass <- render_pass, framebuffer <- framebuffer,
        vb <- vb, ib <- ib, ubo <- ubo,
        tex_image <- tex_image, tex_memory <- tex_memory, tex_view <- tex_view, sampler <- sampler,
        set_layouts <- set_layouts, desc_pool <- desc_pool, desc_set = raw_set,
        pipe_layout <- pipe_layout, pipeline <- pipeline, readback <- readback,
        buf_size = buf_size, push_size = push_size)
}

//! Build the offscreen-self-contained context (own instance + device + queue + pool + resources).
//! The [test] and the recording driver use this; window/show_cube.das does NOT -- it owns its own
//! instance/device with surface+swapchain extensions and calls build_cube_resources directly.
def public build_cube_context() : CubeContext {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 04", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope res <- build_cube_resources(device, phys, queue, pool)
    return <- CubeContext(
        instance <- instance, device <- device, queue = queue, pool <- pool, res <- res)
}

//! Host-side per-frame prep: derive camera matrices + cam_time from `camera_t in [0,1)` and `time`
//! (seconds), push them into the UBO via cube_vs_bind_uniform, and stamp the model push constant.
//! Both the offscreen oracle and the windowed driver call this once before recording any commands.
def public update_cube_uniforms(res : CubeResources; device : Device; time, camera_t : float) {
    let camera_angle = camera_t * 2.0f * PI
    let camera_r = 3.5f
    let cam_x = camera_r * cos(camera_angle)
    let cam_z = camera_r * sin(camera_angle)
    // Lemniscate at 2x frequency, but small amplitude -- the camera stays near the cube's height
    // (cube extends y in [-0.5, 0.5]) so the top/bottom faces, which are textured for a face-on
    // angle, only graze the view instead of dominating it.
    let cam_y = sin(camera_angle * 2.0f) * 0.4f
    let cam_pos = float3(cam_x, cam_y, cam_z)
    cam.view = look_at_rh(cam_pos, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f))
    cam.proj = perspective_vk(60.0f * PI / 180.0f, float(CUBE_W) / float(CUBE_H), 0.1f, 10.0f)
    cam.cam_time = float4(cam_pos.x, cam_pos.y, cam_pos.z, time)
    cube_vs_bind_uniform(device, res.ubo.memory)

    let breathe = 1.0f + 0.05f * sin(time * 2.0f * PI / 5.0f)
    let spin = time * 0.5f
    pc.model = model_matrix(spin, breathe)
}

//! Record the cube's render pass + draw. The pass ends with the color attachment in TRANSFER_SRC_OPTIMAL
//! (create_render_pass_color_depth's default), so a windowed driver blits res.color.image straight into
//! the swap target. Caller must run update_cube_uniforms first to load the UBO + push-constant globals.
def public record_cube_render_pass(res : CubeResources; cmd : CommandBuffer) {
    var clears <- [clear_color(0.02f, 0.0f, 0.06f, 1.0f), clear_depth(1.0f)]
    record_render_pass(cmd, res.render_pass, res.framebuffer, full_area(CUBE_W, CUBE_H), clears) {
        cmd_bind_pipeline(cmd, res.pipeline)
        let sets <- [vk_value_to_boost(res.desc_set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.pipe_layout, 0u, sets, no_dyn)
        cube_vs_push_constants(cmd, res.pipe_layout)
        let vbufs <- [weak_copy(res.vb.buffer)]
        var voffs : array<uint64>
        voffs |> push(0ul)
        cmd_bind_vertex_buffers(cmd, 0u, vbufs, voffs)
        cmd_bind_index_buffer(cmd, weak_copy(res.ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, uint(N_INDICES), 1u, 0u, 0, 0u)
    }
    delete clears
}

//! Per-frame work: update UBO + push constant, record the render pass, copy the color attachment
//! into the readback buffer, clone the bytes out as RGBA8.
def public render_cube_frame(var ctx : CubeContext; time, camera_t : float) : array<uint8> {
    update_cube_uniforms(ctx.res, ctx.device, time, camera_t)
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_cube_render_pass(ctx.res, cmd)
        copy_image_to_buffer(cmd, ctx.res.color.image, ctx.res.readback, CUBE_W, CUBE_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! One-shot: build a context and render one frame. The test uses this; the recording driver uses
//! build_cube_context + render_cube_frame directly in a loop.
def public render_synthwave_cube(time, camera_t : float) : array<uint8> {
    var inscope ctx <- build_cube_context()
    return <- render_cube_frame(ctx, time, camera_t)
}

//! Sample the RGB of pixel (x,y) from a CUBE_W*CUBE_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * CUBE_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
