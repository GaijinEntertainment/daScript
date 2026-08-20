options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 07 render path: a compute-driven particle swarm. One ``VkBuffer`` (created with
// USAGE_STORAGE_BUFFER | USAGE_VERTEX_BUFFER) holds an ``array<Particle>`` (std430, 32 bytes per
// element). Each frame the compute pipeline rewrites every particle's pos + vel via 1/r^2 gravity
// pulled toward the origin; a per-frame execution + memory barrier hands the buffer over to the
// graphics pipeline, which binds it as a per-instance vertex stream and billboards a small quad per
// particle.

module particles_tut

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require particles_tut_shaders
require math

let public PART_W = 768
let public PART_H = 768
let PART_DEPTH_FMT = VkFormat.D32_SFLOAT
let PART_COLOR_FMT = VkFormat.R8G8B8A8_SRGB   // sRGB: gamma-encode the linear output on write (recording readback + window blit match)

//! 1024 particles -- exactly 16 compute workgroups of local_size_x=64, no edge-case bounds check
//! needed in the compute shader.
let public N_PARTICLES = 1024
let PARTICLE_STRIDE = 32u           // std430 size of struct Particle (pos@0 + life@12 + vel@16 + pad@28)
let PARTICLE_FLOATS = 8             // 32 bytes / 4

// ===== matrix builders (column-major, 16 floats each) =====

def perspective_vk(fov_y, aspect, znear, zfar : float) : float4x4 {
    let f = 1.0f / tan(fov_y * 0.5f)
    let nf = 1.0f / (znear - zfar)
    var m : float4x4
    m[0] = float4(f / aspect, 0.0f, 0.0f, 0.0f)
    m[1] = float4(0.0f, -f, 0.0f, 0.0f)
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

// ===== particle init =====

//! Seed N_PARTICLES on a tilted ring of radius ~1.2 with tangential velocity. The orbit decays slowly
//! under the gravity-toward-origin compute integrator, but stays bouncy for any practical recording
//! length -- and the initial ring is instructive: you can watch the integrator perturb it over time.
def gen_initial_particles(n : int) : array<float> {
    var data : array<float>
    data |> resize(n * PARTICLE_FLOATS)
    let speed = 1.4f
    for (i in range(n)) {
        let fi = float(i) / float(n)
        let theta = fi * 2.0f * PI * 1.0f
        // tilted ring: radius 1.2, slight vertical oscillation per-particle
        let r = 1.2f + 0.15f * sin(fi * 2.0f * PI * 7.0f)
        let x = r * cos(theta)
        let y = sin(fi * 2.0f * PI * 5.0f) * 0.3f
        let z = r * sin(theta)
        // tangential velocity in the xz plane (anti-clockwise from +Y), plus a vertical kick
        let vx = -speed * sin(theta)
        let vy = cos(fi * 2.0f * PI * 3.0f) * 0.35f
        let vz = speed * cos(theta)
        let life = 0.4f + 0.6f * fi             // life is just a colour driver, never decremented
        let base = i * PARTICLE_FLOATS
        data[base + 0] = x                       // pos
        data[base + 1] = y
        data[base + 2] = z
        data[base + 3] = life
        data[base + 4] = vx                      // vel
        data[base + 5] = vy
        data[base + 6] = vz
        data[base + 7] = 0.0f                    // pad
    }
    return <- data
}

// ===== host helpers (local) =====

//! POINT_LIST graphics pipeline (one vertex per point) -- v3d hardcodes TRIANGLE_LIST, so this tutorial
//! inlines its own. Same rasteriser/depth defaults as v3d (depth test + write on, LESS_OR_EQUAL), but
//! cull mode is NONE (points have no winding) and the topology draws one screen-space point per vertex.
def private build_points_pipeline(device : Device; render_pass : RenderPass; layout : PipelineLayout;   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
                                  vert : ShaderModule; frag : ShaderModule; width, height : int;
                                  var vertex_bindings : array<VkVertexInputBindingDescription>;
                                  var vertex_attributes : array<VkVertexInputAttributeDescription>) : Pipeline {
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(2)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.vertex = true
    stages[0].module_ = boost_value_to_vk(vert)
    stages[0].pName = "main"
    stages[1] = VkPipelineShaderStageCreateInfo()
    stages[1].stage.fragment = true
    stages[1].module_ = boost_value_to_vk(frag)
    stages[1].pName = "main"

    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = uint(length(vertex_bindings))
    vinput.vertexAttributeDescriptionCount = uint(length(vertex_attributes))
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.POINT_LIST                  // <-- the headline rasteriser choice

    var viewport : VkViewport
    viewport.width = float(width)
    viewport.height = float(height)
    viewport.maxDepth = 1.0f
    var scissor : VkRect2D
    scissor.extent.width = uint(width)
    scissor.extent.height = uint(height)
    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    // cullMode left as NONE -- a single-pixel point has no face to cull
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    depthstate.depthTestEnable = VK_TRUE
    depthstate.depthWriteEnable = VK_FALSE                          // particles read depth (occlusion) but
    depthstate.depthCompareOp = VkCompareOp.LESS_OR_EQUAL            // never write it, so overlapping splats blend
    depthstate.minDepthBounds = 0.0f
    depthstate.maxDepthBounds = 1.0f

    // Additive blend (ONE/ONE) of premultiplied RGBA: overlapping splats stack into bright core+halo hot spots, isolated ones fade into the clear colour at the rim.
    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.blendEnable = VK_TRUE
    blend_att.srcColorBlendFactor = VkBlendFactor.ONE
    blend_att.dstColorBlendFactor = VkBlendFactor.ONE
    blend_att.colorBlendOp = VkBlendOp.ADD
    blend_att.srcAlphaBlendFactor = VkBlendFactor.ONE
    blend_att.dstAlphaBlendFactor = VkBlendFactor.ONE
    blend_att.alphaBlendOp = VkBlendOp.ADD
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    gp.layout = boost_value_to_vk(layout)
    gp.renderPass = boost_value_to_vk(render_pass)
    var h : VkPipeline
    unsafe {
        if (!empty(vertex_bindings)) {
            vinput.pVertexBindingDescriptions = addr(vertex_bindings[0])
        }
        if (!empty(vertex_attributes)) {
            vinput.pVertexAttributeDescriptions = addr(vertex_attributes[0])
        }
        vpstate.pViewports = addr(viewport)
        vpstate.pScissors = addr(scissor)
        blend.pAttachments = addr(blend_att)
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depthstate)
        gp.pColorBlendState = addr(blend)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    return <- vk_value_to_boost(h)
}


// ===== reusable per-frame context =====

//! Long-lived particle state: offscreen targets, render pass + framebuffer, quad VB + IB, particle
//! buffer (shared between compute SSBO and graphics per-instance stream), the graphics UBO, both
//! pipelines + their layouts + descriptor sets, and a readback buffer.
struct public ParticlesResources {
    color           : OffscreenTarget
    depth           : OffscreenDepth
    render_pass     : RenderPass
    framebuffer     : Framebuffer

    particle_buf    : HostBuffer        // SSBO for compute, per-instance vertex for graphics
    ubo             : HostBuffer

    // compute side
    cset_layouts    : array<DescriptorSetLayout>
    cdesc_pool      : DescriptorPool
    cdesc_set       : VkDescriptorSet
    cpipe_layout    : PipelineLayout
    cpipeline       : Pipeline

    // graphics side
    gset_layouts    : array<DescriptorSetLayout>
    gdesc_pool      : DescriptorPool
    gdesc_set       : VkDescriptorSet
    gpipe_layout    : PipelineLayout
    gpipeline       : Pipeline

    readback        : HostBuffer
    buf_size        : uint64
}

def public finalize(var r : ParticlesResources) {
    unsafe {
        delete r.readback
        delete r.gpipeline
        delete r.gpipe_layout
        delete r.gdesc_pool
        delete r.gset_layouts
        delete r.cpipeline
        delete r.cpipe_layout
        delete r.cdesc_pool
        delete r.cset_layouts
        delete r.ubo
        delete r.particle_buf
        delete r.framebuffer
        delete r.render_pass
        delete r.depth
        delete r.color
    }
}

struct public ParticlesContext {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : ParticlesResources
}

def public finalize(var ctx : ParticlesContext) {
    unsafe {
        delete ctx.res
        delete ctx.pool
        delete ctx.device
        delete ctx.instance
    }
}

//! Build the offscreen targets, the two pipelines (compute + graphics), and the once-seeded particle
//! buffer against a caller-owned (device, phys, queue, pool). That buffer carries STORAGE_BUFFER |
//! VERTEX_BUFFER usage, so the SAME VkBuffer binds as a compute SSBO and as a vertex stream.
def public build_particles_resources(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool) : ParticlesResources {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    let _q = queue
    let _p = pool
    let dev = boost_value_to_vk(device)

    var inscope color <- build_offscreen_target(device, phys, PART_W, PART_H, PART_COLOR_FMT)
    var inscope depth <- build_offscreen_depth(device, phys, PART_W, PART_H, PART_DEPTH_FMT)
    var inscope render_pass <- create_render_pass_color_depth(device, PART_COLOR_FMT, PART_DEPTH_FMT)
    var inscope framebuffer <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(color.view), weak_copy(depth.view)],
        width = uint(PART_W),
        height = uint(PART_H),
        layers = 1u))

    // SHARED particle buffer: storage_buffer for the compute SSBO + vertex_buffer for the graphics
    // per-instance binding. HOST_VISIBLE so the initial state can be uploaded without a staging
    // buffer; HOST_COHERENT so the upload completes before the first dispatch.
    var part_usage : VkBufferUsageFlags
    part_usage.storage_buffer = true
    part_usage.vertex_buffer = true
    var part_seed <- gen_initial_particles(N_PARTICLES)
    var inscope particle_buf <- create_host_buffer_from_bytes(device, phys, part_usage, part_seed)
    delete part_seed

    // graphics UBO: view (64) + proj (64) = 128 bytes
    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo <- create_host_buffer(device, phys, 128ul, ubo_usage)

    // ===== compute pipeline + descriptors =====
    var creflections <- [decode_reflection(particles_compute_spv_reflect)]
    var inscope cset_layouts <- build_descriptor_set_layouts(device, creflections)

    var cdpci = DescriptorPoolCreateInfo(
        maxSets = 1u,
        pPoolSizes <- [DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)])
    var inscope cdesc_pool <- create_descriptor_pool(device, cdpci)

    var cdsai = VkDescriptorSetAllocateInfo()
    cdsai.descriptorPool = boost_value_to_vk(cdesc_pool)
    cdsai.descriptorSetCount = 1u
    var craw_layout = boost_value_to_vk(cset_layouts[0])
    var craw_set : VkDescriptorSet
    unsafe {
        cdsai.pSetLayouts = addr(craw_layout)
        vk_check(vkAllocateDescriptorSets(dev, cdsai, addr(craw_set)), null)
    }

    var cwrites : array<WriteDescriptorSet>
    var cw = WriteDescriptorSet(
        dstSet = vk_value_to_boost(craw_set),
        dstBinding = 0u,
        descriptorType = VkDescriptorType.STORAGE_BUFFER,
        descriptorCount = 1u)
    cw.pBufferInfo |> push(DescriptorBufferInfo(
        buffer = weak_copy(particle_buf.buffer),
        range_ = uint64(N_PARTICLES) * uint64(PARTICLE_STRIDE)))
    cwrites |> emplace(cw)
    var no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, cwrites, no_copies)

    var inscope cpipe_layout <- build_pipeline_layout(device, cset_layouts, creflections)
    var inscope cshader <- create_shader_module(device, particles_compute_spv)
    var inscope cpipeline <- create_compute_pipeline(device, cpipe_layout, cshader)
    delete creflections

    // ===== graphics pipeline + descriptors =====
    var greflections <- [decode_reflection(particles_vert_spv_reflect), decode_reflection(particles_frag_spv_reflect)]
    var inscope gset_layouts <- build_descriptor_set_layouts(device, greflections)

    var gdpci = DescriptorPoolCreateInfo(
        maxSets = 1u,
        pPoolSizes <- [DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)])
    var inscope gdesc_pool <- create_descriptor_pool(device, gdpci)

    var gdsai = VkDescriptorSetAllocateInfo()
    gdsai.descriptorPool = boost_value_to_vk(gdesc_pool)
    gdsai.descriptorSetCount = 1u
    var graw_layout = boost_value_to_vk(gset_layouts[0])
    var graw_set : VkDescriptorSet
    unsafe {
        gdsai.pSetLayouts = addr(graw_layout)
        vk_check(vkAllocateDescriptorSets(dev, gdsai, addr(graw_set)), null)
    }

    var gwrites : array<WriteDescriptorSet>
    var gw = WriteDescriptorSet(
        dstSet = vk_value_to_boost(graw_set),
        dstBinding = 0u,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER,
        descriptorCount = 1u)
    gw.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(ubo.buffer), range_ = 128ul))
    gwrites |> emplace(gw)
    update_descriptor_sets(device, gwrites, no_copies)
    delete no_copies

    var inscope gpipe_layout <- build_pipeline_layout(device, gset_layouts, greflections)

    var inscope vert <- create_shader_module(device, particles_vert_spv)
    var inscope frag <- create_shader_module(device, particles_frag_spv)

    // Single vertex binding -- the SAME bytes the compute shader wrote to the SSBO are re-read
    // here as a per-vertex stream (POINT_LIST topology so each particle is one draw vertex).
    var vbindings : array<VkVertexInputBindingDescription>
    var vb0_desc : VkVertexInputBindingDescription
    vb0_desc.binding = 0u
    vb0_desc.stride = PARTICLE_STRIDE                     // SAME 32 byte layout as the Particle struct
    vb0_desc.inputRate = VkVertexInputRate.VERTEX
    vbindings |> push(vb0_desc)

    var vattrs : array<VkVertexInputAttributeDescription>
    var a_pos : VkVertexInputAttributeDescription
    a_pos.location = 0u
    a_pos.binding = 0u
    a_pos.format = VkFormat.R32G32B32_SFLOAT
    a_pos.offset = 0u                                     // pos at struct offset 0
    vattrs |> push(a_pos)
    var a_life : VkVertexInputAttributeDescription
    a_life.location = 1u
    a_life.binding = 0u
    a_life.format = VkFormat.R32_SFLOAT
    a_life.offset = 12u                                   // life at struct offset 12
    vattrs |> push(a_life)
    var a_vel : VkVertexInputAttributeDescription
    a_vel.location = 2u
    a_vel.binding = 0u
    a_vel.format = VkFormat.R32G32B32_SFLOAT
    a_vel.offset = 16u                                    // vel at struct offset 16
    vattrs |> push(a_vel)

    var inscope gpipeline <- build_points_pipeline(device, render_pass, gpipe_layout, vert, frag,
        PART_W, PART_H, vbindings, vattrs)
    delete vbindings
    delete vattrs
    delete greflections

    let buf_size = uint64(PART_W * PART_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    return <- ParticlesResources(
        color <- color, depth <- depth, render_pass <- render_pass, framebuffer <- framebuffer,
        particle_buf <- particle_buf, ubo <- ubo,
        cset_layouts <- cset_layouts, cdesc_pool <- cdesc_pool, cdesc_set = craw_set,
        cpipe_layout <- cpipe_layout, cpipeline <- cpipeline,
        gset_layouts <- gset_layouts, gdesc_pool <- gdesc_pool, gdesc_set = graw_set,
        gpipe_layout <- gpipe_layout, gpipeline <- gpipeline,
        readback <- readback, buf_size = buf_size)
}

//! Self-contained one-shot context (own instance + device + queue + pool + resources).
def public build_particles_context() : ParticlesContext {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 07", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    let no_ext : array<string>
    var features : VkPhysicalDeviceFeatures
    features.largePoints = VK_TRUE                   // gl_PointSize > 1 (POINT_LIST splat width)
    var inscope device <- create_device(phys, gfx, no_ext, features)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope res <- build_particles_resources(device, phys, queue, pool)
    return <- ParticlesContext(
        instance <- instance, device <- device, queue = queue, pool <- pool, res <- res)
}

//! Host-side per-frame prep: derive the camera matrices from ``camera_t in [0,1)``, pack the
//! camera-space right + up axes into the UBO so the vertex shader can billboard each particle, and
//! upload through the bind_uniform helper.
def public update_particles_uniforms(res : ParticlesResources; device : Device; time, camera_t : float) {
    let _t = time
    let camera_angle = camera_t * 2.0f * PI
    let cam_r = 3.0f
    let cam_x = cam_r * cos(camera_angle)
    let cam_z = cam_r * sin(camera_angle)
    let cam_y = sin(camera_angle * 2.0f) * 0.5f
    let cam_pos = float3(cam_x, cam_y, cam_z)
    cam.view = look_at_rh(cam_pos, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f))
    cam.proj = perspective_vk(60.0f * PI / 180.0f, float(PART_W) / float(PART_H), 0.1f, 30.0f)
    particles_vs_bind_uniform(device, res.ubo.memory)
}

//! Record the compute dispatch + the compute->graphics barrier + the points draw into
//! the given command buffer.
def public record_particles_render_pass(res : ParticlesResources; cmd : CommandBuffer; time, dt : float) {
    let raw_cmd = boost_value_to_vk(cmd)

    // 1) Compute pass: dispatch the integrator over all N_PARTICLES particles.
    cmd_bind_pipeline(cmd, res.cpipeline, VkPipelineBindPoint.COMPUTE)
    let csets <- [vk_value_to_boost(res.cdesc_set)]
    var no_cdyn : array<uint>
    cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.COMPUTE, res.cpipe_layout, 0u, csets, no_cdyn)
    var cpc = ComputePush(time = time, dt = dt)
    var cstage : VkShaderStageFlags
    cstage.compute = true
    cmd_push_constants(cmd, res.cpipe_layout, cstage, 0u, cpc)
    cmd_dispatch(cmd, uint(N_PARTICLES / 64), 1u, 1u)

    // 2) Compute -> graphics barrier: the integrator's last write (SHADER_WRITE in COMPUTE_SHADER) must
    //    be visible to the vertex-input fetch (VERTEX_ATTRIBUTE_READ in VERTEX_INPUT) before graphics
    //    reads particles[i].pos. Without it the graphics pipeline races compute and reads stale values.
    var bmb : VkBufferMemoryBarrier
    bmb.srcAccessMask.shader_write = true
    bmb.dstAccessMask.vertex_attribute_read = true
    bmb.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
    bmb.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
    bmb.buffer = boost_value_to_vk(res.particle_buf.buffer)
    bmb.offset = 0ul
    bmb.size = uint64(N_PARTICLES) * uint64(PARTICLE_STRIDE)
    var src_stage : VkPipelineStageFlags
    src_stage.compute_shader = true
    var dst_stage : VkPipelineStageFlags
    dst_stage.vertex_input = true
    var bmbs : array<VkBufferMemoryBarrier>
    bmbs |> push(bmb)
    let no_dep : VkDependencyFlags
    unsafe {
        vkCmdPipelineBarrier(raw_cmd, src_stage, dst_stage,
            no_dep,
            0u, null,
            1u, addr(bmbs[0]),
            0u, null)
    }
    delete bmbs

    // 3) Graphics pass: bind the SAME particle buffer (the bytes the compute just wrote) at
    //    binding 0 as a per-vertex stream, draw N_PARTICLES points (POINT_LIST topology).
    var clears <- [clear_color(0.01f, 0.0f, 0.03f, 1.0f), clear_depth(1.0f)]
    record_render_pass(cmd, res.render_pass, res.framebuffer, full_area(PART_W, PART_H), clears) {
        cmd_bind_pipeline(cmd, res.gpipeline)
        let gsets <- [vk_value_to_boost(res.gdesc_set)]
        var no_gdyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.gpipe_layout, 0u, gsets, no_gdyn)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.particle_buf.buffer))
        cmd_draw(cmd, uint(N_PARTICLES))
    }
    delete clears
}

//! Per-frame work: update UBO, compute + barrier + draw, copy the colour attachment to readback,
//! clone out RGBA8.
def public render_particles_frame(var ctx : ParticlesContext; time, dt, camera_t : float) : array<uint8> {
    update_particles_uniforms(ctx.res, ctx.device, time, camera_t)
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_particles_render_pass(ctx.res, cmd, time, dt)
        copy_image_to_buffer(cmd, ctx.res.color.image, ctx.res.readback, PART_W, PART_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! One-shot: build a context and render one frame at (time=t, camera_t). dt=1/30 so the integrator
//! does a 30-fps tick before the draw.
def public render_particle_swarm(time, camera_t : float) : array<uint8> {
    var inscope ctx <- build_particles_context()
    return <- render_particles_frame(ctx, time, 1.0f / 30.0f, camera_t)
}

//! Sample the RGB of pixel (x,y) from a PART_W*PART_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * PART_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
