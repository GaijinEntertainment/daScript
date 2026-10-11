options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 16 render path: three textured cubes lit through a two-pass G-buffer, on the three Vulkan
// 1.4 promotions that change how a frame is put together. HOST IMAGE COPY uploads each cube's texture
// straight from host memory - no staging buffer, no command buffer. PUSH DESCRIPTORS write every
// descriptor into the command buffer per draw - no descriptor pool, no set allocation. LOCAL READ lets
// the lighting pass read the albedo and normal attachments the geometry pass just wrote, inside the
// same dynamic rendering, through subpassInput - what a render pass needed a second subpass for.
// A pure parametric frame(time) -> image: the oracle asserts known frames, the recording sweeps time.

module vulkan14_tut

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan14_tut_shaders public
require daslib/math_boost
require daslib/enum_trait
require math

let ALBEDO_FMT = VkFormat.R8G8B8A8_UNORM    // G-buffer stays linear: the lighting pass does the math on it
let NORMAL_FMT = VkFormat.R8G8B8A8_UNORM    // world normal packed into [0,1]
let COLOR_FMT = VkFormat.R8G8B8A8_SRGB      // the lit result, gamma-encoded on write for readback and blit
let DEPTH_FMT = VkFormat.D32_SFLOAT
let TEX_DIM = 64
let public N_CUBES = 3
let public N_INDICES = 36
let N_COLOR_ATTACHMENTS = 3u                // albedo, normal, lit color: one rendering holds all three
let CUBE_VERTEX_STRIDE = 32u                // 8 floats: pos.xyz, uv, normal.xyz
let CAMERA_UBO_BYTES = 128ul                // two column-major mat4, std140

//! CUBE_VERTEX_STRIDE bytes per vertex - pos.xyz, uv, normal.xyz - faces wound CCW.
let private cube_vertices : array<float> <- [
    -0.5, -0.5, 0.5,  0.0, 1.0,   0.0, 0.0, 1.0,
     0.5, -0.5, 0.5,  1.0, 1.0,   0.0, 0.0, 1.0,
     0.5, 0.5, 0.5,  1.0, 0.0,   0.0, 0.0, 1.0,
    -0.5, 0.5, 0.5,  0.0, 0.0,   0.0, 0.0, 1.0,
     0.5, -0.5, -0.5,  0.0, 1.0,   0.0, 0.0, -1.0,
    -0.5, -0.5, -0.5,  1.0, 1.0,   0.0, 0.0, -1.0,
    -0.5, 0.5, -0.5,  1.0, 0.0,   0.0, 0.0, -1.0,
     0.5, 0.5, -0.5,  0.0, 0.0,   0.0, 0.0, -1.0,
     0.5, -0.5, 0.5,  0.0, 1.0,   1.0, 0.0, 0.0,
     0.5, -0.5, -0.5,  1.0, 1.0,   1.0, 0.0, 0.0,
     0.5, 0.5, -0.5,  1.0, 0.0,   1.0, 0.0, 0.0,
     0.5, 0.5, 0.5,  0.0, 0.0,   1.0, 0.0, 0.0,
    -0.5, -0.5, -0.5,  0.0, 1.0,  -1.0, 0.0, 0.0,
    -0.5, -0.5, 0.5,  1.0, 1.0,  -1.0, 0.0, 0.0,
    -0.5, 0.5, 0.5,  1.0, 0.0,  -1.0, 0.0, 0.0,
    -0.5, 0.5, -0.5,  0.0, 0.0,  -1.0, 0.0, 0.0,
    -0.5, 0.5, 0.5,  0.0, 1.0,   0.0, 1.0, 0.0,
     0.5, 0.5, 0.5,  1.0, 1.0,   0.0, 1.0, 0.0,
     0.5, 0.5, -0.5,  1.0, 0.0,   0.0, 1.0, 0.0,
    -0.5, 0.5, -0.5,  0.0, 0.0,   0.0, 1.0, 0.0,
    -0.5, -0.5, -0.5,  0.0, 1.0,   0.0, -1.0, 0.0,
     0.5, -0.5, -0.5,  1.0, 1.0,   0.0, -1.0, 0.0,
     0.5, -0.5, 0.5,  1.0, 0.0,   0.0, -1.0, 0.0,
    -0.5, -0.5, 0.5,  0.0, 0.0,   0.0, -1.0, 0.0]

let private cube_indices : array<uint16> <- [
    uint16(0), uint16(1), uint16(2),  uint16(0), uint16(2), uint16(3),
    uint16(4), uint16(5), uint16(6),  uint16(4), uint16(6), uint16(7),
    uint16(8), uint16(9), uint16(10), uint16(8), uint16(10), uint16(11),
    uint16(12), uint16(13), uint16(14), uint16(12), uint16(14), uint16(15),
    uint16(16), uint16(17), uint16(18), uint16(16), uint16(18), uint16(19),
    uint16(20), uint16(21), uint16(22), uint16(20), uint16(22), uint16(23)]

//! `perspective_rh_0_to_1` with the Y flip Vulkan's framebuffer needs (+Y is down in its NDC).
def perspective_vk(fov_y, aspect, znear, zfar : float) : float4x4 {
    var m = perspective_rh_0_to_1(fov_y, aspect, znear, zfar)
    m[1].y = -m[1].y
    return m
}

//! Rotation about Y, then translation to `pos` - no scale, so the vertex shader transforms normals by this same matrix.
def model_matrix(angle : float; pos : float3) : float4x4 {
    let c = cos(angle)
    let s = sin(angle)
    var m : float4x4
    m[0] = float4(c, 0.0f, -s, 0.0f)
    m[1] = float4(0.0f, 1.0f, 0.0f, 0.0f)
    m[2] = float4(s, 0.0f, c, 0.0f)
    m[3] = float4(pos.x, pos.y, pos.z, 1.0f)
    return m
}

//! One hue per cube: the oracle tells the three cubes apart by color.
enum CubePattern {
    red_checker
    green_stripes
    blue_dots
}

//! TEX_DIM x TEX_DIM RGBA8 of `pattern`.
def gen_cube_texture(pattern : CubePattern) : array<uint8> {
    var px : array<uint8>
    px |> resize(TEX_DIM * TEX_DIM * 4)
    for (y in range(TEX_DIM)) {
        for (x in range(TEX_DIM)) {
            var rgb = int3(0, 0, 0)
            if (pattern == CubePattern.red_checker) {
                let on = ((x / 8) + (y / 8)) % 2 == 0
                rgb = int3(230, on ? 40 : 230, on ? 40 : 230)
            } elif (pattern == CubePattern.green_stripes) {
                let on = (x / 8) % 2 == 0
                rgb = int3(on ? 30 : 10, on ? 210 : 40, on ? 60 : 10)
            } else {
                let dx = (x % 16) - 8
                let dy = (y % 16) - 8
                let on = dx * dx + dy * dy < 20
                rgb = int3(on ? 50 : 230, on ? 90 : 230, 240)
            }
            let o = (y * TEX_DIM + x) * 4
            px[o] = uint8(rgb.x)
            px[o + 1] = uint8(rgb.y)
            px[o + 2] = uint8(rgb.z)
            px[o + 3] = uint8(255)
        }
    }
    return <- px
}

//! One TEX_DIM x TEX_DIM RGBA8 texture whose usage carries ``host_transfer`` beside ``sampled``, filled
//! from `pixels` through a host image copy into `layout` (the device's choice from `host_copy_layout`).
//! No staging buffer, no command buffer, no queue: the image is ready to sample when this returns.
def create_host_copied_texture(device : Device; phys : VkPhysicalDevice; pixels : array<uint8>; layout : VkImageLayout) : OffscreenTarget {
    let usage = VkImageUsageFlags.sampled | VkImageUsageFlags.host_transfer     // host_transfer: the 1.4 usage bit a host copy needs
    var tex <- build_image(device, phys, TEX_DIM, TEX_DIM, VkFormat.R8G8B8A8_UNORM, VkSampleCountFlags._1, usage)
    upload_image_from_host(device, tex.image, pixels, TEX_DIM, TEX_DIM, layout)
    return <- tex
}

// ===== the two passes inside one rendering =====

//! Per pass, which attachment each fragment output location lands in and which attachment each ``subpassInput``
//! index reads: geometry writes attachments 0 and 1 and reads nothing, lighting writes 2 and reads 0 and 1.
//! Each pipeline is built with its pass's maps, and the command buffer sets the same maps before that pass's draws.
let GBUF_LOCATIONS <- [0u, 1u, VK_ATTACHMENT_UNUSED]
let GBUF_INPUTS <- [VK_ATTACHMENT_UNUSED, VK_ATTACHMENT_UNUSED, VK_ATTACHMENT_UNUSED]
let LIGHT_LOCATIONS <- [VK_ATTACHMENT_UNUSED, VK_ATTACHMENT_UNUSED, 0u]
let LIGHT_INPUTS <- [0u, 1u, VK_ATTACHMENT_UNUSED]

enum PassKind {
    geometry
    lighting
}

//! The cube vertex: pos (location 0), uv (1), normal (2).
def cube_vertex_attributes() : array<VkVertexInputAttributeDescription> {
    return <- [
        VkVertexInputAttributeDescription(location = 0u, format = VkFormat.R32G32B32_SFLOAT, offset = 0u),
        VkVertexInputAttributeDescription(location = 1u, format = VkFormat.R32G32_SFLOAT, offset = 12u),
        VkVertexInputAttributeDescription(location = 2u, format = VkFormat.R32G32B32_SFLOAT, offset = 20u)]
}

//! Built by hand: each pipeline names the rendering's three color attachments and carries its pass's maps on
//! the create info's pNext chain (``VkPipelineRenderingCreateInfo`` -> ``VkRenderingAttachmentLocationInfo`` ->
//! ``VkRenderingInputAttachmentIndexInfo``); the chain is linked here, in the frame the create info lives in.
def build_pass_pipeline(device : Device; layout : PipelineLayout; vert, frag : ShaderModule; kind : PassKind) : Pipeline {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    let is_geometry_pass = kind == PassKind.geometry
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
    binding.stride = CUBE_VERTEX_STRIDE
    binding.inputRate = VkVertexInputRate.VERTEX
    var attrs <- cube_vertex_attributes()
    var vinput = VkPipelineVertexInputStateCreateInfo()
    if (is_geometry_pass) {
        vinput.vertexBindingDescriptionCount = 1u
        vinput.vertexAttributeDescriptionCount = uint(length(attrs))
    }
    var ia = VkPipelineInputAssemblyStateCreateInfo()
    ia.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var viewport = VkViewport()
    viewport.width = float(V14_W)
    viewport.height = float(V14_H)
    viewport.maxDepth = 1.0
    var scissor : VkRect2D
    scissor.extent.width = uint(V14_W)
    scissor.extent.height = uint(V14_H)
    var vp = VkPipelineViewportStateCreateInfo()
    vp.viewportCount = 1u
    vp.scissorCount = 1u

    var rast = VkPipelineRasterizationStateCreateInfo()
    rast.polygonMode = VkPolygonMode.FILL
    rast.cullMode.back = is_geometry_pass
    rast.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    rast.lineWidth = 1.0
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    // the geometry pass tests and writes depth; the lighting pass is a fullscreen triangle with depth off
    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    depthstate.depthTestEnable = is_geometry_pass ? VK_TRUE : VK_FALSE
    depthstate.depthWriteEnable = is_geometry_pass ? VK_TRUE : VK_FALSE
    depthstate.depthCompareOp = VkCompareOp.LESS

    // one blend attachment per attachment of the rendering, written by this pass or not
    var blend_atts : array<VkPipelineColorBlendAttachmentState>
    blend_atts |> resize(int(N_COLOR_ATTACHMENTS))
    for (att in blend_atts) {
        att.colorWriteMask = VkColorComponentFlags.r | VkColorComponentFlags.g | VkColorComponentFlags.b | VkColorComponentFlags.a
    }
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = N_COLOR_ATTACHMENTS

    var color_fmts <- [ALBEDO_FMT, NORMAL_FMT, COLOR_FMT]
    var locations := is_geometry_pass ? GBUF_LOCATIONS : LIGHT_LOCATIONS
    var inputs := is_geometry_pass ? GBUF_INPUTS : LIGHT_INPUTS
    var rendering = VkPipelineRenderingCreateInfo()
    rendering.colorAttachmentCount = N_COLOR_ATTACHMENTS
    rendering.depthAttachmentFormat = DEPTH_FMT
    var loc_info = VkRenderingAttachmentLocationInfo()
    loc_info.colorAttachmentCount = N_COLOR_ATTACHMENTS
    var in_info = VkRenderingInputAttachmentIndexInfo()
    in_info.colorAttachmentCount = N_COLOR_ATTACHMENTS

    var h : VkPipeline
    unsafe {
        rendering.pColorAttachmentFormats = addr(color_fmts[0])
        loc_info.pColorAttachmentLocations = addr(locations[0])
        in_info.pColorAttachmentInputIndices = addr(inputs[0])
        rendering.pNext = addr<void?>(loc_info)
        loc_info.pNext = addr<void?>(in_info)
        var gp = VkGraphicsPipelineCreateInfo()
        gp.pNext = addr<void?>(rendering)
        gp.stageCount = 2u
        gp.pStages = addr(stages[0])
        vinput.pVertexBindingDescriptions = addr(binding)
        vinput.pVertexAttributeDescriptions = addr(attrs[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(ia)
        vp.pViewports = addr(viewport)
        vp.pScissors = addr(scissor)
        gp.pViewportState = addr(vp)
        gp.pRasterizationState = addr(rast)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depthstate)
        blend.pAttachments = addr(blend_atts[0])
        gp.pColorBlendState = addr(blend)
        gp.layout = boost_value_to_vk(layout)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete stages
    delete attrs
    delete blend_atts
    delete color_fmts
    delete locations
    delete inputs
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! Everything the frame needs that depends only on (device, phys): the G-buffer and color targets, the
//! cube geometry, the camera UBO, the three host-copied textures, both pipelines on push-descriptor set
//! layouts, and a host-visible readback. No descriptor pool anywhere.
struct public Vulkan14Resources {
    albedo            : OffscreenTarget
    normal            : OffscreenTarget
    color             : OffscreenTarget
    depth             : OffscreenDepth
    vb                : HostBuffer
    ib                : HostBuffer
    ubo               : HostBuffer
    textures          : array<OffscreenTarget>
    tex_layout        : VkImageLayout       // the layout host_copy_layout chose; the pushed descriptor names it
    sampler           : Sampler
    gbuf_layouts      : array<DescriptorSetLayout>
    gbuf_pipe_layout  : PipelineLayout
    gbuf_pipeline     : Pipeline
    light_layouts     : array<DescriptorSetLayout>
    light_pipe_layout : PipelineLayout
    light_pipeline    : Pipeline
    readback          : HostBuffer
    buf_size          : uint64
}

def public finalize(var r : Vulkan14Resources) {
    delete r.readback
    delete r.light_pipeline
    delete r.light_pipe_layout
    delete r.light_layouts
    delete r.gbuf_pipeline
    delete r.gbuf_pipe_layout
    delete r.gbuf_layouts
    delete r.sampler
    delete r.textures
    delete r.ubo
    delete r.ib
    delete r.vb
    delete r.depth
    delete r.color
    delete r.normal
    delete r.albedo
}

//! The scene's resources on a caller-owned device from `create_device_vulkan14` (the context's or the viewer's).
def public build_vulkan14_resources(device : Device; phys : VkPhysicalDevice) : Vulkan14Resources {
    let gbuf_usage = VkImageUsageFlags.input_attachment          // read back inside the rendering by the lighting pass
    var inscope albedo <- build_offscreen_target(device, phys, V14_W, V14_H, ALBEDO_FMT, VkSampleCountFlags._1, gbuf_usage)
    var inscope normal <- build_offscreen_target(device, phys, V14_W, V14_H, NORMAL_FMT, VkSampleCountFlags._1, gbuf_usage)
    var inscope color <- build_offscreen_target(device, phys, V14_W, V14_H, COLOR_FMT)
    var inscope depth <- build_offscreen_depth(device, phys, V14_W, V14_H, DEPTH_FMT)

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
    var inscope ubo <- create_host_buffer(device, phys, CAMERA_UBO_BYTES, ubo_usage)

    // host image copy lands in SHADER_READ_ONLY_OPTIMAL where the device lists it, else GENERAL - both sample legally
    let tex_layout = host_copy_layout(phys, VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    var inscope textures : array<OffscreenTarget>
    textures |> reserve(N_CUBES)
    for (pattern in each(CubePattern.red_checker)) {
        var pixels <- gen_cube_texture(pattern)
        textures |> emplace(create_host_copied_texture(device, phys, pixels, tex_layout))
        delete pixels
    }
    let sci = SamplerCreateInfo(
        magFilter = VkFilter.NEAREST,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.NEAREST,
        addressModeU = VkSamplerAddressMode.REPEAT,
        addressModeV = VkSamplerAddressMode.REPEAT,
        addressModeW = VkSamplerAddressMode.REPEAT,
        maxLod = 1.0f)
    var inscope sampler <- create_sampler(device, sci)

    // push-descriptor set layouts: the reflected bindings, flagged so the sets are never allocated
    var gbuf_refl <- [decode_reflection(gbuf_vert_spv_reflect), decode_reflection(gbuf_frag_spv_reflect)]
    var inscope gbuf_layouts <- build_push_descriptor_set_layouts(device, gbuf_refl)
    var inscope gbuf_pipe_layout <- build_pipeline_layout(device, gbuf_layouts, gbuf_refl)
    delete gbuf_refl
    var light_refl <- [decode_reflection(light_vert_spv_reflect), decode_reflection(light_frag_spv_reflect)]
    var inscope light_layouts <- build_push_descriptor_set_layouts(device, light_refl)
    var inscope light_pipe_layout <- build_pipeline_layout(device, light_layouts, light_refl)
    delete light_refl

    var inscope gbuf_vert <- create_shader_module(device, gbuf_vert_spv)
    var inscope gbuf_frag <- create_shader_module(device, gbuf_frag_spv)
    var inscope light_vert <- create_shader_module(device, light_vert_spv)
    var inscope light_frag <- create_shader_module(device, light_frag_spv)
    var inscope gbuf_pipeline <- build_pass_pipeline(device, gbuf_pipe_layout, gbuf_vert, gbuf_frag, PassKind.geometry)
    var inscope light_pipeline <- build_pass_pipeline(device, light_pipe_layout, light_vert, light_frag, PassKind.lighting)

    let buf_size = uint64(V14_W * V14_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    return <- Vulkan14Resources(
        albedo <- albedo, normal <- normal, color <- color, depth <- depth,
        vb <- vb, ib <- ib, ubo <- ubo,
        textures <- textures, tex_layout = tex_layout, sampler <- sampler,
        gbuf_layouts <- gbuf_layouts, gbuf_pipe_layout <- gbuf_pipe_layout, gbuf_pipeline <- gbuf_pipeline,
        light_layouts <- light_layouts, light_pipe_layout <- light_pipe_layout, light_pipeline <- light_pipeline,
        readback <- readback, buf_size = buf_size)
}

//! Self-contained one-shot context for the oracle and the recording driver: a 1.4 instance, a 1.4 device
//! from `create_device_vulkan14`, queue, pool, resources. The windowed viewer builds its own instance and
//! device (surface + swapchain extensions) and calls `build_vulkan14_resources`.
struct public Vulkan14Context {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : Vulkan14Resources
}

def public finalize(var ctx : Vulkan14Context) {
    delete ctx.res
    delete ctx.pool
    delete ctx.device
    delete ctx.instance
}

//! True when a 1.4 instance can be made and its device reports the three features `create_device_vulkan14` needs.
def public vulkan14_available() : bool {
    return probe_device("dasVulkan tutorial 16 probe", make_api_version(1u, 4u, 0u)) $(phys) {
        return vulkan14_supported(phys)
    }
}

def public build_vulkan14_context() : Vulkan14Context {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 16", make_api_version(1u, 4u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device_vulkan14(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope res <- build_vulkan14_resources(device, phys)
    return <- Vulkan14Context(instance <- instance, device <- device, queue = queue, pool <- pool, res <- res)
}

// ===== per frame =====

//! Camera orbit into the UBO through the generated `gbuf_vs_bind_uniform`; light direction into the push-constant global.
def public update_vulkan14_uniforms(res : Vulkan14Resources; device : Device; time : float) {
    let orbit = time * 0.25f
    let eye = float3(4.6f * sin(orbit), 1.6f, 4.6f * cos(orbit))
    cam.view = look_at_rh(eye, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f))
    cam.proj = perspective_vk(50.0f * PI / 180.0f, float(V14_W) / float(V14_H), 0.1f, 20.0f)
    gbuf_vs_bind_uniform(device, res.ubo.memory)
    pc.light_dir = float4(0.7f * cos(time * 0.8f), 1.0f, 0.7f * sin(time * 0.8f) + 0.4f, 0.0f)
}

//! Cube `i`'s camera UBO and texture, pushed into the command buffer - there is no set to allocate or update.
def push_cube_descriptors(res : Vulkan14Resources; cmd : CommandBuffer; i : int) {
    var w0 = WriteDescriptorSet(dstBinding = 0u, descriptorType = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    w0.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(res.ubo.buffer), range_ = CAMERA_UBO_BYTES))
    var w1 = WriteDescriptorSet(dstBinding = 1u, descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 1u)
    w1.pImageInfo |> push(DescriptorImageInfo(sampler = weak_copy(res.sampler), imageView = weak_copy(res.textures[i].view),
        imageLayout = res.tex_layout))
    var writes <- [ <- w0, <- w1]
    cmd_push_descriptor_set(cmd, VkPipelineBindPoint.GRAPHICS, res.gbuf_pipe_layout, 0u, writes)
    unsafe { delete writes }
}

//! Albedo and normal as input attachments in RENDERING_LOCAL_READ, the layout the rendering holds them in.
def push_light_descriptors(res : Vulkan14Resources; cmd : CommandBuffer) {
    var writes : array<WriteDescriptorSet>
    writes |> reserve(2)
    for (binding, target in [0u, 1u], [weak_copy(res.albedo.view), weak_copy(res.normal.view)]) {
        var w = WriteDescriptorSet(dstBinding = binding, descriptorType = VkDescriptorType.INPUT_ATTACHMENT, descriptorCount = 1u)
        w.pImageInfo |> push(DescriptorImageInfo(imageView = weak_copy(target), imageLayout = VkImageLayout.RENDERING_LOCAL_READ))
        writes |> emplace(w)
    }
    cmd_push_descriptor_set(cmd, VkPipelineBindPoint.GRAPHICS, res.light_pipe_layout, 0u, writes)
    unsafe { delete writes }
}

//! The command-buffer twin of the pipeline's maps: fragment location -> attachment, attachment -> ``subpassInput`` index.
def set_pass_maps(cmd : CommandBuffer; locations, inputs : array<uint>) {
    var loc : RenderingAttachmentLocationInfo
    loc.pColorAttachmentLocations := locations
    cmd_set_rendering_attachment_locations(cmd, loc)
    delete loc
    var inp : RenderingInputAttachmentIndexInfo
    inp.pColorAttachmentInputIndices := inputs
    cmd_set_rendering_input_attachment_indices(cmd, inp)
    delete inp
}

//! Record the frame: the G-buffer attachments move to RENDERING_LOCAL_READ (written as attachments and read
//! as input attachments in that one layout), then ONE rendering holds both passes - geometry into attachments
//! 0 and 1, a by-region barrier, lighting into attachment 2. Ends with the lit color in TRANSFER_SRC_OPTIMAL.
def public record_vulkan14_frame(res : Vulkan14Resources; cmd : CommandBuffer; time : float) {
    let local_read_stages = VkPipelineStageFlags2.color_attachment_output | VkPipelineStageFlags2.fragment_shader
    let local_read_access = VkAccessFlags2.color_attachment_write | VkAccessFlags2.input_attachment_read
    // every attachment starts the frame UNDEFINED: the previous frame's reads finished at its queue-idle wait
    transition_image2(cmd, res.albedo.image, VkImageLayout.UNDEFINED, VkImageLayout.RENDERING_LOCAL_READ,
        PIPELINE_STAGE_2_NONE, ACCESS_2_NONE, local_read_stages, local_read_access)
    transition_image2(cmd, res.normal.image, VkImageLayout.UNDEFINED, VkImageLayout.RENDERING_LOCAL_READ,
        PIPELINE_STAGE_2_NONE, ACCESS_2_NONE, local_read_stages, local_read_access)
    transition_image2(cmd, res.color.image, VkImageLayout.UNDEFINED, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
        PIPELINE_STAGE_2_NONE, ACCESS_2_NONE,
        VkPipelineStageFlags2.color_attachment_output, VkAccessFlags2.color_attachment_write)
    transition_depth_image2(cmd, res.depth.image, VkImageLayout.UNDEFINED, VkImageLayout.DEPTH_ATTACHMENT_OPTIMAL,
        PIPELINE_STAGE_2_NONE, ACCESS_2_NONE,
        VkPipelineStageFlags2.early_fragment_tests | VkPipelineStageFlags2.late_fragment_tests,
        VkAccessFlags2.depth_stencil_attachment_write)

    // the G-buffer attachments are transient: cleared on entry, read inside, never stored
    var atts <- [
        <- RenderingAttachmentInfo(imageView = weak_copy(res.albedo.view), imageLayout = VkImageLayout.RENDERING_LOCAL_READ,
            loadOp = VkAttachmentLoadOp.CLEAR, storeOp = VkAttachmentStoreOp.DONT_CARE, clearValue = clear_color(0.0f, 0.0f, 0.0f, 0.0f)),
        <- RenderingAttachmentInfo(imageView = weak_copy(res.normal.view), imageLayout = VkImageLayout.RENDERING_LOCAL_READ,
            loadOp = VkAttachmentLoadOp.CLEAR, storeOp = VkAttachmentStoreOp.DONT_CARE, clearValue = clear_color(0.5f, 0.5f, 0.5f, 0.0f)),
        <- rendering_color_attachment(res.color.view, clear_color(0.0f, 0.0f, 0.0f, 1.0f))]
    var depth_att <- rendering_depth_attachment(res.depth.view, clear_depth(1.0f))
    record_rendering(cmd, full_area(V14_W, V14_H), 1u, atts, depth_att) {
        set_pass_maps(cmd, GBUF_LOCATIONS, GBUF_INPUTS)
        cmd_bind_pipeline(cmd, res.gbuf_pipeline)
        var vbufs <- [weak_copy(res.vb.buffer)]
        var voffs <- [0ul]
        cmd_bind_vertex_buffers(cmd, 0u, vbufs, voffs)
        delete vbufs
        delete voffs
        cmd_bind_index_buffer(cmd, weak_copy(res.ib.buffer), 0ul, VkIndexType.UINT16)
        for (i in range(N_CUBES)) {
            pc.model = model_matrix(time * (0.6f + 0.3f * float(i)), float3(1.7f * float(i - 1), 0.0f, 0.0f))
            gbuf_vs_push_constants(cmd, res.gbuf_pipe_layout)
            push_cube_descriptors(res, cmd, i)
            cmd_draw_indexed(cmd, uint(N_INDICES), 1u, 0u, 0, 0u)
        }
        // the hand-off: attachment writes -> input-attachment reads, per region, inside the rendering
        var dep : DependencyInfo
        dep.dependencyFlags.by_region = true
        dep.pMemoryBarriers |> emplace(MemoryBarrier2(
            srcStageMask = VkPipelineStageFlags2.color_attachment_output, srcAccessMask = VkAccessFlags2.color_attachment_write,
            dstStageMask = VkPipelineStageFlags2.fragment_shader, dstAccessMask = VkAccessFlags2.input_attachment_read))
        cmd_pipeline_barrier2(cmd, dep)
        delete dep
        set_pass_maps(cmd, LIGHT_LOCATIONS, LIGHT_INPUTS)
        cmd_bind_pipeline(cmd, res.light_pipeline)
        light_fs_push_constants(cmd, res.light_pipe_layout)
        push_light_descriptors(res, cmd)
        cmd_draw(cmd, 3u)
    }
    delete atts
    delete depth_att
    transition_image2(cmd, res.color.image, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
        VkPipelineStageFlags2.color_attachment_output, VkAccessFlags2.color_attachment_write,
        VkPipelineStageFlags2.copy | VkPipelineStageFlags2.blit, VkAccessFlags2.transfer_read)
}

//! The lit frame at `time` as V14_W x V14_H RGBA8 pixels.
def public render_vulkan14_frame(var ctx : Vulkan14Context; time : float) : array<uint8> {
    update_vulkan14_uniforms(ctx.res, ctx.device, time)
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_vulkan14_frame(ctx.res, cmd, time)
        copy_image_to_buffer(cmd, ctx.res.color.image, ctx.res.readback, V14_W, V14_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! One-shot: a fresh context per call - a caller rendering many frames keeps one `Vulkan14Context` instead.
def public render_vulkan14_scene(time : float) : array<uint8> {
    var inscope ctx <- build_vulkan14_context()
    return <- render_vulkan14_frame(ctx, time)
}

//! Sample the RGB of pixel (x,y) from a V14_W*V14_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * V14_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
