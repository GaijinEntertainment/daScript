options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 08 -- the canonical two-pass shadow-map dance. One offscreen D32_SFLOAT image used in
// two roles: depth attachment in pass 1 (when we render the scene from the LIGHT'S point of view),
// sampler2DShadow in pass 2 (when the main pass asks "is anything closer to the light here than
// I am?"). The headline rails:
//
//   1. **Depth-only render pass + one image, two roles.** ``create_render_pass_depth_only`` makes a
//      pass with one depth attachment, no colors; ``build_offscreen_depth(extra=sampled)`` allocates
//      the shadow map with ``USAGE_DEPTH_STENCIL_ATTACHMENT | USAGE_SAMPLED``. The depth-only
//      pass's ``finalLayout = DEPTH_STENCIL_READ_ONLY_OPTIMAL`` hands the image to the main pass
//      already laid out for sampling -- no manual barrier between the passes.
//
//   2. ``sampler2DShadow`` + ``textureCompare(...)``. dasSpirv emits OpTypeImage with the Depth=1
//      flag and ``OpImageSampleDrefImplicitLod`` for the sample, returning the [0,1] PCF result.
//      ``create_sampler_shadow`` is the host-side preset: ``compareEnable=true`` +
//      ``compareOp=LESS`` + LINEAR filter for hardware-PCF + white border for off-map = lit.
//
//   3. **Slope-scaled depth bias** on the shadow pipeline -- ``depthBiasEnable=VK_TRUE`` +
//      ``depthBiasSlopeFactor=3.0`` + a small constant bias. Without these the cube and floor
//      self-shadow on every grazing fragment ("shadow acne"). The values are scene-dependent;
//      these were tuned alongside the fragment-side N.L bias in the main shader.
//
//   4. **Vertex-only graphics pipeline** for the shadow pass (no color attachments, no fragment
//      shader). Vulkan accepts ``stageCount=1`` with just the vertex stage when there are no
//      color attachments -- the rasterizer drives depth via fixed function.

module shadow_tut public

require vulkan/vulkan_boost public
require vulkan/vulkan_reflect public
require vulkan/vulkan_assets
require daslib/defer
require daslib/fio
require daslib/module_path
require shadow_tut_shaders public
require math

// ===== scene constants =====

let public SCENE_W = 768
let public SCENE_H = 768
let public SHADOW_W = 1024
let public SHADOW_H = 1024
let public SCENE_COLOR_FMT = VkFormat.R8G8B8A8_SRGB   // sRGB: gamma-encode the linear output on write (recording readback + window blit match)
let public SCENE_DEPTH_FMT = VkFormat.D32_SFLOAT
let public SHADOW_FMT = VkFormat.D32_SFLOAT

// UBO layout: 3 mat4 (192) + 2 vec4 (32) = 224 bytes std140.
let UBO_SIZE = 224ul

// ===== geometry =====

//! 24 vertices = 6 faces * 4 corners; each vertex is (pos.xyz, normal.xyz). Face normals are
//! repeated per-corner so the shader gets the FACE normal (sharp shading, no per-vertex average).
let private cube_vertices : array<float> <- [
    // +Y face (top), normal = (0, 1, 0)
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    // -Y face (bottom), normal = (0, -1, 0)
    -0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f,
    // +X face, normal = (1, 0, 0)
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    // -X face, normal = (-1, 0, 0)
    -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
    // +Z face, normal = (0, 0, 1)
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    // -Z face, normal = (0, 0, -1)
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f
]

let private cube_indices : array<uint16> <- [
    uint16(0),  uint16(1),  uint16(2),  uint16(0),  uint16(2),  uint16(3),
    uint16(4),  uint16(5),  uint16(6),  uint16(4),  uint16(6),  uint16(7),
    uint16(8),  uint16(9),  uint16(10), uint16(8),  uint16(10), uint16(11),
    uint16(12), uint16(13), uint16(14), uint16(12), uint16(14), uint16(15),
    uint16(16), uint16(17), uint16(18), uint16(16), uint16(18), uint16(19),
    uint16(20), uint16(21), uint16(22), uint16(20), uint16(22), uint16(23)
]

//! Floor quad in the XZ plane at y=0, normal up. 4 vertices, 6 indices. The model matrix scales
//! it to cover the scene; we want a quad big enough that the cube's shadow fits inside.
let private floor_vertices : array<float> <- [
    -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f
]

let private floor_indices : array<uint16> <- [
    uint16(0), uint16(1), uint16(2),
    uint16(0), uint16(2), uint16(3)
]

let CUBE_INDEX_COUNT = 36
let FLOOR_INDEX_COUNT = 6

// ===== matrix builders (column-major, std140-friendly) =====

def private identity_m4 : float4x4 {
    var m : float4x4
    m[0] = float4(1.0, 0.0, 0.0, 0.0)
    m[1] = float4(0.0, 1.0, 0.0, 0.0)
    m[2] = float4(0.0, 0.0, 1.0, 0.0)
    m[3] = float4(0.0, 0.0, 0.0, 1.0)
    return m
}

def private translate_m4(t : float3) : float4x4 {
    var m = identity_m4()
    m[3] = float4(t.x, t.y, t.z, 1.0)
    return m
}

def private scale_m4(s : float3) : float4x4 {
    var m : float4x4
    m[0] = float4(s.x, 0.0, 0.0, 0.0)
    m[1] = float4(0.0, s.y, 0.0, 0.0)
    m[2] = float4(0.0, 0.0, s.z, 0.0)
    m[3] = float4(0.0, 0.0, 0.0, 1.0)
    return m
}

def private rotate_y_m4(angle : float) : float4x4 {
    let c = cos(angle)
    let s = sin(angle)
    var m : float4x4
    m[0] = float4(   c, 0.0,  -s, 0.0)
    m[1] = float4( 0.0, 1.0, 0.0, 0.0)
    m[2] = float4(   s, 0.0,   c, 0.0)
    m[3] = float4( 0.0, 0.0, 0.0, 1.0)
    return m
}

//! Right-handed look-at: world-to-view. Camera at `eye`, looking at `target`, with `up` the
//! world-up reference. Returns the view matrix in column-major form (std140 mat4).
def private look_at_m4(eye, target, up : float3) : float4x4 {
    let f = normalize(target - eye)             // forward (camera -> target)
    let r = normalize(cross(f, up))             // right
    let u = cross(r, f)                          // recomputed up
    var m : float4x4
    m[0] = float4(r.x,  u.x, -f.x, 0.0)
    m[1] = float4(r.y,  u.y, -f.y, 0.0)
    m[2] = float4(r.z,  u.z, -f.z, 0.0)
    m[3] = float4(-dot(r, eye), -dot(u, eye), dot(f, eye), 1.0)
    return m
}

//! Vulkan perspective: NDC.z in [0, 1], NDC.y flipped relative to OpenGL. `fovy` radians.
def private perspective_m4(fovy, aspect, znear, zfar : float) : float4x4 {
    let f = 1.0 / tan(fovy * 0.5)
    var m : float4x4
    m[0] = float4(f / aspect, 0.0, 0.0, 0.0)
    m[1] = float4(0.0, -f, 0.0, 0.0)            // Vulkan Y flip
    m[2] = float4(0.0, 0.0, zfar / (znear - zfar), -1.0)
    m[3] = float4(0.0, 0.0, (zfar * znear) / (znear - zfar), 0.0)
    return m
}

//! Orthographic projection for the directional light. Vulkan Z [0, 1]; Y NOT flipped because the
//! light's projection is sampled, not displayed.
def private ortho_m4(left, right, bottom, top, znear, zfar : float) : float4x4 {
    var m : float4x4
    m[0] = float4(2.0 / (right - left), 0.0, 0.0, 0.0)
    m[1] = float4(0.0, 2.0 / (top - bottom), 0.0, 0.0)
    m[2] = float4(0.0, 0.0, -1.0 / (zfar - znear), 0.0)
    m[3] = float4(-(right + left) / (right - left),
                  -(top + bottom) / (top - bottom),
                  -znear / (zfar - znear),
                  1.0)
    return m
}

// ===== resources =====

//! All shadow-tutorial render state owned together: two render passes, two framebuffers, two pipelines
//! (shadow + main), four buffers (cube + floor vb/ib), the shadow map + sampler, UBO, descriptors and
//! readback. The driver (offscreen test or window viewer) owns instance/device/queue/pool, and passes.
struct public ShadowResources {
    color        : OffscreenTarget
    depth        : OffscreenDepth
    shadow_map   : OffscreenDepth   // D32_SFLOAT image used as depth attachment AND sampled in pass 2
    rp_color     : RenderPass
    rp_shadow    : RenderPass
    fb_color     : Framebuffer
    fb_shadow    : Framebuffer
    cube_vb      : HostBuffer
    cube_ib      : HostBuffer
    floor_vb     : HostBuffer
    floor_ib     : HostBuffer
    ubo          : HostBuffer
    shadow_sampler : Sampler
    tex_sampler  : Sampler          // linear/repeat for the brick maps
    brick_albedo : Texture          // ambientCG Bricks031 color (sRGB), tri-planar on the cube
    brick_normal : Texture          // ambientCG Bricks031 OpenGL normal (linear)
    brick_ao     : Texture          // ambientCG Bricks031 ambient occlusion (linear)
    brick_roughness : Texture       // ambientCG Bricks031 roughness (linear) -> drives cube spec
    set_layouts  : array<DescriptorSetLayout>
    desc_pool    : DescriptorPool
    desc_set     : VkDescriptorSet
    pipe_layout  : PipelineLayout
    pipeline_shadow : Pipeline
    pipeline_main   : Pipeline
    readback     : HostBuffer
    buf_size     : uint64
}

def public finalize(var r : ShadowResources) {
    unsafe {
        delete r.readback
        delete r.pipeline_main
        delete r.pipeline_shadow
        delete r.pipe_layout
        delete r.desc_pool
        delete r.set_layouts
        delete r.brick_roughness
        delete r.brick_ao
        delete r.brick_normal
        delete r.brick_albedo
        delete r.tex_sampler
        delete r.shadow_sampler
        delete r.ubo
        delete r.floor_ib
        delete r.floor_vb
        delete r.cube_ib
        delete r.cube_vb
        delete r.fb_shadow
        delete r.fb_color
        delete r.rp_shadow
        delete r.rp_color
        delete r.shadow_map
        delete r.depth
        delete r.color
    }
}

//! Self-contained one-shot context for the offscreen oracle + recording driver. Windowed drivers
//! build their own instance/device with surface+swapchain extensions and call
//! ``build_shadow_resources`` directly.
struct public ShadowContext {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : ShadowResources
}

def public finalize(var ctx : ShadowContext) {
    unsafe {
        delete ctx.res
        delete ctx.pool
        delete ctx.device
        delete ctx.instance
    }
}

// ===== pipeline builders =====

//! Vertex input: one binding (binding 0, stride 24 = 6 floats) -- two attributes (pos at +0,
//! normal at +12), both float3. Shared between the shadow and main passes since the same buffers
//! are bound to both.
def private vertex_input() : VkPipelineVertexInputStateCreateInfo {
    let v = VkPipelineVertexInputStateCreateInfo()
    // bindings + attributes filled by the caller (need stable lifetime across vkCreate*Pipeline)
    return v
}

//! Inline shadow pipeline: vertex-only, no color attachments, depth bias enabled.
def private build_shadow_pipeline(device : Device; rp : RenderPass; layout : PipelineLayout;   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
                                  vert : ShaderModule) : Pipeline {
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(1)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.vertex = true
    stages[0].module_ = boost_value_to_vk(vert)
    stages[0].pName = "main"

    var binding = VkVertexInputBindingDescription()
    binding.binding = 0u
    binding.stride = 24u
    binding.inputRate = VkVertexInputRate.VERTEX
    var attrs : array<VkVertexInputAttributeDescription>
    var a_pos = VkVertexInputAttributeDescription()
    a_pos.location = 0u
    a_pos.binding = 0u
    a_pos.format = VkFormat.R32G32B32_SFLOAT
    a_pos.offset = 0u
    attrs |> push(a_pos)

    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = 1u
    vinput.vertexAttributeDescriptionCount = uint(length(attrs))

    var ia = VkPipelineInputAssemblyStateCreateInfo()
    ia.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var viewport = VkViewport()
    viewport.width = float(SHADOW_W)
    viewport.height = float(SHADOW_H)
    viewport.minDepth = 0.0
    viewport.maxDepth = 1.0
    var scissor : VkRect2D
    scissor.extent.width = uint(SHADOW_W)
    scissor.extent.height = uint(SHADOW_H)
    var vp = VkPipelineViewportStateCreateInfo()
    vp.viewportCount = 1u
    vp.scissorCount = 1u

    var rast = VkPipelineRasterizationStateCreateInfo()
    rast.polygonMode = VkPolygonMode.FILL
    rast.cullMode.back = true                         // standard back-face cull on casters
    rast.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    rast.depthBiasEnable = VK_TRUE                    // SLOPE-SCALED BIAS for acne
    rast.depthBiasConstantFactor = 1.5
    rast.depthBiasSlopeFactor = 3.0
    rast.lineWidth = 1.0

    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    depthstate.depthTestEnable = VK_TRUE
    depthstate.depthWriteEnable = VK_TRUE
    depthstate.depthCompareOp = VkCompareOp.LESS

    var blend = VkPipelineColorBlendStateCreateInfo()  // attachmentCount = 0 (no color attachments)

    var h : VkPipeline
    unsafe {
        var gp = VkGraphicsPipelineCreateInfo()
        gp.stageCount = 1u
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        vinput.pVertexBindingDescriptions = addr(binding)
        vinput.pVertexAttributeDescriptions = addr(attrs[0])
        gp.pInputAssemblyState = addr(ia)
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
    delete attrs
    return <- vk_value_to_boost(h)
}

//! Inline main pipeline: vertex + fragment, color + depth attachments, two vertex attributes.
def private build_main_pipeline(device : Device; rp : RenderPass; layout : PipelineLayout;   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
                                vert, frag : ShaderModule) : Pipeline {
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

    var binding = VkVertexInputBindingDescription()
    binding.binding = 0u
    binding.stride = 24u
    binding.inputRate = VkVertexInputRate.VERTEX
    var attrs : array<VkVertexInputAttributeDescription>
    var a_pos = VkVertexInputAttributeDescription()
    a_pos.location = 0u
    a_pos.binding = 0u
    a_pos.format = VkFormat.R32G32B32_SFLOAT
    a_pos.offset = 0u
    attrs |> push(a_pos)
    var a_norm = VkVertexInputAttributeDescription()
    a_norm.location = 1u
    a_norm.binding = 0u
    a_norm.format = VkFormat.R32G32B32_SFLOAT
    a_norm.offset = 12u
    attrs |> push(a_norm)

    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = 1u
    vinput.vertexAttributeDescriptionCount = uint(length(attrs))

    var ia = VkPipelineInputAssemblyStateCreateInfo()
    ia.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var viewport = VkViewport()
    viewport.width = float(SCENE_W)
    viewport.height = float(SCENE_H)
    viewport.minDepth = 0.0
    viewport.maxDepth = 1.0
    var scissor : VkRect2D
    scissor.extent.width = uint(SCENE_W)
    scissor.extent.height = uint(SCENE_H)
    var vp = VkPipelineViewportStateCreateInfo()
    vp.viewportCount = 1u
    vp.scissorCount = 1u

    var rast = VkPipelineRasterizationStateCreateInfo()
    rast.polygonMode = VkPolygonMode.FILL
    rast.cullMode.back = true
    rast.frontFace = VkFrontFace.CLOCKWISE
    rast.lineWidth = 1.0

    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    depthstate.depthTestEnable = VK_TRUE
    depthstate.depthWriteEnable = VK_TRUE
    depthstate.depthCompareOp = VkCompareOp.LESS

    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var h : VkPipeline
    unsafe {
        var gp = VkGraphicsPipelineCreateInfo()
        gp.stageCount = 2u
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        vinput.pVertexBindingDescriptions = addr(binding)
        vinput.pVertexAttributeDescriptions = addr(attrs[0])
        gp.pInputAssemblyState = addr(ia)
        vp.pViewports = addr(viewport)
        vp.pScissors = addr(scissor)
        gp.pViewportState = addr(vp)
        gp.pRasterizationState = addr(rast)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depthstate)
        blend.pAttachments = addr(blend_att)
        gp.pColorBlendState = addr(blend)
        gp.layout = boost_value_to_vk(layout)
        gp.renderPass = boost_value_to_vk(rp)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete stages
    delete attrs
    return <- vk_value_to_boost(h)
}

// ===== build =====

//! Build the shadow tutorial resources against a caller-owned (device, phys, queue, pool). Used by
//! both build_shadow_context (offscreen test/recording) and window/show_shadow.das (swapchain blit).
def public build_shadow_resources(device : Device; phys : VkPhysicalDevice;   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
                                  queue : VkQueue; pool : CommandPool) : ShadowResources {
    var inscope color <- build_offscreen_target(device, phys, SCENE_W, SCENE_H, SCENE_COLOR_FMT)
    var inscope depth <- build_offscreen_depth(device, phys, SCENE_W, SCENE_H, SCENE_DEPTH_FMT)

    // Shadow map: same allocator as the main depth, but with `sampled` so the main fragment can
    // bind it as a sampler2DShadow descriptor.
    var sm_usage : VkImageUsageFlags
    sm_usage.sampled = true
    var inscope shadow_map <- build_offscreen_depth(device, phys, SHADOW_W, SHADOW_H, SHADOW_FMT, sm_usage)

    var inscope rp_color  <- create_render_pass_color_depth(device, SCENE_COLOR_FMT, SCENE_DEPTH_FMT)
    var inscope rp_shadow <- create_render_pass_depth_only(device, SHADOW_FMT)

    var inscope fb_color <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(rp_color),
        pAttachments <- [weak_copy(color.view), weak_copy(depth.view)],
        width = uint(SCENE_W), height = uint(SCENE_H), layers = 1u))
    var inscope fb_shadow <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(rp_shadow),
        pAttachments <- [weak_copy(shadow_map.view)],
        width = uint(SHADOW_W), height = uint(SHADOW_H), layers = 1u))

    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var verts_copy := cube_vertices
    var inscope cube_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, verts_copy)
    delete verts_copy

    var ib_usage : VkBufferUsageFlags
    ib_usage.index_buffer = true
    var ci_copy := cube_indices
    var inscope cube_ib <- create_host_buffer_from_bytes(device, phys, ib_usage, ci_copy)
    delete ci_copy

    var fv_copy := floor_vertices
    var inscope floor_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, fv_copy)
    delete fv_copy

    var fi_copy := floor_indices
    var inscope floor_ib <- create_host_buffer_from_bytes(device, phys, ib_usage, fi_copy)
    delete fi_copy

    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo <- create_host_buffer(device, phys, UBO_SIZE, ubo_usage)

    var inscope shadow_sampler <- create_sampler_shadow(device)

    // Brick PBR maps (ambientCG Bricks031, CC0) from tutorials/_assets/brick, tri-planar on the cube.
    // tutorial_asset_dir resolves relative to the (absolutely-mounted) vulkan_assets module, so it
    // works from any CWD / CI checkout.
    let brick_dir = tutorial_asset_dir("brick")
    var inscope brick_albedo <- load_texture_2d(device, phys, queue, pool,
        path_join(brick_dir, "Bricks031_1K-JPG_Color.jpg"), true)        // sRGB albedo
    var inscope brick_normal <- load_texture_2d(device, phys, queue, pool,
        path_join(brick_dir, "Bricks031_1K-JPG_NormalGL.jpg"), false)    // linear normal
    var inscope brick_ao <- load_texture_2d(device, phys, queue, pool,
        path_join(brick_dir, "Bricks031_1K-JPG_AmbientOcclusion.jpg"), false)   // linear AO
    var inscope brick_roughness <- load_texture_2d(device, phys, queue, pool,
        path_join(brick_dir, "Bricks031_1K-JPG_Roughness.jpg"), false)          // linear roughness
    let tsci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.LINEAR,
        addressModeU = VkSamplerAddressMode.REPEAT,
        addressModeV = VkSamplerAddressMode.REPEAT,
        addressModeW = VkSamplerAddressMode.REPEAT,
        maxLod = 16.0f)                 // sample the full mip chain (kills minification moiré)
    var inscope tex_sampler <- create_sampler(device, tsci)

    // Descriptors: set 0 binding 0 = UBO, binding 1 = sampler2DShadow, binding 2/3 = brick maps. We reflect off the MAIN
    // fragment shader (the only one that sees both bindings) -- the shadow vertex doesn't see the
    // sampler, but the layout it ships with covers a strict subset.
    var reflections <- [decode_reflection(main_vert_spv_reflect), decode_reflection(main_frag_spv_reflect)]
    var inscope set_layouts <- build_descriptor_set_layouts(device, reflections)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    let ps0 = DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    // shadow_map + brick_albedo + brick_normal + brick_ao + brick_roughness
    let ps1 = DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 5u)
    dpci.pPoolSizes <- [ps0, ps1]
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
    let ubo_info = DescriptorBufferInfo(buffer = weak_copy(ubo.buffer), range_ = UBO_SIZE)
    w0.pBufferInfo |> push(ubo_info)
    writes |> emplace(w0)

    var w1 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 1u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    let shadow_info = DescriptorImageInfo(sampler = weak_copy(shadow_sampler),
        imageView = weak_copy(shadow_map.view), imageLayout = VkImageLayout.DEPTH_STENCIL_READ_ONLY_OPTIMAL)
    w1.pImageInfo |> push(shadow_info)
    writes |> emplace(w1)

    var w2 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 2u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    let albedo_info = DescriptorImageInfo(sampler = weak_copy(tex_sampler),
        imageView = weak_copy(brick_albedo.view), imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    w2.pImageInfo |> push(albedo_info)
    writes |> emplace(w2)

    var w3 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 3u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    let normal_info = DescriptorImageInfo(sampler = weak_copy(tex_sampler),
        imageView = weak_copy(brick_normal.view), imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    w3.pImageInfo |> push(normal_info)
    writes |> emplace(w3)

    var w4 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 4u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    let ao_info = DescriptorImageInfo(sampler = weak_copy(tex_sampler),
        imageView = weak_copy(brick_ao.view), imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    w4.pImageInfo |> push(ao_info)
    writes |> emplace(w4)

    var w5 = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set), dstBinding = 5u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    let rough_info = DescriptorImageInfo(sampler = weak_copy(tex_sampler),
        imageView = weak_copy(brick_roughness.view), imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    w5.pImageInfo |> push(rough_info)
    writes |> emplace(w5)

    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    // Shaders are the compile-time-emitted SPIR-V globals from shadow_tut_shaders.
    var inscope shadow_vert <- create_shader_module(device, shadow_vert_spv)
    var inscope main_vert   <- create_shader_module(device, main_vert_spv)
    var inscope main_frag   <- create_shader_module(device, main_frag_spv)

    var inscope pipeline_shadow <- build_shadow_pipeline(device, rp_shadow, pipe_layout, shadow_vert)
    var inscope pipeline_main   <- build_main_pipeline(device, rp_color, pipe_layout, main_vert, main_frag)

    let buf_size = uint64(SCENE_W) * uint64(SCENE_H) * 4ul
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    return <- ShadowResources(
        color <- color,
        depth <- depth,
        shadow_map <- shadow_map,
        rp_color <- rp_color,
        rp_shadow <- rp_shadow,
        fb_color <- fb_color,
        fb_shadow <- fb_shadow,
        cube_vb <- cube_vb,
        cube_ib <- cube_ib,
        floor_vb <- floor_vb,
        floor_ib <- floor_ib,
        ubo <- ubo,
        shadow_sampler <- shadow_sampler,
        tex_sampler <- tex_sampler,
        brick_albedo <- brick_albedo,
        brick_normal <- brick_normal,
        brick_ao <- brick_ao,
        brick_roughness <- brick_roughness,
        set_layouts <- set_layouts,
        desc_pool <- desc_pool,
        desc_set = raw_set,
        pipe_layout <- pipe_layout,
        pipeline_shadow <- pipeline_shadow,
        pipeline_main <- pipeline_main,
        readback <- readback,
        buf_size = buf_size)
}

def public build_shadow_context() : ShadowContext {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("shadow_tut", make_api_version(1u, 1u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let qfam = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, qfam)
    var queue = get_device_queue(device, qfam, 0u)
    var inscope pool <- create_command_pool(device, CommandPoolCreateInfo(queueFamilyIndex = qfam))
    var inscope res <- build_shadow_resources(device, phys, queue, pool)
    return <- ShadowContext(
        instance <- instance,
        device <- device,
        queue = queue,
        pool <- pool,
        res <- res)
}

// ===== per-frame work =====

//! Compute camera + light VP from `time` + caller-tuned `camera_t` (camera orbit progress). The
//! sun rotates around the Y axis so the cube's shadow visibly tracks it. light_dir POINTS TO the
//! sun (the shader does `dot(n, light_dir)` for diffuse).
def public update_shadow_uniforms(res : ShadowResources; device : Device; time, camera_t : float) {
    // Camera: orbit a bit so the cube + its shadow are both readable in the recording.
    let cam_angle = camera_t * 0.6
    let cam_eye = float3(3.4 * cos(cam_angle), 3.2, 3.4 * sin(cam_angle))
    cam.view = look_at_m4(cam_eye, float3(0.0, 0.7, 0.0), float3(0.0, 1.0, 0.0))
    cam.proj = perspective_m4(0.785, float(SCENE_W) / float(SCENE_H), 0.1, 50.0)

    // Sun direction: orbits in the XZ plane, tilted +Y.
    let sun_yaw = time * 0.7
    let light_dir = normalize(float3(cos(sun_yaw), 1.3, sin(sun_yaw)))

    // Light VP: orthographic, sized to the scene (cube + 5x5 floor). Light "position" is far away
    // along `light_dir`; look at origin.
    let light_eye = float3(light_dir.x * 8.0, light_dir.y * 8.0, light_dir.z * 8.0)
    let light_view = look_at_m4(light_eye, float3(0.0, 0.5, 0.0), float3(0.0, 1.0, 0.0))
    let light_proj = ortho_m4(-3.5, 3.5, -3.5, 3.5, 0.1, 20.0)
    cam.light_vp = light_proj * light_view
    cam.light_dir = float4(light_dir.x, light_dir.y, light_dir.z, 0.0)
    cam.camera_pos = float4(cam_eye.x, cam_eye.y, cam_eye.z, 0.0)

    main_vs_bind_uniform(device, res.ubo.memory)
}

//! Per-frame work for both passes. Pass 1 renders cube + floor depth into the shadow map, whose
//! `finalLayout = DEPTH_STENCIL_READ_ONLY_OPTIMAL` hands it to pass 2 ready to sample. Pass 2 renders
//! cube + floor into the color attachment, with the fragment shader reading that shadow map.
def public record_shadow_render_pass(res : ShadowResources; cmd : CommandBuffer) {
    let sets <- [vk_value_to_boost(res.desc_set)]
    var no_dyn : array<uint>

    // ===== Pass 1: shadow map =====
    var shadow_clears <- [clear_depth(1.0)]
    record_render_pass(cmd, res.rp_shadow, res.fb_shadow,
                       full_area(SHADOW_W, SHADOW_H), shadow_clears) {
        cmd_bind_pipeline(cmd, res.pipeline_shadow)
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.pipe_layout, 0u, sets, no_dyn)
        cmd_bind_index_buffer(cmd, weak_copy(res.cube_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.cube_vb.buffer))
        // Cube hovers at (0, 1.5, 0), rotates so the shadow has shape.
        var cube_model = translate_m4(float3(0.0, 1.5, 0.0)) * rotate_y_m4(0.6)
        var vstage : VkShaderStageFlags
        vstage.vertex = true
        vstage.fragment = true   // main_fs reads op.model to rotate the brick normal into world space
        cmd_push_constants(cmd, res.pipe_layout, vstage, 0u, cube_model)
        cmd_draw_indexed(cmd, uint(CUBE_INDEX_COUNT), 1u, 0u, 0, 0u)

        // Floor scaled 5x5, lying flat at y=0.
        cmd_bind_index_buffer(cmd, weak_copy(res.floor_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.floor_vb.buffer))
        var floor_model = scale_m4(float3(5.0, 1.0, 5.0))
        cmd_push_constants(cmd, res.pipe_layout, vstage, 0u, floor_model)
        cmd_draw_indexed(cmd, uint(FLOOR_INDEX_COUNT), 1u, 0u, 0, 0u)
    }
    delete shadow_clears

    // ===== Pass 2: main color, sampling the shadow map =====
    var clears <- [clear_color(0.04f, 0.04f, 0.08f, 1.0f), clear_depth(1.0)]
    record_render_pass(cmd, res.rp_color, res.fb_color,
                       full_area(SCENE_W, SCENE_H), clears) {
        cmd_bind_pipeline(cmd, res.pipeline_main)
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.pipe_layout, 0u, sets, no_dyn)
        cmd_bind_index_buffer(cmd, weak_copy(res.cube_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.cube_vb.buffer))
        var cube_model = translate_m4(float3(0.0, 1.5, 0.0)) * rotate_y_m4(0.6)
        var vstage : VkShaderStageFlags
        vstage.vertex = true
        vstage.fragment = true   // main_fs reads op.model to rotate the brick normal into world space
        cmd_push_constants(cmd, res.pipe_layout, vstage, 0u, cube_model)
        cmd_draw_indexed(cmd, uint(CUBE_INDEX_COUNT), 1u, 0u, 0, 0u)

        cmd_bind_index_buffer(cmd, weak_copy(res.floor_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_bind_vertex_buffer(cmd, 0u, weak_copy(res.floor_vb.buffer))
        var floor_model = scale_m4(float3(5.0, 1.0, 5.0))
        cmd_push_constants(cmd, res.pipe_layout, vstage, 0u, floor_model)
        cmd_draw_indexed(cmd, uint(FLOOR_INDEX_COUNT), 1u, 0u, 0, 0u)
    }
    delete clears
}

//! Per-frame work: update UBO, record both passes, copy color → readback, clone RGBA8.
def public render_shadow_frame(var ctx : ShadowContext; time, camera_t : float) : array<uint8> {
    update_shadow_uniforms(ctx.res, ctx.device, time, camera_t)
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_shadow_render_pass(ctx.res, cmd)
        copy_image_to_buffer(cmd, ctx.res.color.image, ctx.res.readback, SCENE_W, SCENE_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! One-shot: build a context and render one frame.
def public render_shadow_scene(time, camera_t : float) : array<uint8> {
    var inscope ctx <- build_shadow_context()
    return <- render_shadow_frame(ctx, time, camera_t)
}
