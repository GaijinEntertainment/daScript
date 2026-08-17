options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 05 render path: a thousand small cubes in one indexed-instanced draw call. Per-vertex
// geometry (pos + normal) is bound at binding 0 with input rate VERTEX; the per-instance buffer
// (offset.xyz + color.rgb + phase) is bound at binding 1 with input rate INSTANCE. The instance
// buffer is static -- built once at init from a Lissajous-style 3D curve seeded by instance index --
// and the swarm is animated entirely in the vertex shader by mixing the instance's offset with the
// UBO ``cam_time.w``. Returns INSTANCE_W*INSTANCE_H RGBA8 pixels.

module instancing_tut

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require instancing_tut_shaders
require math

let public INSTANCE_W = 768
let public INSTANCE_H = 768
let INSTANCE_DEPTH_FMT = VkFormat.D32_SFLOAT
let INSTANCE_COLOR_FMT = VkFormat.R8G8B8A8_SRGB   // sRGB: gamma-encode the linear output on write (recording readback + window blit match)

//! 1000 cubes per draw call. Comfortably visible at 768x768 with ~6-unit swarm radius and the camera
//! orbiting at distance 14 -- the centre band of the frame is dominantly cubes while the corners are
//! still clear-colour background.
let public N_INSTANCES = 1000

let public N_VERTS = 24                    // 6 faces * 4 corners
let public N_INDICES = 36                  // 6 faces * 2 tris * 3 indices

// per-vertex stride: 3 (pos) + 3 (normal) = 6 floats = 24 bytes
let VERT_STRIDE = 24u
// per-instance stride: 3 (offset) + 3 (color) + 1 (phase) = 7 floats = 28 bytes
let INST_STRIDE = 28u
let INST_FLOATS_PER = 7

// ===== cube geometry (pos + normal, per-face normals -> faceted look) =====

let private cube_vertices : array<float> <- [
    // +Z (front)
    -0.5, -0.5, 0.5,   0.0, 0.0, 1.0,
     0.5, -0.5, 0.5,   0.0, 0.0, 1.0,
     0.5, 0.5, 0.5,    0.0, 0.0, 1.0,
    -0.5, 0.5, 0.5,    0.0, 0.0, 1.0,
    // -Z (back)
     0.5, -0.5, -0.5,  0.0, 0.0, -1.0,
    -0.5, -0.5, -0.5,  0.0, 0.0, -1.0,
    -0.5, 0.5, -0.5,   0.0, 0.0, -1.0,
     0.5, 0.5, -0.5,   0.0, 0.0, -1.0,
    // +X (right)
     0.5, -0.5, 0.5,   1.0, 0.0, 0.0,
     0.5, -0.5, -0.5,  1.0, 0.0, 0.0,
     0.5, 0.5, -0.5,   1.0, 0.0, 0.0,
     0.5, 0.5, 0.5,    1.0, 0.0, 0.0,
    // -X (left)
    -0.5, -0.5, -0.5,  -1.0, 0.0, 0.0,
    -0.5, -0.5, 0.5,   -1.0, 0.0, 0.0,
    -0.5, 0.5, 0.5,    -1.0, 0.0, 0.0,
    -0.5, 0.5, -0.5,   -1.0, 0.0, 0.0,
    // +Y (top)
    -0.5, 0.5, 0.5,    0.0, 1.0, 0.0,
     0.5, 0.5, 0.5,    0.0, 1.0, 0.0,
     0.5, 0.5, -0.5,   0.0, 1.0, 0.0,
    -0.5, 0.5, -0.5,   0.0, 1.0, 0.0,
    // -Y (bottom)
    -0.5, -0.5, -0.5,  0.0, -1.0, 0.0,
     0.5, -0.5, -0.5,  0.0, -1.0, 0.0,
     0.5, -0.5, 0.5,   0.0, -1.0, 0.0,
    -0.5, -0.5, 0.5,   0.0, -1.0, 0.0]

let private cube_indices : array<uint16> <- [
    uint16(0), uint16(1), uint16(2),  uint16(0), uint16(2), uint16(3),
    uint16(4), uint16(5), uint16(6),  uint16(4), uint16(6), uint16(7),
    uint16(8), uint16(9), uint16(10), uint16(8), uint16(10), uint16(11),
    uint16(12), uint16(13), uint16(14), uint16(12), uint16(14), uint16(15),
    uint16(16), uint16(17), uint16(18), uint16(16), uint16(18), uint16(19),
    uint16(20), uint16(21), uint16(22), uint16(20), uint16(22), uint16(23)]

// ===== matrix builders (column-major, 16 floats each) -- identical to tutorial 04 =====

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

// ===== per-instance buffer init =====

//! Build N_INSTANCES rows of (offset.xyz, color.rgb, phase). Offsets sample a Lissajous-style 3D curve
//! (7 azimuthal x 3 elevation lobes x radial breathing) so the swarm fills a balanced volume with no
//! axis alignment; colours cycle the hue circle 3x, and phase sweeps [0,1) to desync the breathing.
def gen_instance_data(n : int) : array<float> {
    var data : array<float>
    data |> resize(n * INST_FLOATS_PER)
    for (i in range(n)) {
        let fi = float(i) / float(n)
        let theta = fi * 2.0f * PI * 7.0f
        let phi = fi * 2.0f * PI * 3.0f
        let r = 5.0f + 1.5f * sin(fi * 2.0f * PI * 5.0f)
        let x = r * cos(theta) * cos(phi * 0.5f)
        let y = r * sin(phi)
        let z = r * sin(theta) * cos(phi * 0.5f)
        let hue = fi * 6.0f * PI                // three full hue cycles
        let cr = 0.5f + 0.5f * cos(hue)
        let cg = 0.5f + 0.5f * cos(hue + 2.0944f)        // 2 pi / 3
        let cb = 0.5f + 0.5f * cos(hue + 4.1888f)        // 4 pi / 3
        let base = i * INST_FLOATS_PER
        data[base + 0] = x
        data[base + 1] = y
        data[base + 2] = z
        data[base + 3] = cr
        data[base + 4] = cg
        data[base + 5] = cb
        data[base + 6] = fi
    }
    return <- data
}

// ===== reusable per-frame context =====

//! All instancing-render state depending only on (device, phys, queue, pool). Same shape as tutorial 04's
//! CubeResources (offscreen targets + render pass + framebuffer + pipeline + readback), plus a second
//! vertex buffer ``inst_buf`` for the per-instance stream. No texture/sampler -- the buffer is the point.
struct public InstancingResources {
    color       : OffscreenTarget
    depth       : OffscreenDepth
    render_pass : RenderPass
    framebuffer : Framebuffer
    vb          : HostBuffer        // per-vertex, binding 0
    ib          : HostBuffer        // index buffer
    inst_buf    : HostBuffer        // per-instance, binding 1
    ubo         : HostBuffer
    set_layouts : array<DescriptorSetLayout>
    desc_pool   : DescriptorPool
    desc_set    : VkDescriptorSet
    pipe_layout : PipelineLayout
    pipeline    : Pipeline
    readback    : HostBuffer
    buf_size    : uint64
}

def public finalize(var r : InstancingResources) {
    unsafe {
        delete r.readback
        delete r.pipeline
        delete r.pipe_layout
        delete r.desc_pool
        delete r.set_layouts
        delete r.ubo
        delete r.inst_buf
        delete r.ib
        delete r.vb
        delete r.framebuffer
        delete r.render_pass
        delete r.depth
        delete r.color
    }
}

struct public InstancingContext {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : InstancingResources
}

def public finalize(var ctx : InstancingContext) {
    unsafe {
        delete ctx.res
        delete ctx.pool
        delete ctx.device
        delete ctx.instance
    }
}

//! Build the offscreen render targets, the 2-binding vertex input pipeline, descriptors and the
//! readback buffer against a caller-owned (device, phys, queue, pool). Used by the offscreen test +
//! recording driver via build_instancing_context, and by window/show_instancing.das directly.
def public build_instancing_resources(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool) : InstancingResources {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    // queue/pool are unused here (no texture upload), but the signature mirrors build_cube_resources
    // so the windowed driver pattern carries across tutorials.
    let _unused_queue = queue
    let _unused_pool = pool

    var inscope color <- build_offscreen_target(device, phys, INSTANCE_W, INSTANCE_H, INSTANCE_COLOR_FMT)
    var inscope depth <- build_offscreen_depth(device, phys, INSTANCE_W, INSTANCE_H, INSTANCE_DEPTH_FMT)
    var inscope render_pass <- create_render_pass_color_depth(device, INSTANCE_COLOR_FMT, INSTANCE_DEPTH_FMT)
    var inscope framebuffer <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(color.view), weak_copy(depth.view)],
        width = uint(INSTANCE_W),
        height = uint(INSTANCE_H),
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

    var inst_usage : VkBufferUsageFlags
    inst_usage.vertex_buffer = true
    var inst_data <- gen_instance_data(N_INSTANCES)
    var inscope inst_buf <- create_host_buffer_from_bytes(device, phys, inst_usage, inst_data)
    delete inst_data

    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo <- create_host_buffer(device, phys, 144ul, ubo_usage)        // view + proj + cam_time

    var reflections <- [decode_reflection(instancing_vert_spv_reflect), decode_reflection(instancing_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)

    var dpci = DescriptorPoolCreateInfo(
        maxSets = 1u,
        pPoolSizes <- [DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)])
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(set_layouts[0])
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(boost_value_to_vk(device), dsai, addr(raw_set)), null)
    }

    var writes : array<WriteDescriptorSet>
    var w0 = WriteDescriptorSet(
        dstSet = vk_value_to_boost(raw_set),
        dstBinding = 0u,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER,
        descriptorCount = 1u)
    w0.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(ubo.buffer), range_ = 144ul))
    writes |> emplace(w0)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope vert <- create_shader_module(device, instancing_vert_spv)
    var inscope frag <- create_shader_module(device, instancing_frag_spv)

    var vbindings : array<VkVertexInputBindingDescription>
    var vb_desc : VkVertexInputBindingDescription
    vb_desc.binding = 0u
    vb_desc.stride = VERT_STRIDE
    vb_desc.inputRate = VkVertexInputRate.VERTEX
    vbindings |> push(vb_desc)
    var inst_desc : VkVertexInputBindingDescription
    inst_desc.binding = 1u
    inst_desc.stride = INST_STRIDE
    inst_desc.inputRate = VkVertexInputRate.INSTANCE        // <-- the headline rail
    vbindings |> push(inst_desc)

    var vattrs : array<VkVertexInputAttributeDescription>
    var attr_pos : VkVertexInputAttributeDescription
    attr_pos.location = 0u
    attr_pos.binding = 0u
    attr_pos.format = VkFormat.R32G32B32_SFLOAT
    attr_pos.offset = 0u
    vattrs |> push(attr_pos)
    var attr_n : VkVertexInputAttributeDescription
    attr_n.location = 1u
    attr_n.binding = 0u
    attr_n.format = VkFormat.R32G32B32_SFLOAT
    attr_n.offset = 12u
    vattrs |> push(attr_n)
    var attr_off : VkVertexInputAttributeDescription
    attr_off.location = 2u
    attr_off.binding = 1u                // per-instance binding
    attr_off.format = VkFormat.R32G32B32_SFLOAT
    attr_off.offset = 0u
    vattrs |> push(attr_off)
    var attr_col : VkVertexInputAttributeDescription
    attr_col.location = 3u
    attr_col.binding = 1u
    attr_col.format = VkFormat.R32G32B32_SFLOAT
    attr_col.offset = 12u
    vattrs |> push(attr_col)
    var attr_ph : VkVertexInputAttributeDescription
    attr_ph.location = 4u
    attr_ph.binding = 1u
    attr_ph.format = VkFormat.R32_SFLOAT
    attr_ph.offset = 24u
    vattrs |> push(attr_ph)

    var inscope pipeline <- create_graphics_pipeline_v3d(device, render_pass, pipe_layout, vert, frag,
        INSTANCE_W, INSTANCE_H, vbindings, vattrs)
    delete vbindings
    delete vattrs

    let buf_size = uint64(INSTANCE_W * INSTANCE_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    return <- InstancingResources(
        color <- color, depth <- depth, render_pass <- render_pass, framebuffer <- framebuffer,
        vb <- vb, ib <- ib, inst_buf <- inst_buf, ubo <- ubo,
        set_layouts <- set_layouts, desc_pool <- desc_pool, desc_set = raw_set,
        pipe_layout <- pipe_layout, pipeline <- pipeline, readback <- readback,
        buf_size = buf_size)
}

//! Self-contained one-shot context (own instance + device + queue + pool + resources). Used by the
//! [test] and the recording driver.
def public build_instancing_context() : InstancingContext {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 05", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope res <- build_instancing_resources(device, phys, queue, pool)
    return <- InstancingContext(
        instance <- instance, device <- device, queue = queue, pool <- pool, res <- res)
}

//! Host-side per-frame prep: derive camera matrices from ``camera_t in [0,1)`` and stamp the time
//! into ``cam.cam_time.w``. The instance buffer is static so no per-frame data movement past the
//! UBO write.
def public update_instancing_uniforms(res : InstancingResources; device : Device; time, camera_t : float) {
    let camera_angle = camera_t * 2.0f * PI
    let camera_r = 14.0f
    let cam_x = camera_r * cos(camera_angle)
    let cam_z = camera_r * sin(camera_angle)
    // gentle vertical sway so the swarm shows depth from a varying angle
    let cam_y = sin(camera_angle * 2.0f) * 2.0f
    let cam_pos = float3(cam_x, cam_y, cam_z)
    cam.view = look_at_rh(cam_pos, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f))
    cam.proj = perspective_vk(60.0f * PI / 180.0f, float(INSTANCE_W) / float(INSTANCE_H), 0.1f, 60.0f)
    cam.cam_time = float4(cam_pos.x, cam_pos.y, cam_pos.z, time)
    instancing_vs_bind_uniform(device, res.ubo.memory)
}

//! Record the instanced draw into the given command buffer. The render pass ends with the color
//! attachment in TRANSFER_SRC_OPTIMAL (boost default), so the windowed driver can blit straight
//! onto the swap target.
def public record_instancing_render_pass(res : InstancingResources; cmd : CommandBuffer) {
    var clears <- [clear_color(0.02f, 0.0f, 0.04f, 1.0f), clear_depth(1.0f)]
    record_render_pass(cmd, res.render_pass, res.framebuffer, full_area(INSTANCE_W, INSTANCE_H), clears) {
        cmd_bind_pipeline(cmd, res.pipeline)
        let sets <- [vk_value_to_boost(res.desc_set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.pipe_layout, 0u, sets, no_dyn)
        // bind binding 0 (per-vertex) and binding 1 (per-instance) in one call
        let vbufs <- [weak_copy(res.vb.buffer), weak_copy(res.inst_buf.buffer)]
        var voffs <- [0ul, 0ul]
        cmd_bind_vertex_buffers(cmd, 0u, vbufs, voffs)
        cmd_bind_index_buffer(cmd, weak_copy(res.ib.buffer), 0ul, VkIndexType.UINT16)
        // the instancing call: 36 indices fanned over N_INSTANCES instances
        cmd_draw_indexed(cmd, uint(N_INDICES), uint(N_INSTANCES), 0u, 0, 0u)
    }
    delete clears
}

//! Per-frame work: update UBO, record + submit the instanced draw, copy the colour attachment to
//! the readback buffer, clone out as RGBA8.
def public render_instancing_frame(var ctx : InstancingContext; time, camera_t : float) : array<uint8> {
    update_instancing_uniforms(ctx.res, ctx.device, time, camera_t)
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_instancing_render_pass(ctx.res, cmd)
        copy_image_to_buffer(cmd, ctx.res.color.image, ctx.res.readback, INSTANCE_W, INSTANCE_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! One-shot: build a context and render one frame. The test uses this; the recording driver uses
//! build_instancing_context + render_instancing_frame in a loop.
def public render_instance_swarm(time, camera_t : float) : array<uint8> {
    var inscope ctx <- build_instancing_context()
    return <- render_instancing_frame(ctx, time, camera_t)
}

//! Sample the RGB of pixel (x,y) from an INSTANCE_W*INSTANCE_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * INSTANCE_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
