options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 06 render path: draw a procedural cubemap skybox surrounding the camera. Six procedural
// faces (256x256 each) are uploaded once into a single VK_IMAGE_VIEW_TYPE_CUBE image; the skybox is a
// unit cube whose vertex position doubles as the world-space sampling direction. The view matrix
// passed to the shader has its translation column zeroed so the cube stays centred on the camera --
// the skybox appears at infinity even as the camera world position changes.

module skybox_tut

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require skybox_tut_shaders
require math

let public SKY_W = 768
let public SKY_H = 768
let SKY_DEPTH_FMT = VkFormat.D32_SFLOAT
let SKY_COLOR_FMT = VkFormat.R8G8B8A8_SRGB   // sRGB: gamma-encode the linear output on write (recording readback + window blit match)
let FACE_DIM = 256
let N_FACES = 6

let public N_VERTS = 8
let public N_INDICES = 36                  // 6 faces * 2 tris * 3 indices (CW from outside = CCW from inside)

// ===== cube geometry (positions only; the position IS the sampling direction) =====

//! 8 corners of a unit cube. Per-vertex layout: 3 floats (pos.xyz), stride 12.
let private cube_vertices : array<float> <- [
    -1.0, -1.0, -1.0,    // 0
     1.0, -1.0, -1.0,    // 1
     1.0,  1.0, -1.0,    // 2
    -1.0,  1.0, -1.0,    // 3
    -1.0, -1.0,  1.0,    // 4
     1.0, -1.0,  1.0,    // 5
     1.0,  1.0,  1.0,    // 6
    -1.0,  1.0,  1.0]    // 7

//! 36 indices, each face wound CCW **from inside the cube** (= CW from outside), because the v3d pipeline
//! culls back faces with CCW = front and the camera sits inside (the host strips the view's translation
//! column, gluing the cube to the camera). Faces verified by cross-producting the first two edges.
let private cube_indices : array<uint16> <- [
    // +X face (right, inward normal = -X): v1, v5, v6, v2
    uint16(1), uint16(5), uint16(6),  uint16(1), uint16(6), uint16(2),
    // -X face (left, inward normal = +X): v0, v3, v7, v4
    uint16(0), uint16(3), uint16(7),  uint16(0), uint16(7), uint16(4),
    // +Y face (top, inward normal = -Y): v2, v6, v7, v3
    uint16(2), uint16(6), uint16(7),  uint16(2), uint16(7), uint16(3),
    // -Y face (bottom, inward normal = +Y): v0, v4, v5, v1
    uint16(0), uint16(4), uint16(5),  uint16(0), uint16(5), uint16(1),
    // +Z face (front, inward normal = -Z): v4, v7, v6, v5
    uint16(4), uint16(7), uint16(6),  uint16(4), uint16(6), uint16(5),
    // -Z face (back, inward normal = +Z): v0, v1, v2, v3
    uint16(0), uint16(1), uint16(2),  uint16(0), uint16(2), uint16(3)]

// ===== reflective sphere + floor geometry (pos.xyz + normal.xyz, stride 24) =====

let SPHERE_LAT = 48
let SPHERE_LON = 64
let SPHERE_R = 1.0f
let SPHERE_CY = 1.05f                // sphere centre height (sits just above the floor at y=0)
let FLOOR_HALF = 9.0f

//! UV sphere centred at (0, SPHERE_CY, 0). Each vertex is position + outward unit normal. Rendered
//! with a no-cull pipeline so winding doesn't matter.
def gen_sphere(var verts : array<float>; var indices : array<uint16>) {
    verts |> reserve((SPHERE_LAT + 1) * (SPHERE_LON + 1) * 6)
    indices |> reserve(SPHERE_LAT * SPHERE_LON * 6)
    for (i in range(SPHERE_LAT + 1)) {
        let theta = float(i) / float(SPHERE_LAT) * PI
        let st = sin(theta)
        let ct = cos(theta)
        for (j in range(SPHERE_LON + 1)) {
            let phi = float(j) / float(SPHERE_LON) * 2.0f * PI
            let nx = st * cos(phi)
            let ny = ct
            let nz = st * sin(phi)
            verts |> push(nx * SPHERE_R); verts |> push(ny * SPHERE_R + SPHERE_CY); verts |> push(nz * SPHERE_R)
            verts |> push(nx); verts |> push(ny); verts |> push(nz)
        }
    }
    let row = SPHERE_LON + 1
    for (i in range(SPHERE_LAT)) {
        for (j in range(SPHERE_LON)) {
            let a = i * row + j
            let b = a + row
            indices |> push(uint16(a)); indices |> push(uint16(b)); indices |> push(uint16(a + 1))
            indices |> push(uint16(a + 1)); indices |> push(uint16(b)); indices |> push(uint16(b + 1))
        }
    }
}

//! Large floor quad in the XZ plane at y=0, normal up.
def gen_floor(var verts : array<float>; var indices : array<uint16>) {
    verts |> reserve(24)
    indices |> reserve(6)
    let h = FLOOR_HALF
    verts |> push(-h); verts |> push(0.0f); verts |> push(-h); verts |> push(0.0f); verts |> push(1.0f); verts |> push(0.0f)
    verts |> push(h); verts |> push(0.0f); verts |> push(-h); verts |> push(0.0f); verts |> push(1.0f); verts |> push(0.0f)
    verts |> push(h); verts |> push(0.0f); verts |> push(h); verts |> push(0.0f); verts |> push(1.0f); verts |> push(0.0f)
    verts |> push(-h); verts |> push(0.0f); verts |> push(h); verts |> push(0.0f); verts |> push(1.0f); verts |> push(0.0f)
    indices |> push(uint16(0)); indices |> push(uint16(1)); indices |> push(uint16(2))
    indices |> push(uint16(0)); indices |> push(uint16(2)); indices |> push(uint16(3))
}

// ===== matrix builders (column-major) =====

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

//! Strip the translation row from a view matrix so the rendered geometry appears at the same
//! relative position no matter where the camera is in the world. The skybox uses this so the cube
//! stays glued to the camera.
def strip_translation(var m : float4x4) : float4x4 {
    m[3] = float4(0.0f, 0.0f, 0.0f, 1.0f)
    return m
}

// ===== procedural cubemap =====

let public SUN_DIR : float3 = vec3_normalize(float3(0.45f, 0.55f, 0.85f))

//! Sample the sky at a unit direction: a two-tone vertical gradient (zenith blue -> warm horizon), a soft
//! falloff to the ground hemisphere, and a sun disc + Mie-style glow along SUN_DIR. Every cube face
//! rasterises this same continuous function over its own direction patch -- which is why seams vanish.
def sample_sky(dir : float3) : float3 {
    let altitude = clamp(dir.y, -1.0f, 1.0f)
    let elevation = clamp(altitude * 0.5f + 0.5f, 0.0f, 1.0f)
    let zenith = float3(0.18f, 0.32f, 0.70f)
    let horizon = float3(0.88f, 0.62f, 0.45f)
    var col = lerp(horizon, zenith, float3(elevation, elevation, elevation))
    if (altitude < 0.0f) {
        let ground = float3(0.10f, 0.07f, 0.05f)
        let g = clamp(-altitude * 1.6f, 0.0f, 1.0f)
        let g3 = float3(g, g, g)
        col = lerp(col, ground, g3)
    }
    let sun_dot = clamp(vec3_dot(dir, SUN_DIR), 0.0f, 1.0f)
    let glow = pow(sun_dot, 32.0f) * 0.9f
    col = col + float3(1.0f, 0.85f, 0.65f) * glow
    if (sun_dot > 0.998f) {
        col = float3(1.0f, 0.95f, 0.85f)        // the sun disc itself
    }
    return clamp(col, float3(0.0f, 0.0f, 0.0f), float3(1.0f, 1.0f, 1.0f))
}

//! Convert a face index + (s,t) in [0,1]^2 to a unit direction. Matches the Vulkan cubemap face
//! ordering used by VK_IMAGE_VIEW_TYPE_CUBE (layer 0..5 = +X, -X, +Y, -Y, +Z, -Z).
def face_direction(face, x, y, dim : int) : float3 {
    let s = (float(x) + 0.5f) / float(dim) * 2.0f - 1.0f
    let t = (float(y) + 0.5f) / float(dim) * 2.0f - 1.0f
    var d : float3
    if (face == 0) {                                // +X
        d = float3(1.0f, -t, -s)
    } elif (face == 1) {                            // -X
        d = float3(-1.0f, -t, s)
    } elif (face == 2) {                            // +Y
        d = float3(s, 1.0f, t)
    } elif (face == 3) {                            // -Y
        d = float3(s, -1.0f, -t)
    } elif (face == 4) {                            // +Z
        d = float3(s, -t, 1.0f)
    } else {                                        // -Z
        d = float3(-s, -t, -1.0f)
    }
    return vec3_normalize(d)
}

//! Build all 6 RGBA8 faces as one contiguous byte array, ready for a single vkCmdCopyBufferToImage
//! call with regions per layer.
def gen_skybox_pixels() : array<uint8> {
    var pixels : array<uint8>
    pixels |> resize(N_FACES * FACE_DIM * FACE_DIM * 4)
    for (face in range(N_FACES)) {
        let layer_base = face * FACE_DIM * FACE_DIM * 4
        for (y in range(FACE_DIM)) {
            for (x in range(FACE_DIM)) {
                let d = face_direction(face, x, y, FACE_DIM)
                let col = sample_sky(d)
                let p = layer_base + (y * FACE_DIM + x) * 4
                pixels[p + 0] = uint8(clamp(col.x * 255.0f, 0.0f, 255.0f))
                pixels[p + 1] = uint8(clamp(col.y * 255.0f, 0.0f, 255.0f))
                pixels[p + 2] = uint8(clamp(col.z * 255.0f, 0.0f, 255.0f))
                pixels[p + 3] = 255u8
            }
        }
    }
    return <- pixels
}

// ===== reusable per-frame context =====

//! Long-lived skybox state: offscreen color + depth, render pass + framebuffer, cube geometry
//! buffers, UBO, cubemap image + view + sampler, descriptors, the depth-trick pipeline, and the
//! readback buffer.
struct public SkyboxResources {
    color       : OffscreenTarget
    depth       : OffscreenDepth
    render_pass : RenderPass
    framebuffer : Framebuffer
    vb          : HostBuffer
    ib          : HostBuffer
    sphere_vb   : HostBuffer
    sphere_ib   : HostBuffer
    floor_vb    : HostBuffer
    floor_ib    : HostBuffer
    sphere_index_count : int
    floor_index_count  : int
    ubo         : HostBuffer
    sky_image   : Image
    sky_memory  : DeviceMemory
    sky_view    : ImageView
    sky_sampler : Sampler
    set_layouts : array<DescriptorSetLayout>
    desc_pool   : DescriptorPool
    desc_set    : VkDescriptorSet
    pipe_layout : PipelineLayout
    pipeline    : Pipeline
    sphere_pipeline : Pipeline
    floor_pipeline  : Pipeline
    readback    : HostBuffer
    buf_size    : uint64
}

def public finalize(var r : SkyboxResources) {
    unsafe {
        delete r.readback
        delete r.floor_pipeline
        delete r.sphere_pipeline
        delete r.pipeline
        delete r.pipe_layout
        delete r.desc_pool
        delete r.set_layouts
        delete r.sky_sampler
        delete r.sky_view
        delete r.sky_image
        delete r.sky_memory
        delete r.ubo
        delete r.floor_ib
        delete r.floor_vb
        delete r.sphere_ib
        delete r.sphere_vb
        delete r.ib
        delete r.vb
        delete r.framebuffer
        delete r.render_pass
        delete r.depth
        delete r.color
    }
}

struct public SkyboxContext {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : SkyboxResources
}

def public finalize(var ctx : SkyboxContext) {
    unsafe {
        delete ctx.res
        delete ctx.pool
        delete ctx.device
        delete ctx.instance
    }
}

//! Foreground (sphere/floor) pipeline: vertex + fragment, pos+normal vertex input (stride 24), depth
//! test + write LESS, NO culling (so the procedural sphere/floor winding is irrelevant). Shares the
//! skybox's pipeline layout (same UBO + cubemap bindings).
def private build_scene_pipeline(device : Device; rp : RenderPass; layout : PipelineLayout;   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
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
    a_pos.format = VkFormat.R32G32B32_SFLOAT
    a_pos.offset = 0u
    attrs |> push(a_pos)
    var a_norm = VkVertexInputAttributeDescription()
    a_norm.location = 1u
    a_norm.format = VkFormat.R32G32B32_SFLOAT
    a_norm.offset = 12u
    attrs |> push(a_norm)

    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = 1u
    vinput.vertexAttributeDescriptionCount = uint(length(attrs))

    var ia = VkPipelineInputAssemblyStateCreateInfo()
    ia.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var viewport = VkViewport()
    viewport.width = float(SKY_W)
    viewport.height = float(SKY_H)
    viewport.maxDepth = 1.0
    var scissor : VkRect2D
    scissor.extent.width = uint(SKY_W)
    scissor.extent.height = uint(SKY_H)
    var vp = VkPipelineViewportStateCreateInfo()
    vp.viewportCount = 1u
    vp.scissorCount = 1u

    var rast = VkPipelineRasterizationStateCreateInfo()
    rast.polygonMode = VkPolygonMode.FILL          // cullMode left 0 = VK_CULL_MODE_NONE
    rast.frontFace = VkFrontFace.COUNTER_CLOCKWISE
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
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! Build the offscreen targets, the cubemap (uploaded + transitioned to SHADER_READ_ONLY), and the
//! depth-trick pipeline against a caller-owned (device, phys, queue, pool). Used by both
//! build_skybox_context (offscreen test + recording) and window/show_skybox.das (swapchain blit).
def public build_skybox_resources(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool) : SkyboxResources {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    let dev = boost_value_to_vk(device)

    var inscope color <- build_offscreen_target(device, phys, SKY_W, SKY_H, SKY_COLOR_FMT)
    var inscope depth <- build_offscreen_depth(device, phys, SKY_W, SKY_H, SKY_DEPTH_FMT)
    var inscope render_pass <- create_render_pass_color_depth(device, SKY_COLOR_FMT, SKY_DEPTH_FMT)
    var inscope framebuffer <- create_framebuffer(device, FramebufferCreateInfo(
        renderPass = weak_copy(render_pass),
        pAttachments <- [weak_copy(color.view), weak_copy(depth.view)],
        width = uint(SKY_W),
        height = uint(SKY_H),
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

    // sphere + floor geometry (pos + normal, stride 24)
    var sphere_v : array<float>
    var sphere_i : array<uint16>
    gen_sphere(sphere_v, sphere_i)
    var inscope sphere_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, sphere_v)
    var inscope sphere_ib <- create_host_buffer_from_bytes(device, phys, ib_usage, sphere_i)
    let sphere_index_count = length(sphere_i)
    delete sphere_v
    delete sphere_i

    var floor_v : array<float>
    var floor_i : array<uint16>
    gen_floor(floor_v, floor_i)
    var inscope floor_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, floor_v)
    var inscope floor_ib <- create_host_buffer_from_bytes(device, phys, ib_usage, floor_i)
    let floor_index_count = length(floor_i)
    delete floor_v
    delete floor_i

    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    var inscope ubo <- create_host_buffer(device, phys, 224ul, ubo_usage)        // view+proj+view_full (192) + cam_pos (16) + material (16)

    // 6-face cubemap: build the procedural sky on the host and stage it through one
    // HOST_VISIBLE/HOST_COHERENT buffer into a device-local cube image.
    var sky_pixels <- gen_skybox_pixels()
    var staging_usage : VkBufferUsageFlags
    staging_usage.transfer_src = true
    var inscope staging <- create_host_buffer_from_bytes(device, phys, staging_usage, sky_pixels)
    delete sky_pixels

    var ici : ImageCreateInfo
    ici.flags.cube_compatible = true                     // <-- required for VK_IMAGE_VIEW_TYPE_CUBE
    ici.imageType = VkImageType._2D
    ici.format = SKY_COLOR_FMT
    ici.extent.width = uint(FACE_DIM)
    ici.extent.height = uint(FACE_DIM)
    ici.extent.depth = 1u
    ici.mipLevels = 1u
    ici.arrayLayers = uint(N_FACES)                       // 6 faces, one per array layer
    ici.samples._1 = true
    ici.tiling = VkImageTiling.OPTIMAL
    ici.usage.sampled = true
    ici.usage.transfer_dst = true
    ici.initialLayout = VkImageLayout.UNDEFINED
    var inscope sky_image <- create_image(device, ici)
    var ireq : VkMemoryRequirements
    vkGetImageMemoryRequirements(dev, boost_value_to_vk(sky_image), ireq)
    var iwant : VkMemoryPropertyFlags
    iwant.device_local = true
    let imai = MemoryAllocateInfo(
        allocationSize = ireq.size,
        memoryTypeIndex = find_memory_type(phys, ireq.memoryTypeBits, iwant))
    var inscope sky_memory <- allocate_memory(device, imai)
    vk_check(vkBindImageMemory(dev, boost_value_to_vk(sky_image), boost_value_to_vk(sky_memory), 0ul), null)

    var tvci = ImageViewCreateInfo(
        image = weak_copy(sky_image),
        viewType = VkImageViewType.CUBE,                 // <-- the cubemap rail
        format = SKY_COLOR_FMT)
    tvci.subresourceRange.aspectMask.color = true
    tvci.subresourceRange.levelCount = 1u
    tvci.subresourceRange.layerCount = uint(N_FACES)
    var inscope sky_view <- create_image_view(device, tvci)

    let sci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.LINEAR,
        addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE,
        maxLod = 1.0f)
    var inscope sky_sampler <- create_sampler(device, sci)

    // Include the scene shaders' reflections so binding 0 (UBO) picks up the FRAGMENT stage flag
    // (sphere_fs / floor_fs read cam.cam_pos + cam.material) and binding 1 (cubemap) stays fragment.
    var reflections <- [decode_reflection(skybox_vert_spv_reflect), decode_reflection(skybox_frag_spv_reflect),
                        decode_reflection(scene_vert_spv_reflect), decode_reflection(sphere_frag_spv_reflect),
                        decode_reflection(floor_frag_spv_reflect)]
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
    w0.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(ubo.buffer), range_ = 224ul))
    writes |> emplace(w0)
    var w1 = WriteDescriptorSet(
        dstSet = vk_value_to_boost(raw_set),
        dstBinding = 1u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER,
        descriptorCount = 1u)
    w1.pImageInfo |> push(DescriptorImageInfo(
        sampler = weak_copy(sky_sampler),
        imageView = weak_copy(sky_view),
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL))
    writes |> emplace(w1)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    var inscope pipe_layout <- build_pipeline_layout(device, set_layouts, reflections)
    delete reflections

    var inscope vert <- create_shader_module(device, skybox_vert_spv)
    var inscope frag <- create_shader_module(device, skybox_frag_spv)

    var vbindings : array<VkVertexInputBindingDescription>
    var vb_desc : VkVertexInputBindingDescription
    vb_desc.binding = 0u
    vb_desc.stride = 12u
    vb_desc.inputRate = VkVertexInputRate.VERTEX
    vbindings |> push(vb_desc)
    var vattrs : array<VkVertexInputAttributeDescription>
    var attr_pos : VkVertexInputAttributeDescription
    attr_pos.location = 0u
    attr_pos.binding = 0u
    attr_pos.format = VkFormat.R32G32B32_SFLOAT
    attr_pos.offset = 0u
    vattrs |> push(attr_pos)
    // depth-trick pipeline: test LESS_OR_EQUAL, write disabled. The vertex shader's z=w snap puts every
    // skybox fragment at NDC z=1.0, so the test passes against a cleared 1.0 depth but never overwrites
    // it -- foreground geometry (or a re-ordered scene) keeps its own z values intact.
    var inscope pipeline <- create_graphics_pipeline_v3d(device, render_pass, pipe_layout, vert, frag,
        SKY_W, SKY_H, vbindings, vattrs, /*depth_test_enable=*/true, /*depth_write_enable=*/false,
        VkCompareOp.LESS_OR_EQUAL)
    delete vbindings
    delete vattrs

    // foreground pipelines: one shared scene vertex shader, two fragment shaders (metal sphere + floor)
    var inscope scene_vert  <- create_shader_module(device, scene_vert_spv)
    var inscope sphere_frag <- create_shader_module(device, sphere_frag_spv)
    var inscope floor_frag  <- create_shader_module(device, floor_frag_spv)
    var inscope sphere_pipeline <- build_scene_pipeline(device, render_pass, pipe_layout, scene_vert, sphere_frag)
    var inscope floor_pipeline  <- build_scene_pipeline(device, render_pass, pipe_layout, scene_vert, floor_frag)

    let buf_size = uint64(SKY_W * SKY_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    // one-shot cubemap upload: staging -> sky_image (all 6 layers in one copy), transitioned
    // UNDEFINED -> TRANSFER_DST -> SHADER_READ_ONLY across all layers.
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
        // transition_image only takes the layout pair; the cube transitions across all 6 layers
        // because the boost helper uses the full subresource range.
        transition_image(cmd, sky_image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            none, to_dst, top, xfer)
        // One vkCmdCopyBufferToImage call with 6 regions, one per array layer. The staging buffer
        // has the faces laid out contiguously in the order (+X, -X, +Y, -Y, +Z, -Z).
        var regions : array<BufferImageCopy>
        for (face in range(N_FACES)) {
            var region : BufferImageCopy
            region.bufferOffset = uint64(face * FACE_DIM * FACE_DIM * 4)
            region.imageSubresource.aspectMask.color = true
            region.imageSubresource.baseArrayLayer = uint(face)
            region.imageSubresource.layerCount = 1u
            region.imageExtent.width = uint(FACE_DIM)
            region.imageExtent.height = uint(FACE_DIM)
            region.imageExtent.depth = 1u
            regions |> emplace(region)
        }
        cmd_copy_buffer_to_image(cmd, staging.buffer, sky_image, VkImageLayout.TRANSFER_DST_OPTIMAL, regions)
        transition_image(cmd, sky_image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
            to_dst, to_read, xfer, frag_stage)
    }

    return <- SkyboxResources(
        color <- color, depth <- depth, render_pass <- render_pass, framebuffer <- framebuffer,
        vb <- vb, ib <- ib,
        sphere_vb <- sphere_vb, sphere_ib <- sphere_ib, floor_vb <- floor_vb, floor_ib <- floor_ib,
        sphere_index_count = sphere_index_count, floor_index_count = floor_index_count,
        ubo <- ubo,
        sky_image <- sky_image, sky_memory <- sky_memory, sky_view <- sky_view, sky_sampler <- sky_sampler,
        set_layouts <- set_layouts, desc_pool <- desc_pool, desc_set = raw_set,
        pipe_layout <- pipe_layout, pipeline <- pipeline,
        sphere_pipeline <- sphere_pipeline, floor_pipeline <- floor_pipeline,
        readback <- readback,
        buf_size = buf_size)
}

//! Self-contained one-shot context (own instance + device + queue + pool + resources). Used by the
//! [test] and the recording driver; windowed drivers build their own surface-enabled instance and
//! call build_skybox_resources directly.
def public build_skybox_context() : SkyboxContext {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 06", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope res <- build_skybox_resources(device, phys, queue, pool)
    return <- SkyboxContext(
        instance <- instance, device <- device, queue = queue, pool <- pool, res <- res)
}

//! Metal presets the sphere morphs through: float4(tint.rgb, reflectivity).
let METAL_PRESETS : array<float4> <- [
    float4(0.95f, 0.96f, 0.98f, 1.00f),   // chrome
    float4(1.00f, 0.78f, 0.34f, 0.95f),   // gold
    float4(0.95f, 0.55f, 0.38f, 0.92f),   // copper
    float4(0.70f, 0.72f, 0.76f, 0.78f)]   // brushed steel

//! Lerp through the metal presets, ~3 s per preset, looping. Drives the sphere's material over time.
def cycle_metal(time : float) : float4 {
    let period = 3.0f
    let n = length(METAL_PRESETS)
    let t = time / period
    let idx = int(floor(t)) % n
    let nxt = (idx + 1) % n
    let f = t - floor(t)
    return lerp(METAL_PRESETS[idx], METAL_PRESETS[nxt], float4(f, f, f, f))
}

//! Host-side per-frame prep: orbit the camera around the sphere (slightly elevated, looking at it),
//! push the rotation-only view (skybox) + full view + camera pos + cycling material into the UBO.
def public update_skybox_uniforms(res : SkyboxResources; device : Device; time, camera_t : float) {
    let camera_angle = camera_t * 2.0f * PI
    let cam_x = cos(camera_angle) * 4.5f
    let cam_z = sin(camera_angle) * 4.5f
    let cam_y = 2.2f + sin(camera_angle * 2.0f) * 0.6f      // elevated, gentle sway
    let cam_eye = float3(cam_x, cam_y, cam_z)
    var view = look_at_rh(cam_eye, float3(0.0f, 0.9f, 0.0f), float3(0.0f, 1.0f, 0.0f))
    cam.view_full = view                                     // full view (COPY before strip_translation mutates `view`)
    cam.view = strip_translation(view)                       // skybox: rotation only (strips `view` in place)
    cam.proj = perspective_vk(70.0f * PI / 180.0f, float(SKY_W) / float(SKY_H), 0.1f, 50.0f)
    cam.cam_pos = float4(cam_eye.x, cam_eye.y, cam_eye.z, 0.0f)
    cam.material = cycle_metal(time)
    skybox_vs_bind_uniform(device, res.ubo.memory)          // writes the whole cam block
}

//! Record the skybox draw into the given command buffer. The render pass ends with the colour
//! attachment in TRANSFER_SRC_OPTIMAL (boost default), so the windowed driver can blit the result
//! straight onto the swapchain image.
def public record_skybox_render_pass(res : SkyboxResources; cmd : CommandBuffer) {
    var clears <- [clear_color(0.0f, 0.0f, 0.0f, 1.0f), clear_depth(1.0f)]
    record_render_pass(cmd, res.render_pass, res.framebuffer, full_area(SKY_W, SKY_H), clears) {
        let sets <- [vk_value_to_boost(res.desc_set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.pipe_layout, 0u, sets, no_dyn)
        var voffs : array<uint64>
        voffs |> push(0ul)

        // foreground first (depth write): reflective sphere, then checker floor
        cmd_bind_pipeline(cmd, res.sphere_pipeline)
        let sphere_vbufs <- [weak_copy(res.sphere_vb.buffer)]
        cmd_bind_vertex_buffers(cmd, 0u, sphere_vbufs, voffs)
        cmd_bind_index_buffer(cmd, weak_copy(res.sphere_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, uint(res.sphere_index_count), 1u, 0u, 0, 0u)

        cmd_bind_pipeline(cmd, res.floor_pipeline)
        let floor_vbufs <- [weak_copy(res.floor_vb.buffer)]
        cmd_bind_vertex_buffers(cmd, 0u, floor_vbufs, voffs)
        cmd_bind_index_buffer(cmd, weak_copy(res.floor_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, uint(res.floor_index_count), 1u, 0u, 0, 0u)

        // skybox last: its z=1.0 trick + LESS_OR_EQUAL fills only where no foreground was drawn
        cmd_bind_pipeline(cmd, res.pipeline)
        let vbufs <- [weak_copy(res.vb.buffer)]
        cmd_bind_vertex_buffers(cmd, 0u, vbufs, voffs)
        cmd_bind_index_buffer(cmd, weak_copy(res.ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, uint(N_INDICES), 1u, 0u, 0, 0u)
    }
    delete clears
}

//! Per-frame work: update UBO, record the skybox draw, copy the colour attachment to the readback
//! buffer, clone the bytes out as RGBA8.
def public render_skybox_frame(var ctx : SkyboxContext; time, camera_t : float) : array<uint8> {
    update_skybox_uniforms(ctx.res, ctx.device, time, camera_t)
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_skybox_render_pass(ctx.res, cmd)
        copy_image_to_buffer(cmd, ctx.res.color.image, ctx.res.readback, SKY_W, SKY_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! One-shot: build a context and render one frame.
def public render_skybox(time, camera_t : float) : array<uint8> {
    var inscope ctx <- build_skybox_context()
    return <- render_skybox_frame(ctx, time, camera_t)
}

//! Sample the RGB of pixel (x,y) from a SKY_W*SKY_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * SKY_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
