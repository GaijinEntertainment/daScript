options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 11 render path: HDR scene -> Karis-style 5-mip bloom -> ACES-tonemapped LDR.
//
// Eleven rendering passes per frame, each a record_rendering on the target view:
//   * 1 x scene pass (HDR_W x HDR_H, R16G16B16A16_SFLOAT)
//   * 1 x bright pass (BLOOM_W[0] x BLOOM_H[0], R16G16B16A16_SFLOAT)
//   * 4 x downsample passes (bloom[i] -> bloom[i+1])
//   * 4 x upsample passes (bloom[i+1] added onto bloom[i], loadOp = LOAD)
//   * 1 x composite pass (HDR_W x HDR_H LDR target, R8G8B8A8_UNORM)
//
// A render pass carries these hand-offs as external subpass dependencies; here each is one explicit
// barrier: a target goes COLOR_ATTACHMENT_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL when the next pass
// samples it, and back when the upsample draws onto it again. Each barrier names the stage that wrote
// and the stage that reads, so every hand-off inside the frame is explicit.
//
// Five pipelines: scene_pipeline (instanced vertex+fragment, depth-tested), bright_pipeline +
// downsample_pipeline (fullscreen-tri, no depth, no blend) and upsample_pipeline (fullscreen-tri,
// no depth, ADDITIVE blend on RGB, alpha-write masked off) and composite_pipeline (fullscreen-tri,
// no depth, no blend). The three bloom passes -- bright, down, up -- use dynamic viewport because
// the same pipeline runs at every mip resolution; the composite pipeline is fixed at HDR_W x HDR_H
// so it skips dynamic viewport.

module hdr_tut

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require hdr_tut_shaders public
require math

let public HDR_W = 768
let public HDR_H = 768
let HDR_FMT      = VkFormat.R16G16B16A16_SFLOAT
let DEPTH_FMT    = VkFormat.D32_SFLOAT
// LDR target holds *linear* post-tonemap values in [0,1]: the composite shader does ACES but does
// NOT gamma-encode. Gamma comes later and free, from the sRGB swap format at blit time (~12 bits of
// dark-region precision = no banding). show_hdr.das asserts the swap format is one of the two sRGB.
let LDR_FMT      = VkFormat.R16G16B16A16_SFLOAT
// Test readback path uses a sibling 8-bit-sRGB image: composite (linear 16-bit float) is blitted
// here with hardware sRGB encoding, then copied to a host buffer. Existing tests assume the host
// buffer holds gamma-encoded 8-bit RGBA; this preserves that contract.
let READBACK_FMT = VkFormat.R8G8B8A8_SRGB

let public BLOOM_LEVELS = 5
// width / height per mip: 1/2, 1/4, 1/8, 1/16, 1/32 of HDR_W
let public BLOOM_W : array<int> <- [HDR_W / 2, HDR_W / 4, HDR_W / 8, HDR_W / 16, HDR_W / 32]
let public BLOOM_H : array<int> <- [HDR_H / 2, HDR_H / 4, HDR_H / 8, HDR_H / 16, HDR_H / 32]

let public N_INSTANCES = 1000
let public N_VERTS     = 24
let public N_INDICES   = 36

// per-vertex stride: pos + normal = 6 floats = 24 bytes
let VERT_STRIDE = 24u
// per-instance stride: offset + colour + phase + emissive = 8 floats = 32 bytes
let INST_STRIDE = 32u
let INST_FLOATS_PER = 8
let EVERY_NTH_EMISSIVE = 7              // 1 in 7 cubes glows

// ===== cube geometry (24 verts, 36 indices; faceted normals) =====

let private cube_vertices : array<float> <- [
    -0.5, -0.5, 0.5,   0.0, 0.0, 1.0,
     0.5, -0.5, 0.5,   0.0, 0.0, 1.0,
     0.5,  0.5, 0.5,   0.0, 0.0, 1.0,
    -0.5,  0.5, 0.5,   0.0, 0.0, 1.0,
     0.5, -0.5, -0.5,  0.0, 0.0, -1.0,
    -0.5, -0.5, -0.5,  0.0, 0.0, -1.0,
    -0.5,  0.5, -0.5,  0.0, 0.0, -1.0,
     0.5,  0.5, -0.5,  0.0, 0.0, -1.0,
     0.5, -0.5,  0.5,  1.0, 0.0, 0.0,
     0.5, -0.5, -0.5,  1.0, 0.0, 0.0,
     0.5,  0.5, -0.5,  1.0, 0.0, 0.0,
     0.5,  0.5,  0.5,  1.0, 0.0, 0.0,
    -0.5, -0.5, -0.5, -1.0, 0.0, 0.0,
    -0.5, -0.5,  0.5, -1.0, 0.0, 0.0,
    -0.5,  0.5,  0.5, -1.0, 0.0, 0.0,
    -0.5,  0.5, -0.5, -1.0, 0.0, 0.0,
    -0.5,  0.5,  0.5,  0.0, 1.0, 0.0,
     0.5,  0.5,  0.5,  0.0, 1.0, 0.0,
     0.5,  0.5, -0.5,  0.0, 1.0, 0.0,
    -0.5,  0.5, -0.5,  0.0, 1.0, 0.0,
    -0.5, -0.5, -0.5,  0.0, -1.0, 0.0,
     0.5, -0.5, -0.5,  0.0, -1.0, 0.0,
     0.5, -0.5,  0.5,  0.0, -1.0, 0.0,
    -0.5, -0.5,  0.5,  0.0, -1.0, 0.0
]

let private cube_indices : array<uint16> <- [
    uint16(0),  uint16(1),  uint16(2),   uint16(0),  uint16(2),  uint16(3),
    uint16(4),  uint16(5),  uint16(6),   uint16(4),  uint16(6),  uint16(7),
    uint16(8),  uint16(9),  uint16(10),  uint16(8),  uint16(10), uint16(11),
    uint16(12), uint16(13), uint16(14),  uint16(12), uint16(14), uint16(15),
    uint16(16), uint16(17), uint16(18),  uint16(16), uint16(18), uint16(19),
    uint16(20), uint16(21), uint16(22),  uint16(20), uint16(22), uint16(23)
]

// ===== matrix builders (column-major) =====

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

// ===== per-instance buffer init =====

//! Build N_INSTANCES rows of (offset.xyz, color.rgb, phase, emissive). Offsets sample a Lissajous-
//! style 3D curve; colours cycle hue thrice; phase is uniform [0,1); emissive = 1.0 every Nth cube.
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
        let hue = fi * 6.0f * PI
        let cr = 0.5f + 0.5f * cos(hue)
        let cg = 0.5f + 0.5f * cos(hue + 2.0944f)
        let cb = 0.5f + 0.5f * cos(hue + 4.1888f)
        let emissive = (i % EVERY_NTH_EMISSIVE == 0) ? 1.0f : 0.0f
        let base = i * INST_FLOATS_PER
        data[base + 0] = x
        data[base + 1] = y
        data[base + 2] = z
        data[base + 3] = cr
        data[base + 4] = cg
        data[base + 5] = cb
        data[base + 6] = fi
        data[base + 7] = emissive
    }
    return <- data
}

// ===== resources =====

struct public HdrResources {
    hdr_color   : OffscreenTarget
    hdr_depth   : OffscreenDepth
    bloom       : array<OffscreenTarget>   // BLOOM_LEVELS entries
    ldr_color    : OffscreenTarget        // 16-bit float linear post-tonemap (the composite output)
    readback_8bit : OffscreenTarget       // 8-bit sRGB intermediate for the static-test readback path

    sampler_linear_clamp : Sampler

    cube_vb   : HostBuffer
    cube_ib   : HostBuffer
    inst_buf  : HostBuffer
    ubo       : HostBuffer

    scene_set_layouts    : array<DescriptorSetLayout>
    scene_pipe_layout    : PipelineLayout
    scene_pipeline       : Pipeline
    scene_set            : VkDescriptorSet

    post_set_layouts     : array<DescriptorSetLayout>      // index 0 = scene (UBO), index 1 = post (1 sampler)
    post_pipe_layout     : PipelineLayout
    bright_pipeline      : Pipeline
    down_pipeline        : Pipeline
    up_pipeline          : Pipeline                         // additive blend
    bright_set           : VkDescriptorSet                  // reads HDR scene
    down_set             : array<VkDescriptorSet>           // BLOOM_LEVELS-1 entries (mip i -> i+1)
    up_set               : array<VkDescriptorSet>           // BLOOM_LEVELS-1 entries (mip i+1 -> i)

    composite_set_layouts : array<DescriptorSetLayout>     // 2 samplers
    composite_pipe_layout : PipelineLayout
    composite_pipeline    : Pipeline
    composite_set         : VkDescriptorSet

    desc_pool : DescriptorPool

    readback  : HostBuffer
    buf_size  : uint64
}

def public finalize(var r : HdrResources) {
    delete r.readback
    delete r.desc_pool
    delete r.composite_pipeline
    delete r.composite_pipe_layout
    delete r.composite_set_layouts
    delete r.up_pipeline
    delete r.down_pipeline
    delete r.bright_pipeline
    delete r.post_pipe_layout
    delete r.post_set_layouts
    delete r.scene_pipeline
    delete r.scene_pipe_layout
    delete r.scene_set_layouts
    delete r.ubo
    delete r.inst_buf
    delete r.cube_ib
    delete r.cube_vb
    delete r.sampler_linear_clamp
    delete r.readback_8bit
    delete r.ldr_color
    delete r.bloom
    delete r.hdr_depth
    delete r.hdr_color
}

struct public HdrContext {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : HdrResources
}

def public finalize(var ctx : HdrContext) {
    delete ctx.res
    delete ctx.pool
    delete ctx.device
    delete ctx.instance
}

// ===== build =====

def public build_hdr_resources(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool) : HdrResources {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    let _unused_queue = queue
    let _unused_pool = pool

    // ----- offscreen targets -----
    var samples1 : VkSampleCountFlags
    samples1._1 = true
    var hdr_extra : VkImageUsageFlags
    hdr_extra.sampled = true                            // HDR scene is sampled by bright + composite
    var inscope hdr_color <- build_offscreen_target(device, phys, HDR_W, HDR_H, HDR_FMT, samples1, hdr_extra)
    let depth_extra : VkImageUsageFlags
    var inscope hdr_depth <- build_offscreen_depth(device, phys, HDR_W, HDR_H, DEPTH_FMT, depth_extra)

    var bloom_extra : VkImageUsageFlags
    bloom_extra.sampled = true                          // bloom mips are sampled by down + up + composite
    var inscope bloom : array<OffscreenTarget>
    bloom |> reserve(BLOOM_LEVELS)
    for (i in range(BLOOM_LEVELS)) {
        var inscope mip <- build_offscreen_target(device, phys, BLOOM_W[i], BLOOM_H[i], HDR_FMT, samples1, bloom_extra)
        bloom |> emplace(mip)
    }

    var ldr_extra : VkImageUsageFlags
    ldr_extra.transfer_src = true                   // composite output is blitted (sRGB conversion) to swap or to readback_8bit
    var inscope ldr_color <- build_offscreen_target(device, phys, HDR_W, HDR_H, LDR_FMT, samples1, ldr_extra)
    // build_offscreen_target always adds `color_attachment`, which the readback intermediate (only
    // a blit-dst + blit-src) doesn't need. The minimal usage set keeps us portable to drivers that
    // report `R8G8B8A8_SRGB` as supporting `BLIT_*` but not `COLOR_ATTACHMENT` -- rare, but legal.
    var inscope readback_8bit <- build_readback_image(device, phys, HDR_W, HDR_H, READBACK_FMT)

    // ----- sampler -----
    // mipmapMode LINEAR (NEAREST broke linear filtering in practice); maxLod 1.0 covers the single mip
    let sci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.LINEAR,
        addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE,
        maxLod = 1.0f)
    var inscope sampler_linear_clamp <- create_sampler(device, sci)

    // ----- geometry + UBOs -----
    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var ib_usage : VkBufferUsageFlags
    ib_usage.index_buffer = true
    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true

    var cv_copy := cube_vertices
    var inscope cube_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, cv_copy)
    delete cv_copy
    var ci_copy := cube_indices
    var inscope cube_ib <- create_host_buffer_from_bytes(device, phys, ib_usage, ci_copy)
    delete ci_copy

    var inst_data <- gen_instance_data(N_INSTANCES)
    var inscope inst_buf <- create_host_buffer_from_bytes(device, phys, vb_usage, inst_data)
    delete inst_data

    var inscope ubo <- create_host_buffer(device, phys, 144ul, ubo_usage)        // view + proj + cam_time

    // ----- reflection -----
    var scene_refl <- [decode_reflection(hdr_scene_vert_spv_reflect), decode_reflection(hdr_scene_frag_spv_reflect)]
    var inscope scene_set_layouts <- build_descriptor_set_layouts(device, scene_refl)
    var inscope scene_pipe_layout <- build_pipeline_layout(device, scene_set_layouts, scene_refl)

    var bright_refl <- [decode_reflection(hdr_fullscreen_vert_spv_reflect), decode_reflection(hdr_bright_frag_spv_reflect)]
    var inscope post_set_layouts <- build_descriptor_set_layouts(device, bright_refl)
    var inscope post_pipe_layout <- build_pipeline_layout(device, post_set_layouts, bright_refl)
    delete bright_refl

    var comp_refl <- [decode_reflection(hdr_fullscreen_vert_spv_reflect), decode_reflection(hdr_composite_frag_spv_reflect)]
    var inscope composite_set_layouts <- build_descriptor_set_layouts(device, comp_refl)
    var inscope composite_pipe_layout <- build_pipeline_layout(device, composite_set_layouts, comp_refl)
    delete comp_refl

    // ----- descriptor pool sized for all sets -----
    let n_post_samples = uint(1 + 2 * (BLOOM_LEVELS - 1))     // bright + down(4) + up(4) = 9
    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u + n_post_samples + 1u                   // scene + post + composite
    let p_ubo = DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 1u)
    dpci.pPoolSizes |> push(p_ubo)
    // post-sets each have one sampler + composite has two
    let p_smp = DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = n_post_samples + 2u)
    dpci.pPoolSizes |> push(p_smp)
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    // ----- allocate descriptor sets -----
    var scene_set     = alloc_one_set(device, desc_pool, scene_set_layouts[0])
    var bright_set    = alloc_one_set(device, desc_pool, post_set_layouts[1])      // post-sampler is set 1
    var composite_set = alloc_one_set(device, desc_pool, composite_set_layouts[1]) // composite samplers in set 1
    var down_set <- [for (_ in range(BLOOM_LEVELS - 1)); alloc_one_set(device, desc_pool, post_set_layouts[1])]
    var up_set   <- [for (_ in range(BLOOM_LEVELS - 1)); alloc_one_set(device, desc_pool, post_set_layouts[1])]

    // ----- writes -----
    var writes : array<WriteDescriptorSet>
    writes |> reserve(4 + 2 * (BLOOM_LEVELS - 1))     // ubo + bright + composite(2) + down/up per mip
    writes |> emplace(write_ubo(scene_set, 0u, ubo.buffer, 144ul))
    writes |> emplace(write_sampler(bright_set, 0u, hdr_color.view, sampler_linear_clamp))
    for (i in range(BLOOM_LEVELS - 1)) {
        writes |> emplace(write_sampler(down_set[i], 0u, bloom[i].view, sampler_linear_clamp))
        writes |> emplace(write_sampler(up_set[i],   0u, bloom[i + 1].view, sampler_linear_clamp))
    }
    writes |> emplace(write_sampler(composite_set, 0u, hdr_color.view, sampler_linear_clamp))
    writes |> emplace(write_sampler(composite_set, 1u, bloom[0].view,  sampler_linear_clamp))
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    // ----- shaders + pipelines -----
    var inscope sh_scene_v <- create_shader_module(device, hdr_scene_vert_spv)
    var inscope sh_scene_f <- create_shader_module(device, hdr_scene_frag_spv)
    var inscope sh_full_v  <- create_shader_module(device, hdr_fullscreen_vert_spv)
    var inscope sh_bright  <- create_shader_module(device, hdr_bright_frag_spv)
    var inscope sh_down    <- create_shader_module(device, hdr_down_frag_spv)
    var inscope sh_up      <- create_shader_module(device, hdr_up_frag_spv)
    var inscope sh_comp    <- create_shader_module(device, hdr_composite_frag_spv)

    var vbindings <- [
        VkVertexInputBindingDescription(binding = 0u, stride = VERT_STRIDE, inputRate = VkVertexInputRate.VERTEX),
        VkVertexInputBindingDescription(binding = 1u, stride = INST_STRIDE, inputRate = VkVertexInputRate.INSTANCE)]
    var vattrs <- [
        VkVertexInputAttributeDescription(location = 0u, binding = 0u, format = VkFormat.R32G32B32_SFLOAT, offset = 0u),
        VkVertexInputAttributeDescription(location = 1u, binding = 0u, format = VkFormat.R32G32B32_SFLOAT, offset = 12u),
        VkVertexInputAttributeDescription(location = 2u, binding = 1u, format = VkFormat.R32G32B32_SFLOAT, offset = 0u),
        VkVertexInputAttributeDescription(location = 3u, binding = 1u, format = VkFormat.R32G32B32_SFLOAT, offset = 12u),
        VkVertexInputAttributeDescription(location = 4u, binding = 1u, format = VkFormat.R32_SFLOAT,       offset = 24u),
        VkVertexInputAttributeDescription(location = 5u, binding = 1u, format = VkFormat.R32_SFLOAT,       offset = 28u)]

    var inscope scene_pipeline <- create_graphics_pipeline_v3d(device, HDR_FMT, DEPTH_FMT, scene_pipe_layout, sh_scene_v, sh_scene_f,
        HDR_W, HDR_H, vbindings, vattrs)
    delete vbindings
    delete vattrs

    // every post pipeline names the format of the target it draws into
    var inscope bright_pipeline <- create_graphics_pipeline_simple(device, HDR_FMT, post_pipe_layout, sh_full_v, sh_bright,
        HDR_W, HDR_H, /*dynamic_viewport*/true)
    var inscope down_pipeline   <- create_graphics_pipeline_simple(device, HDR_FMT, post_pipe_layout, sh_full_v, sh_down,
        HDR_W, HDR_H, /*dynamic_viewport*/true)
    var inscope up_pipeline     <- build_additive_pipeline(device, post_pipe_layout, sh_full_v, sh_up)
    var inscope composite_pipeline <- create_graphics_pipeline_simple(device, LDR_FMT, composite_pipe_layout,
        sh_full_v, sh_comp, HDR_W, HDR_H)

    delete scene_refl

    let buf_size = uint64(HDR_W * HDR_H * 4)
    var inscope readback <- create_host_buffer(device, phys, buf_size)

    return <- HdrResources(
        hdr_color <- hdr_color,
        hdr_depth <- hdr_depth,
        bloom <- bloom,
        ldr_color <- ldr_color,
        readback_8bit <- readback_8bit,
        sampler_linear_clamp <- sampler_linear_clamp,
        cube_vb <- cube_vb,
        cube_ib <- cube_ib,
        inst_buf <- inst_buf,
        ubo <- ubo,
        scene_set_layouts <- scene_set_layouts,
        scene_pipe_layout <- scene_pipe_layout,
        scene_pipeline <- scene_pipeline,
        scene_set = scene_set,
        post_set_layouts <- post_set_layouts,
        post_pipe_layout <- post_pipe_layout,
        bright_pipeline <- bright_pipeline,
        down_pipeline <- down_pipeline,
        up_pipeline <- up_pipeline,
        bright_set = bright_set,
        down_set <- down_set,
        up_set <- up_set,
        composite_set_layouts <- composite_set_layouts,
        composite_pipe_layout <- composite_pipe_layout,
        composite_pipeline <- composite_pipeline,
        composite_set = composite_set,
        desc_pool <- desc_pool,
        readback <- readback,
        buf_size = buf_size)
}

// ===== helpers =====

def private alloc_one_set(device : Device; pool : DescriptorPool; layout : DescriptorSetLayout) : VkDescriptorSet {
    let dev = boost_value_to_vk(device)
    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(pool)
    dsai.descriptorSetCount = 1u
    var raw_layout = boost_value_to_vk(layout)
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }
    return raw_set
}

def private write_ubo(set : VkDescriptorSet; binding : uint; buf : Buffer; size : uint64) : WriteDescriptorSet {
    var w = WriteDescriptorSet(
        dstSet = vk_value_to_boost(set),
        dstBinding = binding,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER,
        descriptorCount = 1u)
    let bi = DescriptorBufferInfo(buffer = weak_copy(buf), range_ = size)
    w.pBufferInfo |> push(bi)
    return <- w
}

def private write_sampler(set : VkDescriptorSet; binding : uint; view : ImageView; sampler : Sampler) : WriteDescriptorSet {
    var w = WriteDescriptorSet(
        dstSet = vk_value_to_boost(set),
        dstBinding = binding,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER,
        descriptorCount = 1u)
    let ii = DescriptorImageInfo(
        sampler = weak_copy(sampler),
        imageView = weak_copy(view),
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    w.pImageInfo |> push(ii)
    return <- w
}

//! Minimal-usage offscreen image (TRANSFER_SRC + TRANSFER_DST, no color_attachment), sized 1:1 with
//! the LDR target. We only blit through it, and `build_offscreen_target` -- which always sets
//! color_attachment -- would fail `vkCreateImage` where `R8G8B8A8_SRGB` blits but can't be a target.
def private build_readback_image(device : Device; phys : VkPhysicalDevice;
                                 width, height : int; format : VkFormat) : OffscreenTarget {
    var ici = ImageCreateInfo(
        imageType = VkImageType._2D,
        format = format,
        mipLevels = 1u,
        arrayLayers = 1u,
        tiling = VkImageTiling.OPTIMAL,
        initialLayout = VkImageLayout.UNDEFINED)
    ici.extent.width = uint(width)
    ici.extent.height = uint(height)
    ici.extent.depth = 1u
    ici.samples._1 = true
    ici.usage.transfer_dst = true                   // receives the linear->sRGB blit
    ici.usage.transfer_src = true                   // then is copied to the host readback buffer
    var inscope image <- create_image(device, ici)
    var req : VkMemoryRequirements
    vkGetImageMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(image), req)
    var want : VkMemoryPropertyFlags
    want.device_local = true
    let mai = MemoryAllocateInfo(
        allocationSize = req.size,
        memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope memory <- allocate_memory(device, mai)
    vk_check(vkBindImageMemory(boost_value_to_vk(device), boost_value_to_vk(image), boost_value_to_vk(memory), 0ul), null)
    var vci = ImageViewCreateInfo(
        image = weak_copy(image),
        viewType = VkImageViewType._2D,
        format = format)
    vci.subresourceRange.aspectMask.color = true
    vci.subresourceRange.levelCount = 1u
    vci.subresourceRange.layerCount = 1u
    var inscope view <- create_image_view(device, vci)
    return <- OffscreenTarget(image <- image, memory <- memory, view <- view)
}

// ===== pipelines =====

//! Fullscreen-triangle pipeline with additive blend (ONE/ONE on RGB) into an HDR_FMT target, for the
//! upsample passes: each smaller mip's contribution is *added* to the destination, rebuilding the energy
//! pyramid. The colour-write mask drops alpha, pinning dst alpha at the bright-pass value for honest debug views.
def private build_additive_pipeline(device : Device; layout : PipelineLayout;
                                    vert : ShaderModule; frag : ShaderModule) : Pipeline {
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
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.blendEnable = VK_TRUE
    blend_att.srcColorBlendFactor = VkBlendFactor.ONE
    blend_att.dstColorBlendFactor = VkBlendFactor.ONE
    blend_att.colorBlendOp = VkBlendOp.ADD
    // Vulkan reads the alphaBlend* fields even with alpha masked out; ONE/ONE/ADD is the simplest valid fill.
    blend_att.srcAlphaBlendFactor = VkBlendFactor.ONE
    blend_att.dstAlphaBlendFactor = VkBlendFactor.ONE
    blend_att.alphaBlendOp = VkBlendOp.ADD
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    // alpha-write disabled: see helper docstring -- dst alpha is preserved at the bright-pass value.
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var dyn_states <- [VkDynamicState.VIEWPORT, VkDynamicState.SCISSOR]
    var dynstate = VkPipelineDynamicStateCreateInfo()
    dynstate.dynamicStateCount = uint(length(dyn_states))

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    gp.layout = boost_value_to_vk(layout)
    var fmts = rendering_formats(HDR_FMT)
    attach_rendering_formats(gp, fmts)
    var h : VkPipeline
    unsafe {
        dynstate.pDynamicStates = addr(dyn_states[0])
        blend.pAttachments = addr(blend_att)
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pColorBlendState = addr(blend)
        gp.pDynamicState = addr(dynstate)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(gp), null, addr(h)), null)
    }
    delete stages
    delete dyn_states
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

// ===== build context =====

def public build_hdr_context() : HdrContext {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 11", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope res <- build_hdr_resources(device, phys, queue, pool)
    return <- HdrContext(
        instance <- instance, device <- device, queue = queue, pool <- pool, res <- res)
}

// ===== per-frame =====

//! Push the camera matrices + camera world position + time into the scene UBO, exactly like the
//! instancing tutorial. Bloom UBO has no per-frame data -- everything is in the push-constant.
def public update_hdr_uniforms(res : HdrResources; device : Device; time, camera_t : float) {
    let camera_angle = camera_t * 2.0f * PI
    let camera_r = 14.0f
    let cam_x = camera_r * cos(camera_angle)
    let cam_z = camera_r * sin(camera_angle)
    let cam_y = sin(camera_angle * 2.0f) * 2.0f
    let cam_pos = float3(cam_x, cam_y, cam_z)
    cam.view = look_at_rh(cam_pos, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f))
    cam.proj = perspective_vk(60.0f * PI / 180.0f, float(HDR_W) / float(HDR_H), 0.1f, 60.0f)
    cam.cam_time = float4(cam_pos.x, cam_pos.y, cam_pos.z, time)
    hdr_scene_vs_bind_uniform(device, res.ubo.memory)
}

//! Set the dynamic viewport + scissor to `(w x h)`. Called between each bloom-mip render pass.
def private set_dyn_vp(cmd : CommandBuffer; w, h : int) {
    var vp : VkViewport
    vp.width = float(w)
    vp.height = float(h)
    vp.maxDepth = 1.0f
    var sc : VkRect2D
    sc.extent.width = uint(w)
    sc.extent.height = uint(h)
    unsafe {
        vkCmdSetViewport(boost_value_to_vk(cmd), 0u, 1u, addr(vp))
        vkCmdSetScissor(boost_value_to_vk(cmd), 0u, 1u, addr(sc))
    }
}

//! Set the shader-side `pc : BloomCtrl` global to the per-pass values. The generated
//! `hdr_*_push_constants(cmd, layout)` helpers (one per fragment shader reading `pc`) then push the
//! struct with size + stage derived from its `@push_constant` declaration -- no size const, no memcpy.
def private set_pc(src_rcp_w, src_rcp_h, threshold, soft_knee, bloom_intensity : float) {
    pc.src_rcp = float2(src_rcp_w, src_rcp_h)
    pc.threshold = threshold
    pc.soft_knee = soft_knee
    pc.bloom_intensity = bloom_intensity
}

// ===== record =====

//! A target the previous frame sampled, about to be drawn into from scratch: UNDEFINED discards the
//! old contents, and the barrier orders the write after the last fragment-shader read of them.
def private begin_color_target(cmd : CommandBuffer; target : OffscreenTarget) {
    transition_image2(cmd, target.image, VkImageLayout.UNDEFINED, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
        VkPipelineStageFlags2.fragment_shader, ACCESS_2_NONE,
        VkPipelineStageFlags2.color_attachment_output, VkAccessFlags2.color_attachment_write)
}

//! A target that was just drawn into, about to be sampled by the next pass's fragment shader.
def private end_color_target_for_sampling(cmd : CommandBuffer; target : OffscreenTarget) {
    transition_image2(cmd, target.image, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL, VkImageLayout.SHADER_READ_ONLY_OPTIMAL,
        VkPipelineStageFlags2.color_attachment_output, VkAccessFlags2.color_attachment_write,
        VkPipelineStageFlags2.fragment_shader, VkAccessFlags2.shader_sampled_read)
}

//! Record the full HDR + bloom + composite frame into `cmd`. Eleven rendering passes with one barrier
//! per hand-off; the final composite leaves `ldr_color` in `TRANSFER_SRC_OPTIMAL` so the caller can
//! blit it straight to the readback buffer or the swap target.
def public record_hdr_frame(res : HdrResources; cmd : CommandBuffer) {   // nolint:STYLE038 - flat command-record run
    let threshold = 1.0f
    let soft_knee = 0.5f
    let bloom_intensity = 0.12f
    let depth_tests = VkPipelineStageFlags2.early_fragment_tests | VkPipelineStageFlags2.late_fragment_tests

    // ---- scene pass ----
    begin_color_target(cmd, res.hdr_color)
    // the previous frame's depth write lands at early OR late fragment tests; name both on each side
    transition_depth_image2(cmd, res.hdr_depth.image, VkImageLayout.UNDEFINED, VkImageLayout.DEPTH_ATTACHMENT_OPTIMAL,
        depth_tests, VkAccessFlags2.depth_stencil_attachment_write,
        depth_tests, VkAccessFlags2.depth_stencil_attachment_write)
    var scene_clears <- [clear_color(0.02f, 0.0f, 0.04f, 1.0f), clear_depth(1.0f)]
    record_rendering(cmd, res.hdr_color.view, res.hdr_depth.view, full_area(HDR_W, HDR_H), scene_clears) {
        cmd_bind_pipeline(cmd, res.scene_pipeline)
        let sets <- [vk_value_to_boost(res.scene_set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.scene_pipe_layout, 0u, sets, no_dyn)
        let vbufs <- [weak_copy(res.cube_vb.buffer), weak_copy(res.inst_buf.buffer)]
        var voffs <- [0ul, 0ul]
        cmd_bind_vertex_buffers(cmd, 0u, vbufs, voffs)
        cmd_bind_index_buffer(cmd, weak_copy(res.cube_ib.buffer), 0ul, VkIndexType.UINT16)
        cmd_draw_indexed(cmd, uint(N_INDICES), uint(N_INSTANCES), 0u, 0, 0u)
    }
    delete scene_clears
    end_color_target_for_sampling(cmd, res.hdr_color)

    // ---- bright pass : HDR scene -> bloom[0] ----
    let black = clear_color(0.0f, 0.0f, 0.0f, 1.0f)
    begin_color_target(cmd, res.bloom[0])
    record_rendering(cmd, res.bloom[0].view, full_area(BLOOM_W[0], BLOOM_H[0]), black) {
        set_dyn_vp(cmd, BLOOM_W[0], BLOOM_H[0])
        cmd_bind_pipeline(cmd, res.bright_pipeline)
        let sets <- [vk_value_to_boost(res.bright_set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.post_pipe_layout, 1u, sets, no_dyn)
        set_pc(1.0f / float(HDR_W), 1.0f / float(HDR_H), threshold, soft_knee, bloom_intensity)
        hdr_bright_fs_push_constants(cmd, res.post_pipe_layout)
        cmd_draw(cmd, 3u, 1u)
    }
    end_color_target_for_sampling(cmd, res.bloom[0])

    // ---- downsample chain : bloom[i] -> bloom[i+1] ----
    for (i in range(BLOOM_LEVELS - 1)) {
        begin_color_target(cmd, res.bloom[i + 1])
        record_rendering(cmd, res.bloom[i + 1].view, full_area(BLOOM_W[i + 1], BLOOM_H[i + 1]), black) {
            set_dyn_vp(cmd, BLOOM_W[i + 1], BLOOM_H[i + 1])
            cmd_bind_pipeline(cmd, res.down_pipeline)
            var inscope sets <- [vk_value_to_boost(res.down_set[i])]
            var inscope no_dyn : array<uint>
            cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.post_pipe_layout, 1u, sets, no_dyn)
            set_pc(1.0f / float(BLOOM_W[i]), 1.0f / float(BLOOM_H[i]),
                   threshold, soft_knee, bloom_intensity)
            hdr_down_fs_push_constants(cmd, res.post_pipe_layout)
            cmd_draw(cmd, 3u, 1u)
        }
        end_color_target_for_sampling(cmd, res.bloom[i + 1])
    }

    // ---- upsample chain (additive blend) : bloom[i+1] -> bloom[i] ----
    for (k in range(BLOOM_LEVELS - 1)) {
        let i = BLOOM_LEVELS - 2 - k       // iterate dst = 3, 2, 1, 0
        // bloom[i] was sampled by the downsample step; it comes back as a color attachment whose
        // contents the LOAD keeps - the blender reads them, so the destination access is read + write
        transition_image2(cmd, res.bloom[i].image, VkImageLayout.SHADER_READ_ONLY_OPTIMAL, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
            VkPipelineStageFlags2.fragment_shader, VkAccessFlags2.shader_sampled_read,
            VkPipelineStageFlags2.color_attachment_output,
            VkAccessFlags2.color_attachment_read | VkAccessFlags2.color_attachment_write)
        record_rendering(cmd, res.bloom[i].view, full_area(BLOOM_W[i], BLOOM_H[i]), black, VkAttachmentLoadOp.LOAD) {
            set_dyn_vp(cmd, BLOOM_W[i], BLOOM_H[i])
            cmd_bind_pipeline(cmd, res.up_pipeline)
            var inscope sets <- [vk_value_to_boost(res.up_set[i])]
            var inscope no_dyn : array<uint>
            cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.post_pipe_layout, 1u, sets, no_dyn)
            set_pc(1.0f / float(BLOOM_W[i + 1]), 1.0f / float(BLOOM_H[i + 1]),
                   threshold, soft_knee, bloom_intensity)
            hdr_up_fs_push_constants(cmd, res.post_pipe_layout)
            cmd_draw(cmd, 3u, 1u)
        }
        end_color_target_for_sampling(cmd, res.bloom[i])
    }

    // ---- composite pass : HDR + bloom[0] -> ACES -> LDR ----
    // the previous frame's blit read the LDR target; this frame overwrites it from scratch
    transition_image2(cmd, res.ldr_color.image, VkImageLayout.UNDEFINED, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL,
        VkPipelineStageFlags2.blit, ACCESS_2_NONE,
        VkPipelineStageFlags2.color_attachment_output, VkAccessFlags2.color_attachment_write)
    record_rendering(cmd, res.ldr_color.view, full_area(HDR_W, HDR_H), black) {
        cmd_bind_pipeline(cmd, res.composite_pipeline)
        let sets <- [vk_value_to_boost(res.composite_set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.composite_pipe_layout, 1u, sets, no_dyn)
        set_pc(1.0f / float(HDR_W), 1.0f / float(HDR_H), threshold, soft_knee, bloom_intensity)
        hdr_composite_fs_push_constants(cmd, res.composite_pipe_layout)
        cmd_draw(cmd, 3u, 1u)
    }
    // the composite's writes become the blit's reads (readback intermediate or swap target)
    transition_image2(cmd, res.ldr_color.image, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
        VkPipelineStageFlags2.color_attachment_output, VkAccessFlags2.color_attachment_write,
        VkPipelineStageFlags2.blit, VkAccessFlags2.transfer_read)
}

//! Per-frame: update UBO, record the HDR/bloom/composite chain, blit the linear 16-bit-float LDR
//! target through an sRGB intermediate (hardware encodes linear->sRGB at format conversion), then
//! copy those bytes to the host buffer. show_hdr.das blits the same way straight to the swap target.
def public render_hdr_frame(var ctx : HdrContext; time, camera_t : float) : array<uint8> {
    update_hdr_uniforms(ctx.res, ctx.device, time, camera_t)
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_hdr_frame(ctx.res, cmd)
        // ldr_color is in TRANSFER_SRC_OPTIMAL (the frame's closing barrier). Bring readback_8bit to
        // TRANSFER_DST_OPTIMAL so we can blit into it.
        transition_image2(cmd, ctx.res.readback_8bit.image, VkImageLayout.UNDEFINED, VkImageLayout.TRANSFER_DST_OPTIMAL,
            PIPELINE_STAGE_2_NONE, ACCESS_2_NONE, VkPipelineStageFlags2.blit, VkAccessFlags2.transfer_write)
        // Blit linear 16-bit float -> 8-bit sRGB. Hardware applies the encoding at the format
        // conversion; the dark-region precision boost from sRGB hides what would otherwise be
        // visible banding in a smooth 8-bit-UNORM target.
        var region : VkImageBlit
        region.srcSubresource.aspectMask.color = true
        region.srcSubresource.layerCount = 1u
        region.srcOffsets[1].x = HDR_W
        region.srcOffsets[1].y = HDR_H
        region.srcOffsets[1].z = 1
        region.dstSubresource.aspectMask.color = true
        region.dstSubresource.layerCount = 1u
        region.dstOffsets[1].x = HDR_W
        region.dstOffsets[1].y = HDR_H
        region.dstOffsets[1].z = 1
        vkCmdBlitImage(boost_value_to_vk(cmd),
            boost_value_to_vk(ctx.res.ldr_color.image),     VkImageLayout.TRANSFER_SRC_OPTIMAL,
            boost_value_to_vk(ctx.res.readback_8bit.image), VkImageLayout.TRANSFER_DST_OPTIMAL,
            1u, unsafe(addr(region)), VkFilter.NEAREST)
        // readback_8bit -> TRANSFER_SRC_OPTIMAL so copy_image_to_buffer can read it.
        transition_image2(cmd, ctx.res.readback_8bit.image, VkImageLayout.TRANSFER_DST_OPTIMAL, VkImageLayout.TRANSFER_SRC_OPTIMAL,
            VkPipelineStageFlags2.blit, VkAccessFlags2.transfer_write,
            VkPipelineStageFlags2.copy, VkAccessFlags2.transfer_read)
        copy_image_to_buffer(cmd, ctx.res.readback_8bit.image, ctx.res.readback, HDR_W, HDR_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

//! Self-contained one-shot. Used by the test + recording.
def public render_hdr_scene(time, camera_t : float) : array<uint8> {
    var inscope ctx <- build_hdr_context()
    return <- render_hdr_frame(ctx, time, camera_t)
}

//! Sample RGB of pixel (x,y) from an HDR_W*HDR_H RGBA8 buffer.
def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * HDR_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}
