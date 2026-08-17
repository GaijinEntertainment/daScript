options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 14 render path: the GPU-tessellated Utah teapot. No vertex buffer, no index buffer -- a
// single cmd_draw_mesh_tasks_ext dispatches one task workgroup that selects the surviving Bezier
// patches and dispatches a mesh workgroup per patch; each mesh workgroup evaluates its bicubic patch
// on a 7x7 grid (on-GPU tessellation) and emits the triangles. A depth attachment resolves the
// non-convex teapot's self-occlusion. The CI gate (test_teapot.das) asserts the teapot silhouette is
// lit at the center and the corners are background.

module teapot_tut

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require teapot_tut_shaders
require teapot_data
require math

let public TEAPOT_W = 512
let public TEAPOT_H = 512
let TEAPOT_COLOR_FMT = VkFormat.R8G8B8A8_SRGB   // sRGB: gamma-encode the linear output on write (recording readback + window blit match)
let TEAPOT_DEPTH_FMT = VkFormat.D32_SFLOAT
// std140: mvp(64)+model(64)+vp(64)+light_vp(64)+camera(16)+light(16)+cp[512]*16(8192)
let TEAPOT_UBO_SIZE = 8480ul

// Shared scene constants — the per-frame matrices AND the baked sky corner-rays must use the same
// camera, so they live here as the single source of truth.
let CAM_EYE    = float3(0.0f, 0.55f, 3.8f)
let CAM_TARGET = float3(0.0f, 0.05f, 0.0f)
let CAM_UP     = float3(0.0f, 1.0f, 0.0f)
let CAM_FOV_Y  = 45.0f * PI / 180.0f
let LIGHT_DIR  = float3(0.4f, 0.9f, 0.5f)   // un-normalized; normalized where used (the sun)
let FLOOR_Y    = -0.62f                      // the teapot's base sits here; the ground plane lives at this y
let FLOOR_EXT  = 16.0f                        // half-extent of the ground quad in X and Z
let SHADOW_W   = 1024
let SHADOW_H   = 1024
let SHADOW_FMT = VkFormat.D32_SFLOAT          // the shadow map: depth attachment in pass 1, sampled in pass 2

// ===== matrix helpers (column-major, m[col] = column vector) =====

def perspective_vk(fov_y, aspect, znear, zfar : float) : float4x4 {
    let f = 1.0f / tan(fov_y * 0.5f)
    let nf = 1.0f / (znear - zfar)
    var m : float4x4
    m[0] = float4(f / aspect, 0.0f, 0.0f, 0.0f)
    m[1] = float4(0.0f, -f, 0.0f, 0.0f)                 // Vulkan NDC has +Y down
    m[2] = float4(0.0f, 0.0f, zfar * nf, -1.0f)
    m[3] = float4(0.0f, 0.0f, znear * zfar * nf, 0.0f)
    return m
}

//! Orthographic projection for the directional-light shadow pass. Vulkan Z in [0,1]; Y not flipped
//! because the light's projection is sampled, not displayed.
def ortho_vk(left, right, bottom, top, znear, zfar : float) : float4x4 {
    var m : float4x4
    m[0] = float4(2.0f / (right - left), 0.0f, 0.0f, 0.0f)
    m[1] = float4(0.0f, 2.0f / (top - bottom), 0.0f, 0.0f)
    m[2] = float4(0.0f, 0.0f, -1.0f / (zfar - znear), 0.0f)
    m[3] = float4(-(right + left) / (right - left),
                  -(top + bottom) / (top - bottom),
                  -znear / (zfar - znear), 1.0f)
    return m
}

def v3_sub(a, b : float3) : float3 => float3(a.x - b.x, a.y - b.y, a.z - b.z)
def v3_cross(a, b : float3) : float3 => float3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x)
def v3_dot(a, b : float3) : float => a.x * b.x + a.y * b.y + a.z * b.z
def v3_norm(v : float3) : float3 {
    let len = sqrt(v3_dot(v, v))
    return float3(v.x / len, v.y / len, v.z / len)
}

def look_at_rh(eye, center, up : float3) : float4x4 {
    let f = v3_norm(v3_sub(center, eye))
    let s = v3_norm(v3_cross(f, up))
    let u = v3_cross(s, f)
    var m : float4x4
    m[0] = float4(s.x, u.x, -f.x, 0.0f)
    m[1] = float4(s.y, u.y, -f.y, 0.0f)
    m[2] = float4(s.z, u.z, -f.z, 0.0f)
    m[3] = float4(-v3_dot(s, eye), -v3_dot(u, eye), v3_dot(f, eye), 1.0f)
    return m
}

def mat_identity : float4x4 {
    var m : float4x4
    m[0] = float4(1.0f, 0.0f, 0.0f, 0.0f)
    m[1] = float4(0.0f, 1.0f, 0.0f, 0.0f)
    m[2] = float4(0.0f, 0.0f, 1.0f, 0.0f)
    m[3] = float4(0.0f, 0.0f, 0.0f, 1.0f)
    return m
}
def mat_translate(t : float3) : float4x4 {
    var m = mat_identity()
    m[3] = float4(t.x, t.y, t.z, 1.0f)
    return m
}
def mat_scale(s : float) : float4x4 {
    var m = mat_identity()
    m[0].x = s; m[1].y = s; m[2].z = s
    return m
}
def mat_rot_x(a : float) : float4x4 {
    let c = cos(a); let s = sin(a)
    var m = mat_identity()
    m[1] = float4(0.0f, c, s, 0.0f)
    m[2] = float4(0.0f, -s, c, 0.0f)
    return m
}
def mat_rot_y(a : float) : float4x4 {
    let c = cos(a); let s = sin(a)
    var m = mat_identity()
    m[0] = float4(c, 0.0f, -s, 0.0f)
    m[2] = float4(s, 0.0f, c, 0.0f)
    return m
}

//! Ground quad in world space: an XZ plane at y=FLOOR_Y, +-FLOOR_EXT in X and Z, as two triangles
//! (6 vertices, position-only). Wound so the up-face is front-facing under the v3d pipeline's
//! back-cull + CCW-front (verified against the rendered image).
def build_floor_vertices() : array<float> {
    let e = FLOOR_EXT
    let y = FLOOR_Y
    return <- [
        -e, y, -e,    e, y,  e,    e, y, -e,    // tri 1
        -e, y, -e,   -e, y,  e,    e, y,  e     // tri 2
    ]
}

//! Fullscreen triangle for the env sky: 3 vertices of (clip.xy, ray.xyz). Corners (-1,-1),(-1,3),(3,-1)
//! cover the screen with one triangle, front-facing under back-cull. Each corner's world-space view ray
//! comes from the camera basis, so the fragment evaluates env_color in the frame teapot + floor reflect.
def build_sky_vertices() : array<float> {
    let f = v3_norm(v3_sub(CAM_TARGET, CAM_EYE))      // forward
    let r = v3_norm(v3_cross(f, CAM_UP))              // right
    let u = v3_cross(r, f)                            // recomputed up
    let t = tan(CAM_FOV_Y * 0.5f)
    let aspect = float(TEAPOT_W) / float(TEAPOT_H)
    // ray(nx,ny) = f + nx*t*aspect*r - ny*t*u
    let ray_a = f + (-t * aspect) * r + (t) * u       // clip (-1,-1)
    let ray_c = f + (-t * aspect) * r + (-3.0f * t) * u   // clip (-1, 3)
    let ray_b = f + (3.0f * t * aspect) * r + (t) * u     // clip ( 3,-1)
    return <- [
        -1.0f, -1.0f,  ray_a.x, ray_a.y, ray_a.z,
        -1.0f,  3.0f,  ray_c.x, ray_c.y, ray_c.z,
         3.0f, -1.0f,  ray_b.x, ray_b.y, ray_b.z
    ]
}

//! Fit the raw Newell teapot into a unit-ish, Y-up, origin-centered model and spin it about the
//! vertical axis by `angle`. Order (applied right-to-left to a model point): center -> scale ->
//! reorient Z-up to Y-up -> spin.
def teapot_model(angle : float) : float4x4 {
    let fit = mat_scale(2.0f / TEAPOT_EXTENT)
    let center = mat_translate(float3(-TEAPOT_CENTER.x, -TEAPOT_CENTER.y, -TEAPOT_CENTER.z))
    let reorient = mat_rot_x(-PI * 0.5f)              // Newell Z-up -> Y-up
    let spin = mat_rot_y(angle)
    return spin * reorient * fit * center
}

// ===== reusable resources =====

struct public TeapotResources {
    color         : OffscreenTarget
    depth         : OffscreenDepth
    shadow_map    : OffscreenDepth        // D32: depth attachment in pass 1, sampler2DShadow in pass 2
    render_pass   : RenderPass
    rp_shadow     : RenderPass            // depth-only pass for the shadow map
    framebuffer   : Framebuffer
    fb_shadow     : Framebuffer
    ubo           : HostBuffer
    floor_vb      : HostBuffer            // ground quad (world-space positions)
    sky_vb        : HostBuffer            // fullscreen env-sky triangle (clip.xy + ray.xyz)
    shadow_sampler : Sampler             // depth-compare sampler for the shadow lookup
    set_layouts   : array<DescriptorSetLayout>
    desc_pool     : DescriptorPool
    desc_set      : VkDescriptorSet
    pipe_layout   : PipelineLayout
    pipeline      : Pipeline              // mesh-shader teapot
    pipeline_floor : Pipeline             // ground plane (vertex+fragment)
    pipeline_sky   : Pipeline             // env background (vertex+fragment, depth off)
    pipeline_shadow : Pipeline            // mesh-shader teapot, depth-only, from the light's POV
    readback      : HostBuffer
    buf_size      : uint64
}

def public finalize(var r : TeapotResources) {
    unsafe {
        delete r.readback
        delete r.pipeline_shadow
        delete r.pipeline_sky
        delete r.pipeline_floor
        delete r.pipeline
        delete r.pipe_layout
        delete r.desc_pool
        delete r.set_layouts
        delete r.shadow_sampler
        delete r.sky_vb
        delete r.floor_vb
        delete r.ubo
        delete r.fb_shadow
        delete r.framebuffer
        delete r.rp_shadow
        delete r.render_pass
        delete r.shadow_map
        delete r.depth
        delete r.color
    }
}

struct public TeapotContext {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : TeapotResources
}

def public finalize(var ctx : TeapotContext) {
    unsafe {
        delete ctx.res
        delete ctx.pool
        delete ctx.device
        delete ctx.instance
    }
}

//! Depth-only mesh-shader pipeline for the shadow pass: task + mesh stages, NO fragment, NO color
//! attachment (mesh pipelines ignore vertex-input + input-assembly). Slope-scaled depth bias stops the
//! teapot self-shadowing into acne. Hand-rolled: create_mesh_pipeline always adds fragment + blend.
def private build_mesh_shadow_pipeline(device : Device; rp : RenderPass; layout : PipelineLayout;
                                       task, mesh : ShaderModule) : Pipeline {
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(2)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.task_ext = true
    stages[0].module_ = boost_value_to_vk(task)
    stages[0].pName = "main"
    stages[1] = VkPipelineShaderStageCreateInfo()
    stages[1].stage.mesh_ext = true
    stages[1].module_ = boost_value_to_vk(mesh)
    stages[1].pName = "main"

    var viewport : VkViewport
    viewport.width = float(SHADOW_W)
    viewport.height = float(SHADOW_H)
    viewport.maxDepth = 1.0f
    var scissor : VkRect2D
    scissor.extent.width = uint(SHADOW_W)
    scissor.extent.height = uint(SHADOW_H)
    var vp = VkPipelineViewportStateCreateInfo()
    vp.viewportCount = 1u
    vp.scissorCount = 1u

    var rast = VkPipelineRasterizationStateCreateInfo()
    rast.polygonMode = VkPolygonMode.FILL
    rast.frontFace = VkFrontFace.COUNTER_CLOCKWISE       // cullMode left NONE — the Newell patches are
    rast.depthBiasEnable = VK_TRUE                        // not consistently wound; render both sides for
    rast.depthBiasConstantFactor = 1.5f                  // a solid depth silhouette, slope-bias for acne.
    rast.depthBiasSlopeFactor = 3.0f
    rast.lineWidth = 1.0f

    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    depthstate.depthTestEnable = VK_TRUE
    depthstate.depthWriteEnable = VK_TRUE
    depthstate.depthCompareOp = VkCompareOp.LESS
    depthstate.maxDepthBounds = 1.0f

    var blend = VkPipelineColorBlendStateCreateInfo()    // attachmentCount = 0 (no color attachments)

    var h : VkPipeline
    unsafe {
        var gp = VkGraphicsPipelineCreateInfo()
        gp.stageCount = 2u
        gp.pStages = addr(stages[0])
        // pVertexInputState + pInputAssemblyState deliberately null — ignored for mesh pipelines.
        vp.pViewports = addr(viewport)
        vp.pScissors = addr(scissor)
        gp.pViewportState = addr(vp)
        gp.pRasterizationState = addr(rast)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depthstate)
        gp.pColorBlendState = addr(blend)
        gp.layout = boost_value_to_vk(layout)
        gp.renderPass = boost_value_to_vk(rp)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete stages
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! Build the teapot render resources against a caller-owned (device, phys, queue, pool). Uploads the
//! Newell control net into the UBO's cp[] once; the per-frame transform is restamped each frame.
def public build_teapot_resources(device : Device; phys : VkPhysicalDevice) : TeapotResources {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    var inscope color <- build_offscreen_target(device, phys, TEAPOT_W, TEAPOT_H, TEAPOT_COLOR_FMT)
    var inscope depth <- build_offscreen_depth(device, phys, TEAPOT_W, TEAPOT_H, TEAPOT_DEPTH_FMT)
    var inscope render_pass <- create_render_pass_color_depth(device, TEAPOT_COLOR_FMT, TEAPOT_DEPTH_FMT)
    var inscope framebuffer <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(color.view), weak_copy(depth.view)],
        width = uint(TEAPOT_W),
        height = uint(TEAPOT_H),
        layers = 1u))

    // Shadow map: a D32 depth image used as the depth attachment in pass 1, then sampled as a
    // sampler2DShadow in pass 2. `sampled` usage lets the floor fragment bind it. The depth-only pass's
    // finalLayout = DEPTH_STENCIL_READ_ONLY_OPTIMAL hands it to the color pass ready to sample.
    var sm_usage : VkImageUsageFlags
    sm_usage.sampled = true
    var inscope shadow_map <- build_offscreen_depth(device, phys, SHADOW_W, SHADOW_H, SHADOW_FMT, sm_usage)
    var inscope rp_shadow <- create_render_pass_depth_only(device, SHADOW_FMT)
    var inscope fb_shadow <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(rp_shadow),
        pAttachments <- [weak_copy(shadow_map.view)],
        width = uint(SHADOW_W), height = uint(SHADOW_H), layers = 1u))
    var inscope shadow_sampler <- create_sampler_shadow(device)

    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo_buf <- create_host_buffer(device, phys, TEAPOT_UBO_SIZE, ubo_usage)

    // upload the static control net into the UBO global's cp[] once (persists across frames)
    var cps <- teapot_control_points()
    for (i in range(length(cps))) {
        ubo.cp[i] = float4(cps[i].x, cps[i].y, cps[i].z, 0.0f)
    }
    delete cps

    // ground quad + env-sky triangle vertex buffers (static)
    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var fverts <- build_floor_vertices()
    var inscope floor_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, fverts)
    delete fverts
    var sverts <- build_sky_vertices()
    var inscope sky_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, sverts)
    delete sverts

    // Reflection-driven descriptor + pipeline layout. EVERY shader touching the shared UBO is reflected,
    // so binding 0 collects all stage flags (task|mesh|fragment for the teapot, vertex|fragment for floor
    // + sky) -- a fragment shader reading binding 0 off a vertex-only layout would just read zero.
    var reflections <- [decode_reflection(teapot_task_spv_reflect),
                        decode_reflection(teapot_mesh_spv_reflect),
                        decode_reflection(teapot_frag_spv_reflect),
                        decode_reflection(floor_vert_spv_reflect),
                        decode_reflection(floor_frag_spv_reflect),
                        decode_reflection(sky_vert_spv_reflect),
                        decode_reflection(sky_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    let ps0 = DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    dpci.pPoolSizes |> push(ps0)
    // binding 1 = the shadow-map sampler2DShadow
    let ps1 = DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    dpci.pPoolSizes |> push(ps1)
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
    var w0 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 0u,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    let binfo = DescriptorBufferInfo(buffer = weak_copy(ubo_buf.buffer), range_ = TEAPOT_UBO_SIZE)
    w0.pBufferInfo |> push(binfo)
    writes |> emplace(w0)

    // binding 1 = sampler2DShadow (the shadow map)
    var w1 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 1u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    let shadow_info = DescriptorImageInfo(sampler = weak_copy(shadow_sampler),
        imageView = weak_copy(shadow_map.view), imageLayout = VkImageLayout.DEPTH_STENCIL_READ_ONLY_OPTIMAL)
    w1.pImageInfo |> push(shadow_info)
    writes |> emplace(w1)

    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope task <- create_shader_module(device, teapot_task_spv)
    var inscope mesh <- create_shader_module(device, teapot_mesh_spv)
    var inscope frag <- create_shader_module(device, teapot_frag_spv)
    var inscope pipeline <- create_mesh_pipeline(device, render_pass, pipe_layout, task, mesh, frag,
        TEAPOT_W, TEAPOT_H, false, true)        // dynamic_viewport=false, enable_depth=true

    // ground-plane pipeline: pos-only vertex (stride 12), depth test + write on
    var inscope floor_vert <- create_shader_module(device, floor_vert_spv)
    var inscope floor_frag <- create_shader_module(device, floor_frag_spv)
    var floor_bind : array<VkVertexInputBindingDescription>
    var fb0 : VkVertexInputBindingDescription
    fb0.binding = 0u; fb0.stride = 12u; fb0.inputRate = VkVertexInputRate.VERTEX
    floor_bind |> push(fb0)
    var floor_attr : array<VkVertexInputAttributeDescription>
    var fa0 : VkVertexInputAttributeDescription
    fa0.location = 0u; fa0.binding = 0u; fa0.format = VkFormat.R32G32B32_SFLOAT; fa0.offset = 0u
    floor_attr |> push(fa0)
    var inscope pipeline_floor <- create_graphics_pipeline_v3d(device, render_pass, pipe_layout,
        floor_vert, floor_frag, TEAPOT_W, TEAPOT_H, floor_bind, floor_attr)
    delete floor_bind
    delete floor_attr

    // env-sky pipeline: clip.xy + ray.xyz vertex (stride 20), depth OFF (drawn first, fills background)
    var inscope sky_vert <- create_shader_module(device, sky_vert_spv)
    var inscope sky_frag <- create_shader_module(device, sky_frag_spv)
    var sky_bind : array<VkVertexInputBindingDescription>
    var sb0 : VkVertexInputBindingDescription
    sb0.binding = 0u; sb0.stride = 20u; sb0.inputRate = VkVertexInputRate.VERTEX
    sky_bind |> push(sb0)
    var sky_attr : array<VkVertexInputAttributeDescription>
    var sa0 : VkVertexInputAttributeDescription
    sa0.location = 0u; sa0.binding = 0u; sa0.format = VkFormat.R32G32_SFLOAT; sa0.offset = 0u
    sky_attr |> push(sa0)
    var sa1 : VkVertexInputAttributeDescription
    sa1.location = 1u; sa1.binding = 0u; sa1.format = VkFormat.R32G32B32_SFLOAT; sa1.offset = 8u
    sky_attr |> push(sa1)
    var inscope pipeline_sky <- create_graphics_pipeline_v3d(device, render_pass, pipe_layout,
        sky_vert, sky_frag, TEAPOT_W, TEAPOT_H, sky_bind, sky_attr, false, false)   // depth test/write off
    delete sky_bind
    delete sky_attr

    // shadow caster: the same on-GPU tessellation, depth-only, from the light's POV
    var inscope shadow_task <- create_shader_module(device, teapot_shadow_task_spv)
    var inscope shadow_mesh <- create_shader_module(device, teapot_shadow_mesh_spv)
    var inscope pipeline_shadow <- build_mesh_shadow_pipeline(device, rp_shadow, pipe_layout,
        shadow_task, shadow_mesh)

    let buf_size = uint64(TEAPOT_W * TEAPOT_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    return <- TeapotResources(
        color <- color, depth <- depth, shadow_map <- shadow_map,
        render_pass <- render_pass, rp_shadow <- rp_shadow,
        framebuffer <- framebuffer, fb_shadow <- fb_shadow,
        ubo <- ubo_buf, floor_vb <- floor_vb, sky_vb <- sky_vb, shadow_sampler <- shadow_sampler,
        set_layouts <- set_layouts, desc_pool <- desc_pool, desc_set = raw_set,
        pipe_layout <- pipe_layout, pipeline <- pipeline, pipeline_floor <- pipeline_floor,
        pipeline_sky <- pipeline_sky, pipeline_shadow <- pipeline_shadow,
        readback <- readback, buf_size = buf_size)
}

//! Probe whether this machine's Vulkan device supports VK_EXT_mesh_shader (extension + meshShader/
//! taskShader feature bits), so the pixel-oracle can skip cleanly on a driver that lacks it instead of
//! panicking in create_device_mesh_shader. Creates and tears down a throwaway instance + physical device.
def public mesh_shader_available() : bool {
    if (volkInitialize() != 0) {
        return false
    }
    var inscope instance <- create_instance("dasVulkan tutorial 14 probe", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    var count = 0u
    vkEnumeratePhysicalDevices(boost_value_to_vk(instance), count, null)
    if (count == 0u) {
        return false
    }
    let phys = select_physical_device(instance)
    return mesh_shader_supported(phys)
}

def public build_teapot_context() : TeapotContext {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 14", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device_mesh_shader(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope res <- build_teapot_resources(device, phys)
    return <- TeapotContext(
        instance <- instance, device <- device, queue = queue, pool <- pool, res <- res)
}

//! Per-frame uniform prep: spin the teapot by `time`, set the shared camera/floor VP, the light's
//! shadow VP, the camera + light + debug flag, then upload the whole UBO in one bind call.
def public update_teapot_uniforms(res : TeapotResources; device : Device; time : float; debug_tint : bool) {
    let model = teapot_model(time * 0.6f)
    let view = look_at_rh(CAM_EYE, CAM_TARGET, CAM_UP)
    let proj = perspective_vk(CAM_FOV_Y, float(TEAPOT_W) / float(TEAPOT_H), 0.1f, 100.0f)
    let vp = proj * view
    ubo.mvp = vp * model
    ubo.model = model
    ubo.vp = vp                          // floor uses proj*view directly (its verts are world-space)
    // directional-light VP for the shadow map: look at the scene from far along the sun direction.
    let light_dir = v3_norm(LIGHT_DIR)
    let light_eye = float3(light_dir.x * 6.0f, light_dir.y * 6.0f, light_dir.z * 6.0f)
    let light_view = look_at_rh(light_eye, float3(0.0f, FLOOR_Y + 0.4f, 0.0f), CAM_UP)
    let light_proj = ortho_vk(-2.4f, 2.4f, -2.4f, 2.4f, 0.1f, 14.0f)
    ubo.light_vp = light_proj * light_view
    ubo.camera = float4(CAM_EYE.x, CAM_EYE.y, CAM_EYE.z, debug_tint ? 1.0f : 0.0f)
    ubo.light = float4(light_dir, 0.0f)
    teapot_mesh_bind_uniform(device, res.ubo.memory)
}

def public record_teapot_render_pass(res : TeapotResources; cmd : CommandBuffer) {
    var sets <- [vk_value_to_boost(res.desc_set)]
    var no_dyn : array<uint>

    // ===== Pass 1: shadow map — the teapot's depth from the light's POV (mesh-shader, depth-only).
    // The pass's finalLayout = DEPTH_STENCIL_READ_ONLY_OPTIMAL hands the image to pass 2 ready to sample.
    var shadow_clears <- [clear_depth(1.0f)]
    record_render_pass(cmd, res.rp_shadow, res.fb_shadow, full_area(SHADOW_W, SHADOW_H), shadow_clears) {
        cmd_bind_pipeline(cmd, res.pipeline_shadow)
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.pipe_layout, 0u, sets, no_dyn)
        cmd_draw_mesh_tasks_e_x_t(cmd, 1u, 1u, 1u)
    }
    delete shadow_clears

    // ===== Pass 2: color — sky, then the floor (which samples the shadow map), then the teapot.
    var clears <- [clear_color(0.06f, 0.07f, 0.10f, 1.0f), clear_depth(1.0f)]
    record_render_pass(cmd, res.render_pass, res.framebuffer, full_area(TEAPOT_W, TEAPOT_H), clears) {
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.pipe_layout, 0u, sets, no_dyn)
        // 1. env sky (depth off) paints the background, so the corners are the environment, not a void
        cmd_bind_pipeline(cmd, res.pipeline_sky)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.sky_vb.buffer))
        cmd_draw(cmd, 3u)
        // 2. reflective checkerboard ground the teapot sits on (receives the teapot's shadow)
        cmd_bind_pipeline(cmd, res.pipeline_floor)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.floor_vb.buffer))
        cmd_draw(cmd, 6u)
        // 3. the GPU-tessellated teapot (mesh-shader pipeline)
        cmd_bind_pipeline(cmd, res.pipeline)
        cmd_draw_mesh_tasks_e_x_t(cmd, 1u, 1u, 1u)
    }
    delete clears
    delete sets
    delete no_dyn
}

def public render_teapot_frame(var ctx : TeapotContext; time : float; debug_tint : bool) : array<uint8> {
    update_teapot_uniforms(ctx.res, ctx.device, time, debug_tint)
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_teapot_render_pass(ctx.res, cmd)
        copy_image_to_buffer(cmd, ctx.res.color.image, ctx.res.readback, TEAPOT_W, TEAPOT_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! One-shot: build a context and render one frame (the [test] uses this).
def public render_teapot(time : float; debug_tint : bool = false) : array<uint8> {
    var inscope ctx <- build_teapot_context()
    return <- render_teapot_frame(ctx, time, debug_tint)
}

//! Sample the RGB of pixel (x,y) from a TEAPOT_W*TEAPOT_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * TEAPOT_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
