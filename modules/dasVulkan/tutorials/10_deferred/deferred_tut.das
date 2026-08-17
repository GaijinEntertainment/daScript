options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 10 -- deferred shading "putting it all together". Reuses the shadow rail from tutorial
// 08 and builds the deferred renderer as FOUR single-subpass render passes that hand off via
// attachment finalLayouts -- the more common real-world architecture than one multi-subpass pass
// with input attachments (and the architecture REAL screen-space AO needs, since SSAO must sample
// NEIGHBOUR G-buffer texels, which input attachments cannot do):
//
//   * SHADOW pass: depth-only render from the sun's POV into a 1024x1024 D32_SFLOAT image
//     (DEPTH_STENCIL_READ_ONLY at end-of-pass), sampled in the lighting pass as `sampler2DShadow`.
//
//   * G-BUFFER pass: 3 MRT color + depth. albedo+spec (RGBA8), perturbed world normal+shininess
//     (RGBA16F), world-pos+material (RGBA16F). The 3 color attachments STORE and transition to
//     SHADER_READ_ONLY so the next two passes can SAMPLE them.
//
//   * SSAO pass: a fullscreen triangle that samples g_normal + g_worldpos as textures and computes
//     real hemisphere-kernel screen-space ambient occlusion (writes an R8 factor, SHADER_READ_ONLY).
//
//   * LIGHTING pass: a fullscreen triangle that samples the 3 G-buffer textures + the SSAO factor +
//     the shadow map + the HDR environment, composites the lit result into final_color (TRANSFER_SRC
//     for the blit/readback).
//
// FOUR pipelines, one per shader pair:
//   * shadow_pipeline   (depth-only, vertex-only,            rp_shadow)
//   * gbuffer_pipeline  (vertex+fragment, pos+normal+uv,     rp_gbuffer,  3 color blends)
//   * ssao_pipeline     (vertex+fragment, no vertex input,   rp_ssao,     1 color blend)
//   * lighting_pipeline (vertex+fragment, no vertex input,   rp_lighting, 1 color blend)
//
// SCENE: a concrete-cat statue + a brick floor (real CC0 PBR maps), lit by ONE shadowed directional
// sun + THREE orbiting colored point lights packed in the scene UBO, with HDR-equirect IBL. The real
// SSAO darkens crevices + the cat-floor contact, grounding the cat on the brick.

module deferred_tut

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/vulkan_assets        // tutorial_asset_dir
require geometry/geom_gen           // load_obj_mesh (text OBJ, no stbimage)
require daslib/fio                  // path_join
require deferred_tut_shaders public
require math

let public DEFERRED_W = 768
let public DEFERRED_H = 768
let public SHADOW_W = 1024
let public SHADOW_H = 1024

let DEPTH_FMT    = VkFormat.D32_SFLOAT
let ALBEDO_FMT   = VkFormat.R8G8B8A8_UNORM
let NORMAL_FMT   = VkFormat.R16G16B16A16_SFLOAT
let WORLDPOS_FMT = VkFormat.R16G16B16A16_SFLOAT
let SSAO_FMT     = VkFormat.R8_UNORM
let FINAL_FMT    = VkFormat.R8G8B8A8_SRGB   // sRGB: the final DISPLAY image, gamma-encoded on write (G-buffer formats above stay linear data)

let XFORM_SIZE = 192ul                                     // 3 * float4x4
let SCENE_SIZE = 192ul                                     // 4*float4 + 3*float4x4? no: 1 mat (64) + 2 vec (32) + 6 vec (96) = 192

// debug_mode push-constant labels (host-visible -- the shader treats the int symbolically).
let public DBG_LIT       = 0
let public DBG_ALBEDO    = 1
let public DBG_NORMAL    = 2
let public DBG_WORLDPOS  = 3
let public DBG_SSAO      = 4
let public DBG_LIGHT     = 5
let public DBG_MODE_COUNT = 6

// ===== cat statue geometry (loaded from an OBJ; stride 32 = pos + normal + uv, uint32 indices) =====

// The concrete-cat-statue model (CC0, tutorials/_assets/cat) replaces the old cube: organic geometry
// shows off deferred shading, shadows, AO and IBL far better than a box. load_obj_mesh is plain text
// (CI-safe geometry; only HDR + textures need stbimage), and OBJ UVs ride along (the floor pads one).

let CAT_HEIGHT = 2.2f       // fitted height in world units; base sits at y=0, centred in x/z

//! Load + fit the cat into `verts` (interleaved pos.xyz + normal.xyz + uv.xy, stride 32) and `indices`
//! (uint32). The model is centred in x/z, its base dropped to y=0, and scaled to CAT_HEIGHT tall.
def build_cat_geometry(var verts : array<float>; var indices : array<uint>) {
    let dir = tutorial_asset_dir("cat")
    var frag <- load_obj_mesh(path_join(dir, "concrete_cat_statue.obj"))
    var bmin = float3(1.0e30)
    var bmax = float3(-1.0e30)
    for (gv in frag.vertices) {
        bmin = min(bmin, gv.xyz)
        bmax = max(bmax, gv.xyz)
    }
    let center = (bmin + bmax) * 0.5
    let s = CAT_HEIGHT / max(bmax.y - bmin.y, 0.001)
    verts |> reserve(length(frag.vertices) * 8)
    for (gv in frag.vertices) {
        let p = float3((gv.xyz.x - center.x) * s, (gv.xyz.y - bmin.y) * s, (gv.xyz.z - center.z) * s)
        verts |> push(p.x); verts |> push(p.y); verts |> push(p.z)
        verts |> push(gv.normal.x); verts |> push(gv.normal.y); verts |> push(gv.normal.z)
        verts |> push(gv.uv.x); verts |> push(gv.uv.y)
    }
    indices |> reserve(length(frag.indices))
    for (gi in frag.indices) {
        indices |> push(uint(gi))
    }
    delete frag
}

// Floor quad: 4 verts pos + (normal = 0, 1, 0). Big enough to catch the sun's shadow at all
// camera angles. Triangle winding matches the +Y face above (CCW from above).
// stride 32 = pos + normal + uv; the floor is procedural (world-pos tri-planar), so its uv is a dummy.
let private floor_vertices : array<float> <- [
    -6.0, 0.0,  6.0,  0.0, 1.0, 0.0,  0.0, 0.0,
     6.0, 0.0,  6.0,  0.0, 1.0, 0.0,  1.0, 0.0,
     6.0, 0.0, -6.0,  0.0, 1.0, 0.0,  1.0, 1.0,
    -6.0, 0.0, -6.0,  0.0, 1.0, 0.0,  0.0, 1.0
]

let private floor_indices : array<uint16> <- [
    uint16(0), uint16(1), uint16(2),  uint16(0), uint16(2), uint16(3)
]

let public N_FLOOR_INDICES = 6

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

def ortho_vk(left, right, bottom, top, znear, zfar : float) : float4x4 {
    let rl = 1.0f / (right - left)
    let tb = 1.0f / (top - bottom)
    let fn = 1.0f / (znear - zfar)
    var m : float4x4
    m[0] = float4(2.0f * rl, 0.0f, 0.0f, 0.0f)
    m[1] = float4(0.0f, -2.0f * tb, 0.0f, 0.0f)
    m[2] = float4(0.0f, 0.0f, fn, 0.0f)
    m[3] = float4(-(right + left) * rl, (top + bottom) * tb, znear * fn, 1.0f)
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

def translate_m4(t : float3) : float4x4 {
    var m : float4x4
    m[0] = float4(1.0f, 0.0f, 0.0f, 0.0f)
    m[1] = float4(0.0f, 1.0f, 0.0f, 0.0f)
    m[2] = float4(0.0f, 0.0f, 1.0f, 0.0f)
    m[3] = float4(t.x, t.y, t.z, 1.0f)
    return m
}

def identity_m4 : float4x4 {
    var m : float4x4
    m[0] = float4(1.0f, 0.0f, 0.0f, 0.0f)
    m[1] = float4(0.0f, 1.0f, 0.0f, 0.0f)
    m[2] = float4(0.0f, 0.0f, 1.0f, 0.0f)
    m[3] = float4(0.0f, 0.0f, 0.0f, 1.0f)
    return m
}

def mat4_mul(a, b : float4x4) : float4x4 {
    var r : float4x4
    for (col in range(4)) {
        let bc = b[col]
        r[col] = a[0] * bc.x + a[1] * bc.y + a[2] * bc.z + a[3] * bc.w
    }
    return r
}

// ===== resources =====

struct public DeferredResources {
    g_albedo      : OffscreenTarget
    g_normal      : OffscreenTarget
    g_worldpos    : OffscreenTarget
    ssao_img      : OffscreenTarget
    final_color   : OffscreenTarget
    depth         : OffscreenDepth
    shadow_map    : OffscreenDepth
    shadow_sampler : Sampler
    env_tex       : Texture            // HDR equirect environment (IBL)
    env_sampler   : Sampler
    cat_albedo    : Texture            // cat PBR: diff / nor_gl / arm
    cat_normal    : Texture
    cat_arm       : Texture
    cat_sampler   : Sampler            // shared linear/repeat/mip sampler (cat + floor brick)
    floor_albedo  : Texture            // floor brick: Color / NormalGL / AO
    floor_normal  : Texture
    floor_ao      : Texture
    floor_roughness : Texture          // floor brick: Roughness (linear) -> drives floor spec
    gbuffer_sampler : Sampler          // NEAREST/clamp — exact G-buffer texel reads (ssao + lighting)

    rp_shadow     : RenderPass
    rp_gbuffer    : RenderPass
    rp_ssao       : RenderPass
    rp_lighting   : RenderPass
    fb_shadow     : Framebuffer
    fb_gbuffer    : Framebuffer
    fb_ssao       : Framebuffer
    fb_lighting   : Framebuffer

    cube_vb : HostBuffer               // the cat model (loaded from OBJ)
    cube_ib : HostBuffer               // uint32 indices
    cat_index_count : uint
    floor_vb : HostBuffer
    floor_ib : HostBuffer
    xform_ubo : HostBuffer
    scene_ubo : HostBuffer

    shadow_set_layouts : array<DescriptorSetLayout>
    shadow_pipe_layout : PipelineLayout
    shadow_pipeline    : Pipeline
    shadow_set         : VkDescriptorSet

    gbuffer_set_layouts : array<DescriptorSetLayout>
    gbuffer_pipe_layout : PipelineLayout
    gbuffer_pipeline    : Pipeline
    gbuffer_set         : VkDescriptorSet

    ssao_set_layouts : array<DescriptorSetLayout>
    ssao_pipe_layout : PipelineLayout
    ssao_pipeline    : Pipeline
    ssao_set         : VkDescriptorSet      // set 0: xform UBO + g_normal + g_worldpos (sampled)

    lighting_set_layouts : array<DescriptorSetLayout>
    lighting_pipe_layout : PipelineLayout
    lighting_pipeline    : Pipeline
    lighting_set0        : VkDescriptorSet  // 4 sampled G-buffer + ssao textures
    lighting_set1        : VkDescriptorSet  // shadow_map + scene UBO + env_map

    desc_pool : DescriptorPool

    readback : HostBuffer
    buf_size : uint64
}

def public finalize(var r : DeferredResources) {
    unsafe {
        delete r.readback
        delete r.desc_pool
        delete r.lighting_pipeline
        delete r.lighting_pipe_layout
        delete r.lighting_set_layouts
        delete r.ssao_pipeline
        delete r.ssao_pipe_layout
        delete r.ssao_set_layouts
        delete r.gbuffer_pipeline
        delete r.gbuffer_pipe_layout
        delete r.gbuffer_set_layouts
        delete r.shadow_pipeline
        delete r.shadow_pipe_layout
        delete r.shadow_set_layouts
        delete r.fb_lighting
        delete r.fb_ssao
        delete r.fb_gbuffer
        delete r.fb_shadow
        delete r.rp_lighting
        delete r.rp_ssao
        delete r.rp_gbuffer
        delete r.rp_shadow
        delete r.gbuffer_sampler
        delete r.floor_roughness
        delete r.floor_ao
        delete r.floor_normal
        delete r.floor_albedo
        delete r.cat_sampler
        delete r.cat_arm
        delete r.cat_normal
        delete r.cat_albedo
        delete r.env_sampler
        delete r.env_tex
        delete r.shadow_sampler
        delete r.scene_ubo
        delete r.xform_ubo
        delete r.floor_ib
        delete r.floor_vb
        delete r.cube_ib
        delete r.cube_vb
        delete r.shadow_map
        delete r.depth
        delete r.final_color
        delete r.ssao_img
        delete r.g_worldpos
        delete r.g_normal
        delete r.g_albedo
    }
}

struct public DeferredContext {
    instance : Instance
    device   : Device
    queue    : VkQueue
    pool     : CommandPool
    res      : DeferredResources
}

def public finalize(var ctx : DeferredContext) {
    unsafe {
        delete ctx.res
        delete ctx.pool
        delete ctx.device
        delete ctx.instance
    }
}

// ===== build =====

// The largest builder in the tutorials (8 images + 8 textures/samplers + 4 render passes + 4 framebuffers
// + 4 pipelines + descriptors). One stack frame for all of it overflows the ~16KB per-context stack CI
// gives each test (worse on Linux), so phase helpers fill `r` by reference -- no `options stack` bump.

def public build_deferred_resources(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool) : DeferredResources {
    var r : DeferredResources
    build_df_images(r, device, phys)
    build_df_textures(r, device, phys, queue, pool)
    build_df_passes_framebuffers(r, device)
    build_df_geometry(r, device, phys)
    build_df_pipelines(r, device)
    build_df_descriptors(r, device)
    r.buf_size = uint64(DEFERRED_W * DEFERRED_H * 4)
    r.readback <- create_host_buffer(device, phys, r.buf_size)
    return <- r
}

// ----- offscreen images -----
def private build_df_images(var r : DeferredResources; device : Device; phys : VkPhysicalDevice) {
    var samples1 : VkSampleCountFlags
    samples1._1 = true
    var gbuf_extra : VkImageUsageFlags
    gbuf_extra.sampled = true               // sampled (not input_attachment) — ssao + lighting read NEIGHBOURS
    r.g_albedo   <- build_offscreen_target(device, phys, DEFERRED_W, DEFERRED_H, ALBEDO_FMT,   samples1, gbuf_extra)
    r.g_normal   <- build_offscreen_target(device, phys, DEFERRED_W, DEFERRED_H, NORMAL_FMT,   samples1, gbuf_extra)
    r.g_worldpos <- build_offscreen_target(device, phys, DEFERRED_W, DEFERRED_H, WORLDPOS_FMT, samples1, gbuf_extra)
    r.ssao_img   <- build_offscreen_target(device, phys, DEFERRED_W, DEFERRED_H, SSAO_FMT,     samples1, gbuf_extra)
    let depth_extra : VkImageUsageFlags
    r.depth <- build_offscreen_depth(device, phys, DEFERRED_W, DEFERRED_H, DEPTH_FMT, depth_extra)
    var sm_extra : VkImageUsageFlags
    sm_extra.sampled = true
    r.shadow_map <- build_offscreen_depth(device, phys, SHADOW_W, SHADOW_H, DEPTH_FMT, sm_extra)
    var final_extra : VkImageUsageFlags
    final_extra.transfer_src = true
    r.final_color <- build_offscreen_target(device, phys, DEFERRED_W, DEFERRED_H, FINAL_FMT, samples1, final_extra)
}

// ----- textures + samplers (HDR env, cat PBR maps, floor brick maps, the NEAREST G-buffer sampler) -----
def private build_df_textures(var r : DeferredResources; device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool) {
    r.shadow_sampler <- create_sampler_shadow(device)

    // HDR equirect environment (Cannon, Poly Haven CC0) + a linear sampler (wrap azimuth, clamp poles)
    r.env_tex <- load_texture_hdr(device, phys, queue, pool,
        path_join(tutorial_asset_dir("hdri"), "cannon_2k.hdr"))
    let esci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        addressModeU = VkSamplerAddressMode.REPEAT,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE)
    r.env_sampler <- create_sampler(device, esci)

    // Cat statue PBR maps (concrete_cat_statue, CC0): diff is sRGB albedo; nor_gl + arm are linear.
    let cat_tex_dir = path_join(tutorial_asset_dir("cat"), "textures")
    r.cat_albedo <- load_texture_2d(device, phys, queue, pool,
        path_join(cat_tex_dir, "concrete_cat_statue_diff_2k.jpg"), true)
    r.cat_normal <- load_texture_2d(device, phys, queue, pool,
        path_join(cat_tex_dir, "concrete_cat_statue_nor_gl_2k.jpg"), false)
    r.cat_arm <- load_texture_2d(device, phys, queue, pool,
        path_join(cat_tex_dir, "concrete_cat_statue_arm_2k.jpg"), false)
    // Anisotropy keeps the floor brick crisp at grazing angles, where the tiled map minifies hard; feature-gated because lavapipe (CI) lacks it.
    var dev_feats : VkPhysicalDeviceFeatures
    vkGetPhysicalDeviceFeatures(phys, dev_feats)
    let csci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.LINEAR,
        addressModeU = VkSamplerAddressMode.REPEAT,
        addressModeV = VkSamplerAddressMode.REPEAT,
        addressModeW = VkSamplerAddressMode.REPEAT,
        maxLod = 16.0f,
        anisotropyEnable = dev_feats.samplerAnisotropy,
        maxAnisotropy = dev_feats.samplerAnisotropy != 0u ? 8.0f : 1.0f)
    r.cat_sampler <- create_sampler(device, csci)

    // Floor brick maps (ambientCG Bricks031, CC0): Color sRGB, NormalGL + AO linear. Reuse cat_sampler.
    let brick_dir = tutorial_asset_dir("brick")
    r.floor_albedo <- load_texture_2d(device, phys, queue, pool,
        path_join(brick_dir, "Bricks031_1K-JPG_Color.jpg"), true)
    r.floor_normal <- load_texture_2d(device, phys, queue, pool,
        path_join(brick_dir, "Bricks031_1K-JPG_NormalGL.jpg"), false)
    r.floor_ao <- load_texture_2d(device, phys, queue, pool,
        path_join(brick_dir, "Bricks031_1K-JPG_AmbientOcclusion.jpg"), false)
    r.floor_roughness <- load_texture_2d(device, phys, queue, pool,
        path_join(brick_dir, "Bricks031_1K-JPG_Roughness.jpg"), false)

    // NEAREST/clamp for the G-buffer reads: ssao + lighting need exact texels, since linear would smear world-pos / normals across silhouettes. No mips.
    let gsci = SamplerCreateInfo(
        magFilter = VkFilter.NEAREST,
        minFilter = VkFilter.NEAREST,
        addressModeU = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeV = VkSamplerAddressMode.CLAMP_TO_EDGE,
        addressModeW = VkSamplerAddressMode.CLAMP_TO_EDGE)
    r.gbuffer_sampler <- create_sampler(device, gsci)
}

// ----- render passes (three single-subpass passes) + framebuffers -----
def private build_df_passes_framebuffers(var r : DeferredResources; device : Device) {
    r.rp_shadow   <- create_render_pass_depth_only(device, DEPTH_FMT)
    r.rp_gbuffer  <- build_gbuffer_render_pass(device)
    r.rp_ssao     <- create_render_pass_single_color(device, SSAO_FMT, VkImageLayout.SHADER_READ_ONLY_OPTIMAL)
    r.rp_lighting <- create_render_pass_single_color(device, FINAL_FMT)   // default finalLayout = TRANSFER_SRC_OPTIMAL

    var sm_attach <- [weak_copy(r.shadow_map.view)]
    var sm_fbci = FramebufferCreateInfo(
        renderPass = weak_copy(r.rp_shadow),
        pAttachments <- sm_attach,
        width = uint(SHADOW_W), height = uint(SHADOW_H), layers = 1u)
    r.fb_shadow <- create_framebuffer(device, sm_fbci)

    // G-buffer framebuffer: 3 MRT color + depth.
    var gb_attach <- [
        weak_copy(r.g_albedo.view),
        weak_copy(r.g_normal.view),
        weak_copy(r.g_worldpos.view),
        weak_copy(r.depth.view)]
    var gb_fbci = FramebufferCreateInfo(
        renderPass = weak_copy(r.rp_gbuffer),
        pAttachments <- gb_attach,
        width = uint(DEFERRED_W), height = uint(DEFERRED_H), layers = 1u)
    r.fb_gbuffer <- create_framebuffer(device, gb_fbci)

    // SSAO framebuffer: the single R8 occlusion target.
    var ssao_attach <- [weak_copy(r.ssao_img.view)]
    var ssao_fbci = FramebufferCreateInfo(
        renderPass = weak_copy(r.rp_ssao),
        pAttachments <- ssao_attach,
        width = uint(DEFERRED_W), height = uint(DEFERRED_H), layers = 1u)
    r.fb_ssao <- create_framebuffer(device, ssao_fbci)

    // Lighting framebuffer: the final composited color (blitted/copied out afterwards).
    var lit_attach <- [weak_copy(r.final_color.view)]
    var lit_fbci = FramebufferCreateInfo(
        renderPass = weak_copy(r.rp_lighting),
        pAttachments <- lit_attach,
        width = uint(DEFERRED_W), height = uint(DEFERRED_H), layers = 1u)
    r.fb_lighting <- create_framebuffer(device, lit_fbci)
}

// ----- geometry + UBO buffers -----
def private build_df_geometry(var r : DeferredResources; device : Device; phys : VkPhysicalDevice) {
    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var ib_usage : VkBufferUsageFlags
    ib_usage.index_buffer = true
    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true

    // cube_vb/cube_ib hold the CAT model (loaded from the OBJ; uint32 indices)
    var cat_v : array<float>
    var cat_i : array<uint>
    build_cat_geometry(cat_v, cat_i)
    r.cat_index_count = uint(length(cat_i))
    r.cube_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, cat_v)
    delete cat_v
    r.cube_ib <- create_host_buffer_from_bytes(device, phys, ib_usage, cat_i)
    delete cat_i

    var floor_v_copy := floor_vertices
    r.floor_vb <- create_host_buffer_from_bytes(device, phys, vb_usage, floor_v_copy)
    delete floor_v_copy
    var floor_i_copy := floor_indices
    r.floor_ib <- create_host_buffer_from_bytes(device, phys, ib_usage, floor_i_copy)
    delete floor_i_copy

    r.xform_ubo <- create_host_buffer(device, phys, XFORM_SIZE, ubo_usage)
    r.scene_ubo <- create_host_buffer(device, phys, SCENE_SIZE, ubo_usage)
}

// ----- pipeline layouts (via reflection) + shaders + pipelines -----
def private build_df_pipelines(var r : DeferredResources; device : Device) {
    var shadow_refl <- [decode_reflection(shadow_vert_spv_reflect)]
    r.shadow_set_layouts <- build_descriptor_set_layouts(device, shadow_refl)
    r.shadow_pipe_layout <- build_pipeline_layout(device, r.shadow_set_layouts, shadow_refl)
    delete shadow_refl

    var gbuf_refl <- [decode_reflection(gbuffer_vert_spv_reflect), decode_reflection(gbuffer_frag_spv_reflect)]
    r.gbuffer_set_layouts <- build_descriptor_set_layouts(device, gbuf_refl)
    r.gbuffer_pipe_layout <- build_pipeline_layout(device, r.gbuffer_set_layouts, gbuf_refl)
    delete gbuf_refl

    var ssao_refl <- [decode_reflection(ssao_vert_spv_reflect), decode_reflection(ssao_frag_spv_reflect)]
    r.ssao_set_layouts <- build_descriptor_set_layouts(device, ssao_refl)
    r.ssao_pipe_layout <- build_pipeline_layout(device, r.ssao_set_layouts, ssao_refl)
    delete ssao_refl

    var light_refl <- [decode_reflection(lighting_vert_spv_reflect), decode_reflection(lighting_frag_spv_reflect)]
    r.lighting_set_layouts <- build_descriptor_set_layouts(device, light_refl)
    r.lighting_pipe_layout <- build_pipeline_layout(device, r.lighting_set_layouts, light_refl)
    delete light_refl

    var inscope sh_shadow   <- create_shader_module(device, shadow_vert_spv)
    var inscope sh_gbuf_v   <- create_shader_module(device, gbuffer_vert_spv)
    var inscope sh_gbuf_f   <- create_shader_module(device, gbuffer_frag_spv)
    var inscope sh_ssao_v   <- create_shader_module(device, ssao_vert_spv)
    var inscope sh_ssao_f   <- create_shader_module(device, ssao_frag_spv)
    var inscope sh_light_v  <- create_shader_module(device, lighting_vert_spv)
    var inscope sh_light_f  <- create_shader_module(device, lighting_frag_spv)

    r.shadow_pipeline   <- build_shadow_pipeline_d(device, r.rp_shadow, r.shadow_pipe_layout, sh_shadow)
    r.gbuffer_pipeline  <- build_gbuffer_pipeline(device, r.rp_gbuffer, r.gbuffer_pipe_layout, sh_gbuf_v, sh_gbuf_f)
    r.ssao_pipeline     <- build_fullscreen_pipeline(device, r.rp_ssao, r.ssao_pipe_layout, sh_ssao_v, sh_ssao_f, 0u, false)
    r.lighting_pipeline <- build_fullscreen_pipeline(device, r.rp_lighting, r.lighting_pipe_layout, sh_light_v, sh_light_f, 0u, false)
}

// ----- descriptor pool + sets + writes -----
def private build_df_descriptors(var r : DeferredResources; device : Device) {
    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 5u                                              // shadow + gbuffer + ssao + lighting_set0 + lighting_set1
    // xform x3 (shadow/gbuffer/ssao) + scene (lighting_set1)
    let p_ubo = DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = 4u)
    dpci.pPoolSizes |> push(p_ubo)
    // gbuffer 7 + ssao 2 + lighting 4 + shadow + env
    let p_smp = DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = 17u)
    dpci.pPoolSizes |> push(p_smp)
    r.desc_pool <- create_descriptor_pool(device, dpci)

    r.shadow_set    = alloc_one_set(device, r.desc_pool, r.shadow_set_layouts[0])
    r.gbuffer_set   = alloc_one_set(device, r.desc_pool, r.gbuffer_set_layouts[0])
    r.ssao_set      = alloc_one_set(device, r.desc_pool, r.ssao_set_layouts[0])      // xform UBO + 2 sampled G-buffer
    r.lighting_set0 = alloc_one_set(device, r.desc_pool, r.lighting_set_layouts[0])  // 4 sampled G-buffer + ssao
    r.lighting_set1 = alloc_one_set(device, r.desc_pool, r.lighting_set_layouts[1])  // shadow_map + scene UBO + env

    var writes <- [
        write_ubo(r.shadow_set, 0u, r.xform_ubo.buffer, XFORM_SIZE),
        write_ubo(r.gbuffer_set, 0u, r.xform_ubo.buffer, XFORM_SIZE),
        write_image_sampler(r.gbuffer_set, 1u, r.cat_albedo.view, r.cat_sampler),
        write_image_sampler(r.gbuffer_set, 2u, r.cat_normal.view, r.cat_sampler),
        write_image_sampler(r.gbuffer_set, 3u, r.cat_arm.view, r.cat_sampler),
        write_image_sampler(r.gbuffer_set, 4u, r.floor_albedo.view, r.cat_sampler),
        write_image_sampler(r.gbuffer_set, 5u, r.floor_normal.view, r.cat_sampler),
        write_image_sampler(r.gbuffer_set, 6u, r.floor_ao.view, r.cat_sampler),
        write_image_sampler(r.gbuffer_set, 7u, r.floor_roughness.view, r.cat_sampler),
        // ssao set 0: xform UBO (b0) + sampled g_normal (b1) + g_worldpos (b2), NEAREST
        write_ubo(r.ssao_set, 0u, r.xform_ubo.buffer, XFORM_SIZE),
        write_image_sampler(r.ssao_set, 1u, r.g_normal.view, r.gbuffer_sampler),
        write_image_sampler(r.ssao_set, 2u, r.g_worldpos.view, r.gbuffer_sampler),
        // lighting set 0: sampled albedo (b0) / normal (b1) / world-pos (b2) / ssao (b3), NEAREST
        write_image_sampler(r.lighting_set0, 0u, r.g_albedo.view, r.gbuffer_sampler),
        write_image_sampler(r.lighting_set0, 1u, r.g_normal.view, r.gbuffer_sampler),
        write_image_sampler(r.lighting_set0, 2u, r.g_worldpos.view, r.gbuffer_sampler),
        write_image_sampler(r.lighting_set0, 3u, r.ssao_img.view, r.gbuffer_sampler),
        write_shadow_sampler(r.lighting_set1, 0u, r.shadow_map.view, r.shadow_sampler),
        write_ubo(r.lighting_set1, 1u, r.scene_ubo.buffer, SCENE_SIZE),
        write_image_sampler(r.lighting_set1, 2u, r.env_tex.view, r.env_sampler)]

    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)
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

def private write_shadow_sampler(set : VkDescriptorSet; binding : uint; view : ImageView; sampler : Sampler) : WriteDescriptorSet {
    var w = WriteDescriptorSet(
        dstSet = vk_value_to_boost(set),
        dstBinding = binding,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER,
        descriptorCount = 1u)
    let ii = DescriptorImageInfo(
        sampler = weak_copy(sampler),
        imageView = weak_copy(view),
        imageLayout = VkImageLayout.DEPTH_STENCIL_READ_ONLY_OPTIMAL)
    w.pImageInfo |> push(ii)
    return <- w
}

//! A regular sampled image (SHADER_READ_ONLY layout) — for the HDR environment map.
def private write_image_sampler(set : VkDescriptorSet; binding : uint; view : ImageView; sampler : Sampler) : WriteDescriptorSet {
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

// ===== G-buffer render pass (3 MRT color + depth, single subpass) =====

// The 3 color attachments STORE + transition to SHADER_READ_ONLY so the ssao + lighting passes can
// SAMPLE them. No input attachments, no explicit subpass deps -- the cross-pass handoff rides the
// attachment finalLayouts, the same pattern the shadow pass uses.

def private gbuf_color_attach(fmt : VkFormat) : VkAttachmentDescription {
    var a : VkAttachmentDescription
    a.format = fmt
    a.samples._1 = true
    a.loadOp = VkAttachmentLoadOp.CLEAR
    a.storeOp = VkAttachmentStoreOp.STORE          // consumed by ssao + lighting -> MUST store
    a.stencilLoadOp = VkAttachmentLoadOp.DONT_CARE
    a.stencilStoreOp = VkAttachmentStoreOp.DONT_CARE
    a.initialLayout = VkImageLayout.UNDEFINED
    a.finalLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL
    return a
}

def private build_gbuffer_render_pass(device : Device) : RenderPass {
    var a_depth : VkAttachmentDescription
    a_depth.format = DEPTH_FMT
    a_depth.samples._1 = true
    a_depth.loadOp = VkAttachmentLoadOp.CLEAR
    a_depth.storeOp = VkAttachmentStoreOp.DONT_CARE
    a_depth.stencilLoadOp = VkAttachmentLoadOp.DONT_CARE
    a_depth.stencilStoreOp = VkAttachmentStoreOp.DONT_CARE
    a_depth.initialLayout = VkImageLayout.UNDEFINED
    a_depth.finalLayout = VkImageLayout.DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    var attaches <- [
        gbuf_color_attach(ALBEDO_FMT),
        gbuf_color_attach(NORMAL_FMT),
        gbuf_color_attach(WORLDPOS_FMT),
        a_depth]

    // Single subpass: write the 3 G-buffer color attachments + depth.
    var s0_color <- [
        att_ref(0u, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL),
        att_ref(1u, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL),
        att_ref(2u, VkImageLayout.COLOR_ATTACHMENT_OPTIMAL)]
    var s0_depth = att_ref(3u, VkImageLayout.DEPTH_STENCIL_ATTACHMENT_OPTIMAL)

    var s0 : VkSubpassDescription
    s0.pipelineBindPoint = VkPipelineBindPoint.GRAPHICS
    s0.colorAttachmentCount = 3u

    var subpasses : array<VkSubpassDescription>
    subpasses |> push(s0)

    var ci = VkRenderPassCreateInfo()
    ci.attachmentCount = uint(length(attaches))
    ci.subpassCount = 1u
    var h : VkRenderPass
    unsafe {
        subpasses[0].pColorAttachments = addr(s0_color[0])
        subpasses[0].pDepthStencilAttachment = addr(s0_depth)
        ci.pAttachments = addr(attaches[0])
        ci.pSubpasses = addr(subpasses[0])
        vk_check(vkCreateRenderPass(boost_value_to_vk(device), ci, null, h), null)
    }
    delete attaches
    delete subpasses
    delete s0_color
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

def private att_ref(att : uint; layout : VkImageLayout) : VkAttachmentReference {
    var r : VkAttachmentReference
    r.attachment = att
    r.layout = layout
    return r
}

// ===== pipeline builders =====

def private build_shadow_pipeline_d(device : Device; rp : RenderPass; layout : PipelineLayout; vert : ShaderModule) : Pipeline {   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
    var stages : array<VkPipelineShaderStageCreateInfo>
    stages |> resize(1)
    stages[0] = VkPipelineShaderStageCreateInfo()
    stages[0].stage.vertex = true
    stages[0].module_ = boost_value_to_vk(vert)
    stages[0].pName = "main"

    var vbindings : array<VkVertexInputBindingDescription>
    var vb_desc : VkVertexInputBindingDescription
    vb_desc.binding = 0u
    vb_desc.stride = 32u                                            // pos + normal + uv (shadow uses only pos@0)
    vb_desc.inputRate = VkVertexInputRate.VERTEX
    vbindings |> push(vb_desc)
    var vattrs : array<VkVertexInputAttributeDescription>
    var ap : VkVertexInputAttributeDescription
    ap.location = 0u
    ap.binding = 0u
    ap.format = VkFormat.R32G32B32_SFLOAT
    ap.offset = 0u
    vattrs |> push(ap)
    var an : VkVertexInputAttributeDescription
    an.location = 1u
    an.binding = 0u
    an.format = VkFormat.R32G32B32_SFLOAT
    an.offset = 12u
    vattrs |> push(an)

    var vinput = VkPipelineVertexInputStateCreateInfo()
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var viewport : VkViewport
    viewport.width = float(SHADOW_W)
    viewport.height = float(SHADOW_H)
    viewport.maxDepth = 1.0f
    var scissor : VkRect2D
    scissor.extent.width = uint(SHADOW_W)
    scissor.extent.height = uint(SHADOW_H)
    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.cullMode.back = true
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    raster.depthBiasEnable = 1u
    raster.depthBiasConstantFactor = 4.0f
    raster.depthBiasSlopeFactor = 4.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var depth = VkPipelineDepthStencilStateCreateInfo()
    depth.depthTestEnable = 1u
    depth.depthWriteEnable = 1u
    depth.depthCompareOp = VkCompareOp.LESS

    // No color attachments, but Vulkan still needs a valid blend struct (count=0).
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 0u

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = uint(length(stages))
    var h : VkPipeline
    unsafe {
        vinput.vertexBindingDescriptionCount = 1u
        vinput.pVertexBindingDescriptions = addr(vbindings[0])
        vinput.vertexAttributeDescriptionCount = uint(length(vattrs))
        vinput.pVertexAttributeDescriptions = addr(vattrs[0])
        vpstate.pViewports = addr(viewport)
        vpstate.pScissors = addr(scissor)
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depth)
        gp.pColorBlendState = addr(blend)
        gp.layout = boost_value_to_vk(layout)
        gp.renderPass = boost_value_to_vk(rp)
        gp.subpass = 0u
        var pci : array<VkGraphicsPipelineCreateInfo>
        pci |> push(gp)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(pci[0]), null, addr(h)), null)
        delete pci
    }
    delete stages
    delete vbindings
    delete vattrs
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

def private build_gbuffer_pipeline(device : Device; rp : RenderPass; layout : PipelineLayout;   // nolint:STYLE038 - flat one-call-per-item Vulkan setup run
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

    var vbindings : array<VkVertexInputBindingDescription>
    var vb_desc : VkVertexInputBindingDescription
    vb_desc.binding = 0u
    vb_desc.stride = 32u                                            // pos(12) + normal(12) + uv(8)
    vb_desc.inputRate = VkVertexInputRate.VERTEX
    vbindings |> push(vb_desc)
    var vattrs : array<VkVertexInputAttributeDescription>
    var ap : VkVertexInputAttributeDescription
    ap.location = 0u
    ap.binding = 0u
    ap.format = VkFormat.R32G32B32_SFLOAT
    ap.offset = 0u
    vattrs |> push(ap)
    var an : VkVertexInputAttributeDescription
    an.location = 1u
    an.binding = 0u
    an.format = VkFormat.R32G32B32_SFLOAT
    an.offset = 12u
    vattrs |> push(an)
    var au : VkVertexInputAttributeDescription
    au.location = 2u
    au.binding = 0u
    au.format = VkFormat.R32G32_SFLOAT
    au.offset = 24u
    vattrs |> push(au)

    var vinput = VkPipelineVertexInputStateCreateInfo()
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var viewport : VkViewport
    viewport.width = float(DEFERRED_W)
    viewport.height = float(DEFERRED_H)
    viewport.maxDepth = 1.0f
    var scissor : VkRect2D
    scissor.extent.width = uint(DEFERRED_W)
    scissor.extent.height = uint(DEFERRED_H)
    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.cullMode.back = true
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var depth = VkPipelineDepthStencilStateCreateInfo()
    depth.depthTestEnable = 1u
    depth.depthWriteEnable = 1u
    depth.depthCompareOp = VkCompareOp.LESS

    var att : VkPipelineColorBlendAttachmentState
    att.colorWriteMask.r = true
    att.colorWriteMask.g = true
    att.colorWriteMask.b = true
    att.colorWriteMask.a = true
    var blend_atts <- [for (_ in range(3)); att]      // 3 identical MRT blend states (write RGBA, no blend)
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 3u

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    var h : VkPipeline
    unsafe {
        vinput.vertexBindingDescriptionCount = 1u
        vinput.pVertexBindingDescriptions = addr(vbindings[0])
        vinput.vertexAttributeDescriptionCount = uint(length(vattrs))
        vinput.pVertexAttributeDescriptions = addr(vattrs[0])
        vpstate.pViewports = addr(viewport)
        vpstate.pScissors = addr(scissor)
        blend.pAttachments = addr(blend_atts[0])
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depth)
        gp.pColorBlendState = addr(blend)
        gp.layout = boost_value_to_vk(layout)
        gp.renderPass = boost_value_to_vk(rp)
        gp.subpass = 0u
        var pci : array<VkGraphicsPipelineCreateInfo>
        pci |> push(gp)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(pci[0]), null, addr(h)), null)
        delete pci
    }
    delete stages
    delete vbindings
    delete vattrs
    delete blend_atts
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

//! Build a fullscreen-triangle pipeline (no vertex input, no depth, one color attachment).
//! `_depth` is reserved for future depth-test-on use.
def private build_fullscreen_pipeline(device : Device; rp : RenderPass; layout : PipelineLayout;
                                      vert : ShaderModule; frag : ShaderModule; subpass : uint;
                                      _depth : bool) : Pipeline {
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

    var viewport : VkViewport
    viewport.width = float(DEFERRED_W)
    viewport.height = float(DEFERRED_H)
    viewport.maxDepth = 1.0f
    var scissor : VkRect2D
    scissor.extent.width = uint(DEFERRED_W)
    scissor.extent.height = uint(DEFERRED_H)
    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    var depth_state = VkPipelineDepthStencilStateCreateInfo()

    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    var h : VkPipeline
    unsafe {
        vpstate.pViewports = addr(viewport)
        vpstate.pScissors = addr(scissor)
        blend.pAttachments = addr(blend_att)
        gp.pStages = addr(stages[0])
        gp.pVertexInputState = addr(vinput)
        gp.pInputAssemblyState = addr(iasm)
        gp.pViewportState = addr(vpstate)
        gp.pRasterizationState = addr(raster)
        gp.pMultisampleState = addr(msaa)
        gp.pDepthStencilState = addr(depth_state)
        gp.pColorBlendState = addr(blend)
        gp.layout = boost_value_to_vk(layout)
        gp.renderPass = boost_value_to_vk(rp)
        gp.subpass = subpass
        var pci : array<VkGraphicsPipelineCreateInfo>
        pci |> push(gp)
        vk_check(vkCreateGraphicsPipelines(boost_value_to_vk(device), null, 1u, addr(pci[0]), null, addr(h)), null)
        delete pci
    }
    delete stages
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

// ===== per-frame UBO updates =====

//! Set the shader-side xform + scene globals from time + camera_t, then marshal them into the two
//! UBOs. Lights orbit on a tilted circle around the scene origin; the sun direction is fixed.
def public update_deferred_uniforms(res : DeferredResources; device : Device; time, camera_t : float) {
    let camera_angle = camera_t * 2.0f * PI
    let camera_r = 5.0f
    let cam_x = camera_r * cos(camera_angle)
    let cam_z = camera_r * sin(camera_angle)
    let cam_y = 2.0f + sin(camera_angle * 2.0f) * 0.6f
    let cam_pos = float3(cam_x, cam_y, cam_z)

    let sun_dir = vec3_normalize(float3(-0.4f, 1.0f, 0.3f))
    let sun_eye = float3(sun_dir.x * 8.0f, sun_dir.y * 8.0f, sun_dir.z * 8.0f)
    let sun_view = look_at_rh(sun_eye, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f))
    let sun_proj = ortho_vk(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 20.0f)
    let light_vp = mat4_mul(sun_proj, sun_view)

    xform.view = look_at_rh(cam_pos, float3(0.0f, 1.0f, 0.0f), float3(0.0f, 1.0f, 0.0f))
    xform.proj = perspective_vk(60.0f * PI / 180.0f, float(DEFERRED_W) / float(DEFERRED_H), 0.1f, 50.0f)
    xform.light_vp = light_vp
    gbuffer_vs_bind_uniform(device, res.xform_ubo.memory)

    scene.light_vp = light_vp
    scene.light_dir = float4(sun_dir.x, sun_dir.y, sun_dir.z, 0.0f)
    scene.camera_pos = float4(cam_pos.x, cam_pos.y, cam_pos.z, time)
    // Three colored point lights orbit at radius 2.5, height 1.2 (above the floor, below the cat's centre),
    // range 6 / falloff 1.5 -- a wide soft pool. Kept as ACCENTS (~1.6 peak) so sun + ambient stay the base:
    // a 4x bump blew the concrete/brick out to flat colour and killed the dark contact that grounds the cat.
    let lt = time * 0.5f
    let lr = 2.5f
    let lh = 1.2f
    scene.pos0 = float4(lr * cos(lt),          lh, lr * sin(lt),          6.0f)
    scene.col0 = float4(1.6f, 0.34f, 0.26f, 1.5f)
    scene.pos1 = float4(lr * cos(lt + 2.094f), lh, lr * sin(lt + 2.094f), 6.0f)
    scene.col1 = float4(0.26f, 1.5f, 0.42f, 1.5f)
    scene.pos2 = float4(lr * cos(lt + 4.188f), lh, lr * sin(lt + 4.188f), 6.0f)
    scene.col2 = float4(0.34f, 0.5f, 1.6f, 1.5f)
    lighting_fs_bind_uniform(device, res.scene_ubo.memory)
}

// ===== per-frame recording =====

//! Record shadow pass (depth-only, casters) + deferred multi-subpass pass (G-buffer + SSAO +
//! lighting). The caller must have set `dbg.debug_mode` before invoking; lighting_fs_push_constants
//! pushes that.
def public record_deferred_render(res : DeferredResources; cmd : CommandBuffer) {
    // ---- shadow pass (depth-only) ----
    var sh_clears <- [clear_depth(1.0f)]
    record_render_pass(cmd, res.rp_shadow, res.fb_shadow, full_area(SHADOW_W, SHADOW_H), sh_clears) {
        cmd_bind_pipeline(cmd, res.shadow_pipeline)
        let s_sets <- [vk_value_to_boost(res.shadow_set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.shadow_pipe_layout, 0u, s_sets, no_dyn)
        draw_casters(res, cmd, res.shadow_pipe_layout)
    }
    delete sh_clears

    // ---- G-buffer pass (3 MRT color + depth) ----
    var gb_clears <- [
        clear_color(0.0f, 0.0f, 0.0f, 1.0f),     // 0: albedo bg
        clear_color(0.0f, 0.0f, 0.0f, 0.0f),     // 1: normal bg (zero vec = background marker)
        clear_color(0.0f, 0.0f, 0.0f, 0.0f),     // 2: world-pos bg
        clear_depth(1.0f)                         // 3: depth
    ]
    record_render_pass(cmd, res.rp_gbuffer, res.fb_gbuffer, full_area(DEFERRED_W, DEFERRED_H), gb_clears) {
        cmd_bind_pipeline(cmd, res.gbuffer_pipeline)
        let g_sets <- [vk_value_to_boost(res.gbuffer_set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.gbuffer_pipe_layout, 0u, g_sets, no_dyn)
        draw_casters(res, cmd, res.gbuffer_pipe_layout)
    }
    delete gb_clears

    // ---- SSAO pass (samples the G-buffer normal + world-pos; writes the occlusion factor) ----
    var ssao_clears <- [clear_color(1.0f, 1.0f, 1.0f, 1.0f)]      // 1 = no darkening
    record_render_pass(cmd, res.rp_ssao, res.fb_ssao, full_area(DEFERRED_W, DEFERRED_H), ssao_clears) {
        cmd_bind_pipeline(cmd, res.ssao_pipeline)
        let ssao_sets <- [vk_value_to_boost(res.ssao_set)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.ssao_pipe_layout, 0u, ssao_sets, no_dyn)
        cmd_draw(cmd, 3u)
    }
    delete ssao_clears

    // ---- lighting pass (samples G-buffer + ssao + shadow + env; composites the final colour) ----
    var lit_clears <- [clear_color(0.05f, 0.05f, 0.08f, 1.0f)]
    record_render_pass(cmd, res.rp_lighting, res.fb_lighting, full_area(DEFERRED_W, DEFERRED_H), lit_clears) {
        cmd_bind_pipeline(cmd, res.lighting_pipeline)
        let l_sets0 <- [vk_value_to_boost(res.lighting_set0)]
        var no_dyn : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.lighting_pipe_layout, 0u, l_sets0, no_dyn)
        let l_sets1 <- [vk_value_to_boost(res.lighting_set1)]
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS, res.lighting_pipe_layout, 1u, l_sets1, no_dyn)
        lighting_fs_push_constants(cmd, res.lighting_pipe_layout)
        cmd_draw(cmd, 3u)
    }
    delete lit_clears
}

//! Draw cube + floor with the appropriate ObjectPush per object. Used by BOTH the shadow pass and
//! the gbuffer pass; the only difference is the bound pipeline.
def private draw_casters(res : DeferredResources; cmd : CommandBuffer; pipe_layout : PipelineLayout) {
    // Cat stays put (base y=0, baked into the verts) -- only the camera orbits; both moving reads as double motion.
    op.model = identity_m4()
    op.material = 0
    let cube_vbufs <- [weak_copy(res.cube_vb.buffer)]
    var cube_offs : array<uint64>
    cube_offs |> push(0ul)
    cmd_bind_vertex_buffers(cmd, 0u, cube_vbufs, cube_offs)
    cmd_bind_index_buffer(cmd, weak_copy(res.cube_ib.buffer), 0ul, VkIndexType.UINT32)
    push_object_pc(cmd, pipe_layout)
    cmd_draw_indexed(cmd, res.cat_index_count, 1u, 0u, 0, 0u)

    // Floor: identity transform
    op.model = identity_m4()
    op.material = 1
    let floor_vbufs <- [weak_copy(res.floor_vb.buffer)]
    var floor_offs : array<uint64>
    floor_offs |> push(0ul)
    cmd_bind_vertex_buffers(cmd, 0u, floor_vbufs, floor_offs)
    cmd_bind_index_buffer(cmd, weak_copy(res.floor_ib.buffer), 0ul, VkIndexType.UINT16)
    push_object_pc(cmd, pipe_layout)
    cmd_draw_indexed(cmd, uint(N_FLOOR_INDICES), 1u, 0u, 0, 0u)
}

def private push_object_pc(cmd : CommandBuffer; pipe_layout : PipelineLayout) {
    // ObjectPush is in shadow_vs + gbuffer_vs; the gbuffer-derived helper writes the full 68-byte range covering both.
    gbuffer_vs_push_constants(cmd, pipe_layout)
}

// ===== one-shot render (pixel oracle entry) =====

def public render_deferred_cube(time, camera_t : float; debug_mode : int) : array<uint8> {
    var inscope ctx <- build_deferred_context()
    return <- render_deferred_frame(ctx, time, camera_t, debug_mode)
}

def public render_deferred_frame(var ctx : DeferredContext; time, camera_t : float; debug_mode : int) : array<uint8> {
    update_deferred_uniforms(ctx.res, ctx.device, time, camera_t)
    dbg.debug_mode = debug_mode
    var pixels : array<uint8>
    run_cmd_sync(ctx.device, ctx.pool, ctx.queue) $(cmd) {
        record_deferred_render(ctx.res, cmd)
        copy_image_to_buffer(cmd, ctx.res.final_color.image, ctx.res.readback, DEFERRED_W, DEFERRED_H)
    }
    map_memory_to_array(ctx.device, ctx.res.readback.memory, ctx.res.buf_size) $(m) {
        pixels := m
    }
    return <- pixels
}

def public build_deferred_context() : DeferredContext {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var inscope instance <- create_instance("dasVulkan tutorial 10", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    // Enable samplerAnisotropy when the device advertises it (off on lavapipe/CI) so the brick
    // sampler can use anisotropic filtering; the sampler is gated on the same bit.
    var avail_feats : VkPhysicalDeviceFeatures
    vkGetPhysicalDeviceFeatures(phys, avail_feats)
    var want_feats : VkPhysicalDeviceFeatures
    want_feats.samplerAnisotropy = avail_feats.samplerAnisotropy
    let no_ext : array<string>
    var inscope device <- create_device(phys, gfx, no_ext, want_feats)
    volkLoadDevice(boost_value_to_vk(device))
    var queue = get_device_queue(device, gfx, 0u)
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var inscope res <- build_deferred_resources(device, phys, queue, pool)
    return <- DeferredContext(
        instance <- instance, device <- device, queue = queue, pool <- pool, res <- res)
}

// ===== UI helpers =====

def public px(pixels : array<uint8>; x, y : int) : int3 {
    let p = (y * DEFERRED_W + x) * 4
    return int3(int(pixels[p]), int(pixels[p + 1]), int(pixels[p + 2]))
}

def public debug_mode_label(m : int) : string {
    if (m == DBG_ALBEDO) return "albedo"
    if (m == DBG_NORMAL) return "normal"
    if (m == DBG_WORLDPOS) return "world-pos"
    if (m == DBG_SSAO) return "ssao"
    if (m == DBG_LIGHT) return "lighting-only"
    return "lit"
}

def public next_debug_mode(m : int) : int {
    return (m + 1) % DBG_MODE_COUNT
}

// ===== 7-state UX toggle (auto-cycle + 6 fixed modes) =====

enum public DeferredToggle {
    auto_toggle
    force_lit
    force_albedo
    force_normal
    force_worldpos
    force_ssao
    force_light
}

let public AUTO_PERIOD = 2.5f

def public deferred_active_mode(toggle : DeferredToggle; t : float) : int {
    if (toggle == DeferredToggle.force_lit) return DBG_LIT
    if (toggle == DeferredToggle.force_albedo) return DBG_ALBEDO
    if (toggle == DeferredToggle.force_normal) return DBG_NORMAL
    if (toggle == DeferredToggle.force_worldpos) return DBG_WORLDPOS
    if (toggle == DeferredToggle.force_ssao) return DBG_SSAO
    if (toggle == DeferredToggle.force_light) return DBG_LIGHT
    return int(floor(t / AUTO_PERIOD)) % DBG_MODE_COUNT
}

def public deferred_toggle_label(toggle : DeferredToggle) : string {
    if (toggle == DeferredToggle.force_lit) return "FORCE LIT"
    if (toggle == DeferredToggle.force_albedo) return "FORCE ALBEDO"
    if (toggle == DeferredToggle.force_normal) return "FORCE NORMAL"
    if (toggle == DeferredToggle.force_worldpos) return "FORCE WORLD-POS"
    if (toggle == DeferredToggle.force_ssao) return "FORCE SSAO"
    if (toggle == DeferredToggle.force_light) return "FORCE LIGHT"
    return "AUTO"
}

def public next_deferred_toggle(toggle : DeferredToggle) : DeferredToggle {
    if (toggle == DeferredToggle.auto_toggle) return DeferredToggle.force_lit
    if (toggle == DeferredToggle.force_lit) return DeferredToggle.force_albedo
    if (toggle == DeferredToggle.force_albedo) return DeferredToggle.force_normal
    if (toggle == DeferredToggle.force_normal) return DeferredToggle.force_worldpos
    if (toggle == DeferredToggle.force_worldpos) return DeferredToggle.force_ssao
    if (toggle == DeferredToggle.force_ssao) return DeferredToggle.force_light
    return DeferredToggle.auto_toggle
}
