options gen2
options _comment_hygiene = true
options indenting = 4

// glTF 2.0 PBR renderer: uploads a backend-neutral GltfScene (daslang's modules/dasGLTF loader)
// onto Vulkan resources and renders it with the same Cook-Torrance metal-rough PBR as the OpenGL
// adapter (gltf/gltf_pbr) — the BRDF / normal-perturbation / skinning math is shared via
// gltf/gltf_pbr_common, so the two backends shade identically by construction.
//
// Render contract: record gltf_vk_render inside a color+depth render pass whose color target is a
// UNORM format — the fragment shader sRGB-encodes its output (GL parity), so an _SRGB attachment
// would double-encode. Call gltf_vk_update BEFORE submitting the command buffer (UBO writes are
// host-visible memory, read at execution time). Per-shader UBO split (one @uniform block per
// shader, the [vulkan_*_shader] bind limit): rigid VS = view_proj; skinned VS = view_proj + the
// world-space joint palette (per-skin buffer); FS = light + material factors (per-material buffer,
// light half rewritten each frame). Rigid node transforms ride push constants (two float4x4 =
// the guaranteed 128-byte budget); the skinned path bakes `root` into the palette and never reads
// them, exactly like the GL renderer.
//
// Out of scope (mirrors gltf_gl/gltf_pbr where noted): points/lines primitives are skipped
// (pipelines are TRIANGLE_LIST); BLEND draws are not depth-sorted (GL parity); skins are clamped
// to GLTF_VK_MAX_JOINTS.

// NOT shared: requires vulkan_boost / vulkan_assets (both non-shared).
module vulkan_gltf

require vulkan public
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/vulkan_assets public
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require gltf/gltf_boost public
require gltf/gltf_pbr_common public
require daslib/math_boost
require math

// Max joints in the palette UBO. 64 mat4 = 4 KB, comfortably inside the 16 KB minimum UBO
// guarantee. Mirrors gltf_pbr's GLTF_MAX_JOINTS.
let public GLTF_VK_MAX_JOINTS = 64
// GLTF_VK_MAX_JOINTS * 4 — the palette as float4 columns. A literal because a fixed-array dim
// must fold under the lint compile (no optimizations), which a const expression does not.
let public GLTF_VK_PALETTE_VECS = 256

// ===== shaders =====
// Vertex locations mirror gltf_gl's GltfGlVertex contract (0..7): rigid consumes 0..3, skinned adds
// 6..7; uv1/color0 (4..5) are uploaded but unconsumed — a pipeline may provide attributes it ignores.

var @in @location = 0 a_position : float3
var @in @location = 1 a_normal : float3
var @in @location = 2 a_tangent : float4
var @in @location = 3 a_uv0 : float2
var @in @location = 6 a_joints0 : float4
var @in @location = 7 a_weights0 : float4

var @out @location = 0 v_world_pos : float3
var @out @location = 1 v_world_normal : float3
var @out @location = 2 v_world_tangent : float3
var @out @location = 3 v_tangent_w : float
var @out @location = 4 v_uv0 : float2

struct GltfVkCam {
    view_proj : float4x4
}
var @uniform @set = 0 @binding = 0 vg_cam : GltfVkCam

// The palette stores each joint matrix as 4 consecutive float4 COLUMNS (the SPIR-V emitter
// supports scalar/vector UBO array elements, not matrices). Same data as gltf_pbr's
// float4x4[64], world-space (joint.world * inverseBind) with root pre-baked.
struct GltfVkSkin {
    view_proj : float4x4
    joints : float4[GLTF_VK_PALETTE_VECS]
}
// same set/binding as vg_cam — legal because no single shader reaches both blocks
var @uniform @set = 0 @binding = 0 vg_skin : GltfVkSkin

struct GltfVkModelPC {
    model : float4x4
    normal_mat : float4x4   // transpose(inverse(model)); float4x4 keeps the push block matrix-only
}
var @push_constant vg_pc : GltfVkModelPC

[vulkan_vertex_shader(name="gltf_vk_vs_rigid_spv")]
def gltf_vk_vs_rigid {
    let world = vg_pc.model * float4(a_position, 1.0)
    v_world_pos = world.xyz
    v_world_normal = normalize((vg_pc.normal_mat * float4(a_normal, 0.0)).xyz)
    v_world_tangent = (vg_pc.normal_mat * float4(a_tangent.xyz, 0.0)).xyz
    v_tangent_w = a_tangent.w
    v_uv0 = a_uv0
    gl_Position = vg_cam.view_proj * world
}

[vulkan_vertex_shader(name="gltf_vk_vs_skinned_spv")]
def gltf_vk_vs_skinned {
    // clamp to the palette size: a skin with > GLTF_VK_MAX_JOINTS joints (or malformed JOINTS_0)
    // would otherwise index vg_skin.joints out of range -> undefined shader behavior
    let jx = clamp(int(a_joints0.x), 0, GLTF_VK_MAX_JOINTS - 1) * 4
    let jy = clamp(int(a_joints0.y), 0, GLTF_VK_MAX_JOINTS - 1) * 4
    let jz = clamp(int(a_joints0.z), 0, GLTF_VK_MAX_JOINTS - 1) * 4
    let jw = clamp(int(a_joints0.w), 0, GLTF_VK_MAX_JOINTS - 1) * 4
    // Linear-blend skinning on the palette COLUMNS: sum(w_i * (M_i * v)) == (sum(w_i * M_i)) * v,
    // so blending the 4 columns first, then applying the blended matrix column-major, is exactly
    // gltf_skin_point's result with fewer operations (the column storage is what the UBO supports).
    let c0 = (vg_skin.joints[jx] * a_weights0.x + vg_skin.joints[jy] * a_weights0.y
            + vg_skin.joints[jz] * a_weights0.z + vg_skin.joints[jw] * a_weights0.w)
    let c1 = (vg_skin.joints[jx + 1] * a_weights0.x + vg_skin.joints[jy + 1] * a_weights0.y
            + vg_skin.joints[jz + 1] * a_weights0.z + vg_skin.joints[jw + 1] * a_weights0.w)
    let c2 = (vg_skin.joints[jx + 2] * a_weights0.x + vg_skin.joints[jy + 2] * a_weights0.y
            + vg_skin.joints[jz + 2] * a_weights0.z + vg_skin.joints[jw + 2] * a_weights0.w)
    let c3 = (vg_skin.joints[jx + 3] * a_weights0.x + vg_skin.joints[jy + 3] * a_weights0.y
            + vg_skin.joints[jz + 3] * a_weights0.z + vg_skin.joints[jw + 3] * a_weights0.w)
    let world = c0 * a_position.x + c1 * a_position.y + c2 * a_position.z + c3
    let wnormal = (c0 * a_normal.x + c1 * a_normal.y + c2 * a_normal.z).xyz
    let wtangent = (c0 * a_tangent.x + c1 * a_tangent.y + c2 * a_tangent.z).xyz
    v_world_pos = world.xyz
    v_world_normal = normalize(wnormal)
    v_world_tangent = wtangent
    v_tangent_w = a_tangent.w
    v_uv0 = a_uv0
    gl_Position = vg_skin.view_proj * world
}

// FS data, one block per the bind limit: light (rewritten per frame) + material factors packed
// into float4 lanes.
struct GltfVkMat {
    cam_pos : float4            // xyz = camera position
    light_dir : float4          // xyz = direction TOWARD the light, normalized
    light_color : float4        // rgb = color * intensity
    ambient : float4            // rgb = constant ambient irradiance
    base_color_factor : float4
    mr_factors : float4         // x metallic, y roughness, z occlusion strength, w normal scale
    emissive_alpha : float4     // xyz emissive factor, w alpha cutoff
    mode_flags : float4         // x alpha mode 0/1/2, y environment amount, z environment max lod
}
var @uniform @set = 0 @binding = 1 vg_mat : GltfVkMat

// base-color / emissive are sRGB images (hardware decode -> linear); the rest are linear
var @set = 0 @binding = 2 vg_base_color_tex : sampler2D
var @set = 0 @binding = 3 vg_metal_rough_tex : sampler2D
var @set = 0 @binding = 4 vg_normal_tex : sampler2D
var @set = 0 @binding = 5 vg_occlusion_tex : sampler2D
var @set = 0 @binding = 6 vg_emissive_tex : sampler2D
// equirect HDR environment (linear radiance, mipmapped); amount 0 -> flat ambient (white bound)
var @set = 0 @binding = 7 vg_env_tex : sampler2D

var @in @location = 0 f_world_pos : float3
var @in @location = 1 f_world_normal : float3
var @in @location = 2 f_world_tangent : float3
var @in @location = 3 f_tangent_w : float
var @in @location = 4 f_uv0 : float2

var @out @location = 0 o_color : float4

[vulkan_fragment_shader(name="gltf_vk_fs_spv")]
def gltf_vk_fs {
    let uv = f_uv0
    let base = texture(vg_base_color_tex, uv) * vg_mat.base_color_factor
    let alpha_mode = vg_mat.mode_flags.x
    // MASK: hard cutoff; BLEND leaves alpha for the blend pipeline; OPAQUE forces alpha 1
    if (alpha_mode > 0.5 && alpha_mode < 1.5 && base.w < vg_mat.emissive_alpha.w) {
        discard()
    }
    let mr = texture(vg_metal_rough_tex, uv)
    let metallic = mr.z * vg_mat.mr_factors.x
    let roughness = clamp(mr.y * vg_mat.mr_factors.y, 0.04, 1.0)
    let occ = texture(vg_occlusion_tex, uv).x
    let emissive = texture(vg_emissive_tex, uv).xyz * vg_mat.emissive_alpha.xyz

    let albedo = base.xyz
    let n = gltf_perturb_normal(normalize(f_world_normal), f_world_tangent, f_tangent_w,
        texture(vg_normal_tex, uv).xyz, vg_mat.mr_factors.w)
    let v = normalize(vg_mat.cam_pos.xyz - f_world_pos)
    let l = normalize(vg_mat.light_dir.xyz)
    let direct = gltf_pbr_direct(albedo, metallic, roughness, n, v, l, vg_mat.light_color.xyz)
    var ambient = gltf_pbr_ambient(albedo, vg_mat.ambient.xyz, occ, vg_mat.mr_factors.z)
    if (vg_mat.mode_flags.y > 0.0) {
        // image-based ambient: irradiance from a deep mip along n, reflection from a
        // roughness-scaled mip along the mirror direction (mips as prefiltered environment);
        // clamp so a tiny environment (max lod < 2) can't produce a negative LOD
        let irr = textureLod(vg_env_tex, gltf_equirect_uv(n), max(vg_mat.mode_flags.z - 2.0, 0.0)).xyz
        let refl = textureLod(vg_env_tex, gltf_equirect_uv(gltf_reflect_dir(n, v)), roughness * vg_mat.mode_flags.z).xyz
        ambient = gltf_env_ambient(albedo, metallic, roughness, n, v, irr, refl,
            gltf_occlusion_factor(occ, vg_mat.mr_factors.z)) * vg_mat.mode_flags.y
    }

    let linear_out = ambient + direct + emissive
    // UNORM color target -> encode linear -> sRGB in the shader (GL parity)
    let out_rgb = gltf_srgb_encode(linear_out)
    let out_a = alpha_mode > 1.5 ? base.w : 1.0
    o_color = float4(out_rgb, out_a)
}

// ===== std140 buffer sizes (see the block structs above) =====

let private CAM_UBO_SIZE = 64ul                                          // one float4x4
let private SKIN_UBO_SIZE = uint64(64 + 64 * GLTF_VK_MAX_JOINTS)         // view_proj + palette
let private MAT_UBO_SIZE = 128ul                                         // 8 x float4

// ===== uploaded model =====

struct public GltfVkPrim {
    vb : HostBuffer
    ib : HostBuffer
    index_count : int
    material : int              // resolved material slot (length(scene.materials) = default slot)
}

struct public GltfVkMesh {
    prims : array<GltfVkPrim>
}

//! A GltfScene uploaded onto Vulkan: per-primitive vertex/index buffers, PBR textures + samplers,
//! per-material and per-skin UBOs, descriptor sets and pipeline variants. Create with
//! gltf_vk_upload, drive with gltf_vk_update + gltf_vk_render, `delete` to release.
struct public GltfVkModel {
    meshes : array<GltfVkMesh>              // parallel to scene.meshes (non-triangle prims skipped)
    textures : array<Texture>               // parallel to scene.textures; width == 0 => defaults bind
    samplers : array<Sampler>               // parallel to scene.samplers
    white_tex : Texture
    flat_normal_tex : Texture
    default_sampler : Sampler
    env_tex : Texture                       // equirect HDR environment; width == 0 => none
    env_max_lod : float
    cam_ubo : HostBuffer                    // rigid-path view_proj
    mat_ubos : array<HostBuffer>            // one per material + trailing default slot
    skin_ubos : array<HostBuffer>           // one per skin
    vert_rigid : ShaderModule
    vert_skinned : ShaderModule
    frag : ShaderModule
    set_layouts_rigid : array<DescriptorSetLayout>
    set_layouts_skinned : array<DescriptorSetLayout>
    pipe_layout_rigid : PipelineLayout
    pipe_layout_skinned : PipelineLayout
    desc_pool : DescriptorPool
    desc_sets : table<int; uint64>          // pair_key(mat, skin) -> raw VkDescriptorSet (pool-owned)
    pipelines : array<Pipeline>
    pipeline_index : table<int; int>        // variant_key -> pipelines index
}

def public finalize(var m : GltfVkModel) {
    unsafe {
        delete m.pipeline_index
        delete m.pipelines
        delete m.desc_sets
        delete m.desc_pool
        delete m.pipe_layout_skinned
        delete m.pipe_layout_rigid
        delete m.set_layouts_skinned
        delete m.set_layouts_rigid
        delete m.frag
        delete m.vert_skinned
        delete m.vert_rigid
        delete m.skin_ubos
        delete m.mat_ubos
        delete m.cam_ubo
        delete m.env_tex
        delete m.default_sampler
        delete m.flat_normal_tex
        delete m.white_tex
        delete m.samplers
        delete m.textures
        delete m.meshes
    }
}

// ===== private helpers =====

def private pair_key(mat_slot, skin_slot : int) : int => mat_slot * 4096 + skin_slot + 1

def private variant_key(skinned, blend, double_sided : bool) : int {
    return (skinned ? 1 : 0) | (blend ? 2 : 0) | (double_sided ? 4 : 0)
}

def private material_at(scene : GltfScene; mat_slot : int) : GltfMaterial {
    if (mat_slot >= 0 && mat_slot < length(scene.materials)) {
        let m = scene.materials[mat_slot]
        return m
    }
    return GltfMaterial()
}

def private vk_wrap_mode(mode : int) : VkSamplerAddressMode {
    if (mode == 33071) return VkSamplerAddressMode.CLAMP_TO_EDGE       // GL_CLAMP_TO_EDGE
    if (mode == 33648) return VkSamplerAddressMode.MIRRORED_REPEAT     // GL_MIRRORED_REPEAT
    return VkSamplerAddressMode.REPEAT                                 // 10497 / default
}

def private vk_mag_filter(f : int) : VkFilter => f == 9728 ? VkFilter.NEAREST : VkFilter.LINEAR   // GL_NEAREST

// glTF minFilter: 9728/9729 = unmipped NEAREST/LINEAR (base level pinned via mip NEAREST + maxLod 0,
// the Vulkan-spec GL-emulation note), 9984..9987 = <min>_MIPMAP_<mip>, absent (0) = trilinear default
def private vk_min_filter(f : int) : VkFilter => (f == 9728 || f == 9984 || f == 9986) ? VkFilter.NEAREST : VkFilter.LINEAR

def private vk_mipmap_mode(f : int) : VkSamplerMipmapMode => (f == 9728 || f == 9729 || f == 9984 || f == 9985) ? VkSamplerMipmapMode.NEAREST : VkSamplerMipmapMode.LINEAR

def private vk_max_lod(f : int) : float => (f == 9728 || f == 9729) ? 0.0 : 32.0

def private push_attr(var attrs : array<VkVertexInputAttributeDescription>; loc : uint; fmt : VkFormat; off : uint) {
    var a : VkVertexInputAttributeDescription
    a.location = loc
    a.binding = 0u
    a.format = fmt
    a.offset = off
    attrs |> push(a)
}

// GltfVertex is tightly packed (float3 = 12 bytes in daslang), so the CPU array uploads verbatim:
// position 0, normal 12, tangent 24, uv0 40, uv1 48, color0 56, joints0 72, weights0 88 — 104 stride.
def private gltf_vertex_layout(var bindings : array<VkVertexInputBindingDescription>;
                               var attrs : array<VkVertexInputAttributeDescription>) {
    var b : VkVertexInputBindingDescription
    b.binding = 0u
    b.stride = uint(typeinfo sizeof(type<GltfVertex>))
    b.inputRate = VkVertexInputRate.VERTEX
    bindings |> push(b)
    push_attr(attrs, 0u, VkFormat.R32G32B32_SFLOAT, 0u)        // position
    push_attr(attrs, 1u, VkFormat.R32G32B32_SFLOAT, 12u)       // normal
    push_attr(attrs, 2u, VkFormat.R32G32B32A32_SFLOAT, 24u)    // tangent
    push_attr(attrs, 3u, VkFormat.R32G32_SFLOAT, 40u)          // uv0
    push_attr(attrs, 6u, VkFormat.R32G32B32A32_SFLOAT, 72u)    // joints0
    push_attr(attrs, 7u, VkFormat.R32G32B32A32_SFLOAT, 88u)    // weights0
}

// v3d variant with blend + double-sided control (v3d hardcodes back-cull + opaque). Always built
// with dynamic viewport/scissor so one pipeline serves any target size — gltf_vk_render sets them.
def private create_gltf_pipeline(device : Device; render_pass : RenderPass; layout : PipelineLayout;   // nolint:STYLE038 - flat one-call-per-item pipeline setup
                                 vert, frag : ShaderModule; blend_enable, double_sided : bool) : Pipeline {
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
    var vattrs : array<VkVertexInputAttributeDescription>
    gltf_vertex_layout(vbindings, vattrs)

    var vinput = VkPipelineVertexInputStateCreateInfo()
    vinput.vertexBindingDescriptionCount = uint(length(vbindings))
    vinput.vertexAttributeDescriptionCount = uint(length(vattrs))
    var iasm = VkPipelineInputAssemblyStateCreateInfo()
    iasm.topology = VkPrimitiveTopology.TRIANGLE_LIST

    var vpstate = VkPipelineViewportStateCreateInfo()
    vpstate.viewportCount = 1u
    vpstate.scissorCount = 1u

    var raster = VkPipelineRasterizationStateCreateInfo()
    raster.polygonMode = VkPolygonMode.FILL
    if (!double_sided) {
        raster.cullMode.back = true
    }
    raster.frontFace = VkFrontFace.COUNTER_CLOCKWISE
    raster.lineWidth = 1.0f
    var msaa = VkPipelineMultisampleStateCreateInfo()
    msaa.rasterizationSamples._1 = true

    // depth write stays on for BLEND too — mirrors the GL renderer (no depth-sorting either way)
    var depthstate = VkPipelineDepthStencilStateCreateInfo()
    depthstate.depthTestEnable = VK_TRUE
    depthstate.depthWriteEnable = VK_TRUE
    depthstate.depthCompareOp = VkCompareOp.LESS_OR_EQUAL
    depthstate.minDepthBounds = 0.0f
    depthstate.maxDepthBounds = 1.0f

    var blend_att : VkPipelineColorBlendAttachmentState
    blend_att.colorWriteMask.r = true
    blend_att.colorWriteMask.g = true
    blend_att.colorWriteMask.b = true
    blend_att.colorWriteMask.a = true
    if (blend_enable) {
        blend_att.blendEnable = VK_TRUE
        blend_att.srcColorBlendFactor = VkBlendFactor.SRC_ALPHA
        blend_att.dstColorBlendFactor = VkBlendFactor.ONE_MINUS_SRC_ALPHA
        blend_att.colorBlendOp = VkBlendOp.ADD
        blend_att.srcAlphaBlendFactor = VkBlendFactor.ONE
        blend_att.dstAlphaBlendFactor = VkBlendFactor.ONE_MINUS_SRC_ALPHA
        blend_att.alphaBlendOp = VkBlendOp.ADD
    }
    var blend = VkPipelineColorBlendStateCreateInfo()
    blend.attachmentCount = 1u

    var dyn_states <- [VkDynamicState.VIEWPORT, VkDynamicState.SCISSOR]
    var dynstate = VkPipelineDynamicStateCreateInfo()
    dynstate.dynamicStateCount = uint(length(dyn_states))

    var gp = VkGraphicsPipelineCreateInfo()
    gp.stageCount = 2u
    gp.layout = boost_value_to_vk(layout)
    gp.renderPass = boost_value_to_vk(render_pass)
    var h : VkPipeline
    unsafe {
        vinput.pVertexBindingDescriptions = addr(vbindings[0])
        vinput.pVertexAttributeDescriptions = addr(vattrs[0])
        dynstate.pDynamicStates = addr(dyn_states[0])
        gp.pDynamicState = addr(dynstate)
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
    delete dyn_states
    delete vattrs
    delete vbindings
    delete stages
    var b = vk_value_to_boost(h)
    b._needs_delete = true
    b._device = unsafe(reinterpret<uint64>(boost_value_to_vk(device)))
    return <- b
}

// ===== upload =====

//! Upload `scene` onto Vulkan resources against a caller-owned (device, phys, queue, pool). `render_pass`
//! must be a color+depth pass with a UNORM color format (the shader sRGB-encodes); dynamic viewport/scissor
//! means any target size works. The pool's queue family must match `queue` (texture staging records into it).
def public gltf_vk_upload(device : Device; phys : VkPhysicalDevice; queue : VkQueue; pool : CommandPool;   // nolint:STYLE037,STYLE038 - linear gltf-to-device upload walk, arms per accessor kind
                          render_pass : RenderPass; scene : GltfScene) : GltfVkModel {
    let dev = boost_value_to_vk(device)
    var model : GltfVkModel

    // vertex/index buffers per triangle primitive
    var vb_usage : VkBufferUsageFlags
    vb_usage.vertex_buffer = true
    var ib_usage : VkBufferUsageFlags
    ib_usage.index_buffer = true
    model.meshes |> reserve(length(scene.meshes))
    for (mesh in scene.meshes) {
        var vkm : GltfVkMesh
        vkm.prims |> reserve(length(mesh.primitives))
        for (prim in mesh.primitives) {
            continue if (prim.mode != GltfPrimitiveMode.triangles || empty(prim.vertices) || empty(prim.indices))
            var verts_copy := prim.vertices
            var idx_copy := prim.indices
            var vkp = GltfVkPrim(
                vb <- create_host_buffer_from_bytes(device, phys, vb_usage, verts_copy),
                ib <- create_host_buffer_from_bytes(device, phys, ib_usage, idx_copy),
                index_count = length(prim.indices),
                material = ((prim.material >= 0 && prim.material < length(scene.materials))
                    ? prim.material : length(scene.materials)))
            delete verts_copy
            delete idx_copy
            vkm.prims |> emplace(vkp)
        }
        model.meshes |> emplace(vkm)
    }

    // textures: sRGB for base-color/emissive usage, UNORM for data maps (normal/MR/occlusion)
    var srgb : array<bool>
    srgb |> resize(length(scene.textures))
    for (m in scene.materials) {
        if (m.baseColorTex >= 0 && m.baseColorTex < length(srgb)) {
            srgb[m.baseColorTex] = true
        }
        if (m.emissiveTex >= 0 && m.emissiveTex < length(srgb)) {
            srgb[m.emissiveTex] = true
        }
    }
    model.textures |> reserve(length(scene.textures))
    for (ti in range(length(scene.textures))) {
        var t : Texture
        let img_idx = scene.textures[ti].image
        if (img_idx >= 0 && img_idx < length(scene.images)) {
            let img & = unsafe(scene.images[img_idx])
            if (img.width > 0 && img.height > 0 && !empty(img.pixels)) {
                var px_copy := img.pixels
                t <- upload_texture(device, phys, queue, pool, img.width, img.height,
                    srgb[ti] ? VkFormat.R8G8B8A8_SRGB : VkFormat.R8G8B8A8_UNORM, px_copy)
                delete px_copy
            }
        }
        model.textures |> emplace(t)
    }
    delete srgb
    var white_px <- [255u8, 255u8, 255u8, 255u8]
    model.white_tex <- upload_texture(device, phys, queue, pool, 1, 1, VkFormat.R8G8B8A8_UNORM, white_px, false)
    delete white_px
    var flat_px <- [128u8, 128u8, 255u8, 255u8]
    model.flat_normal_tex <- upload_texture(device, phys, queue, pool, 1, 1, VkFormat.R8G8B8A8_UNORM, flat_px, false)
    delete flat_px

    // samplers
    model.samplers |> reserve(length(scene.samplers))
    for (js in scene.samplers) {
        let sci = SamplerCreateInfo(
            magFilter = vk_mag_filter(js.magFilter),
            minFilter = vk_min_filter(js.minFilter),
            mipmapMode = vk_mipmap_mode(js.minFilter),
            addressModeU = vk_wrap_mode(js.wrapS),
            addressModeV = vk_wrap_mode(js.wrapT),
            addressModeW = VkSamplerAddressMode.REPEAT,
            maxLod = vk_max_lod(js.minFilter))
        var smp <- create_sampler(device, sci)
        model.samplers |> emplace(smp)
    }
    let dsci = SamplerCreateInfo(
        magFilter = VkFilter.LINEAR,
        minFilter = VkFilter.LINEAR,
        mipmapMode = VkSamplerMipmapMode.LINEAR,
        addressModeU = VkSamplerAddressMode.REPEAT,
        addressModeV = VkSamplerAddressMode.REPEAT,
        addressModeW = VkSamplerAddressMode.REPEAT,
        maxLod = 32.0f)
    model.default_sampler <- create_sampler(device, dsci)

    // UBO buffers
    var ubo_usage : VkBufferUsageFlags
    ubo_usage.uniform_buffer = true
    model.cam_ubo <- create_host_buffer(device, phys, CAM_UBO_SIZE, ubo_usage)
    let nmat = length(scene.materials)
    model.mat_ubos |> reserve(nmat + 1)
    for (_mi in range(nmat + 1)) {
        var hb <- create_host_buffer(device, phys, MAT_UBO_SIZE, ubo_usage)
        model.mat_ubos |> emplace(hb)
    }
    model.skin_ubos |> reserve(length(scene.skins))
    for (_si in range(length(scene.skins))) {
        var hb <- create_host_buffer(device, phys, SKIN_UBO_SIZE, ubo_usage)
        model.skin_ubos |> emplace(hb)
    }

    // shader modules + reflection-driven layouts, one pair per vertex-path variant
    model.vert_rigid <- create_shader_module(device, gltf_vk_vs_rigid_spv)
    model.vert_skinned <- create_shader_module(device, gltf_vk_vs_skinned_spv)
    model.frag <- create_shader_module(device, gltf_vk_fs_spv)
    var refl_rigid <- [decode_reflection(gltf_vk_vs_rigid_spv_reflect), decode_reflection(gltf_vk_fs_spv_reflect)]
    var refl_skinned <- [decode_reflection(gltf_vk_vs_skinned_spv_reflect), decode_reflection(gltf_vk_fs_spv_reflect)]
    model.set_layouts_rigid <- build_descriptor_set_layouts(device, refl_rigid)
    model.set_layouts_skinned <- build_descriptor_set_layouts(device, refl_skinned)
    model.pipe_layout_rigid <- build_pipeline_layout(device, model.set_layouts_rigid, refl_rigid)
    model.pipe_layout_skinned <- build_pipeline_layout(device, model.set_layouts_skinned, refl_skinned)
    delete refl_rigid
    delete refl_skinned

    // walk the node instances once: which (material, skin) descriptor pairs and which pipeline
    // variants does this scene actually draw?
    var pair_keys : table<int>
    var variant_keys : table<int>
    for (node in scene.nodes) {
        continue if (node.mesh < 0 || node.mesh >= length(model.meshes))
        let skin_slot = (node.skin >= 0 && node.skin < length(model.skin_ubos)) ? node.skin : -1
        for (prim in model.meshes[node.mesh].prims) {
            pair_keys |> insert(pair_key(prim.material, skin_slot))
            let mat = material_at(scene, prim.material)
            variant_keys |> insert(variant_key(skin_slot >= 0, mat.alphaMode == GltfAlphaMode.blend, mat.doubleSided))
        }
    }

    // descriptor pool + one set per (material, skin) pair
    let nsets = max(1, length(pair_keys))
    var dpci = DescriptorPoolCreateInfo(
        maxSets = uint(nsets),
        pPoolSizes <- [
            DescriptorPoolSize(type_ = VkDescriptorType.UNIFORM_BUFFER, descriptorCount = uint(nsets * 2)),
            DescriptorPoolSize(type_ = VkDescriptorType.COMBINED_IMAGE_SAMPLER, descriptorCount = uint(nsets * 6))])
    model.desc_pool <- create_descriptor_pool(device, dpci)
    for (k in keys(pair_keys)) {
        let skin_slot = (k % 4096) - 1
        let mat_slot = k / 4096
        let skinned = skin_slot >= 0
        var dsai = VkDescriptorSetAllocateInfo()
        dsai.descriptorPool = boost_value_to_vk(model.desc_pool)
        dsai.descriptorSetCount = 1u
        var raw_layout = boost_value_to_vk(skinned ? model.set_layouts_skinned[0] : model.set_layouts_rigid[0])
        var raw_set : VkDescriptorSet
        unsafe {
            dsai.pSetLayouts = addr(raw_layout)
            vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
        }
        write_gltf_set(device, model, scene, raw_set, mat_slot, skin_slot)
        model.desc_sets[k] = unsafe(reinterpret<uint64>(raw_set))
    }
    delete pair_keys

    // pipeline variants
    for (vk_ in keys(variant_keys)) {
        let skinned = (vk_ & 1) != 0
        let blend = (vk_ & 2) != 0
        let double_sided = (vk_ & 4) != 0
        var p <- create_gltf_pipeline(device, render_pass,
            skinned ? model.pipe_layout_skinned : model.pipe_layout_rigid,
            skinned ? model.vert_skinned : model.vert_rigid, model.frag,
            blend, double_sided)
        model.pipelines |> emplace(p)
        model.pipeline_index[vk_] = length(model.pipelines) - 1
    }
    delete variant_keys

    return <- model
}

// bindings: 0 = vertex UBO (cam or skin palette), 1 = material UBO, 2..6 = the five PBR samplers
def private write_gltf_set(device : Device; model : GltfVkModel; scene : GltfScene;
                           raw_set : VkDescriptorSet; mat_slot, skin_slot : int) {
    let mat = material_at(scene, mat_slot)
    var writes : array<WriteDescriptorSet>

    var w0 = WriteDescriptorSet(
        dstSet = vk_value_to_boost(raw_set),
        dstBinding = 0u,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER,
        descriptorCount = 1u)
    if (skin_slot >= 0) {
        w0.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(model.skin_ubos[skin_slot].buffer), range_ = SKIN_UBO_SIZE))
    } else {
        w0.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(model.cam_ubo.buffer), range_ = CAM_UBO_SIZE))
    }
    writes |> emplace(w0)

    var w1 = WriteDescriptorSet(
        dstSet = vk_value_to_boost(raw_set),
        dstBinding = 1u,
        descriptorType = VkDescriptorType.UNIFORM_BUFFER,
        descriptorCount = 1u)
    w1.pBufferInfo |> push(DescriptorBufferInfo(buffer = weak_copy(model.mat_ubos[min(mat_slot, length(model.mat_ubos) - 1)].buffer), range_ = MAT_UBO_SIZE))
    writes |> emplace(w1)

    write_tex_binding(writes, model, scene, raw_set, 2u, mat.baseColorTex, false)
    write_tex_binding(writes, model, scene, raw_set, 3u, mat.metalRoughTex, false)
    write_tex_binding(writes, model, scene, raw_set, 4u, mat.normalTex, true)
    write_tex_binding(writes, model, scene, raw_set, 5u, mat.occlusionTex, false)
    write_tex_binding(writes, model, scene, raw_set, 6u, mat.emissiveTex, false)
    writes |> emplace_env_write(model, raw_set)

    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)
    unsafe {
        delete writes
    }
}

// binding 7: the environment (white when none is set — the shader gates on mode_flags.y anyway)
def private emplace_env_write(var writes : array<WriteDescriptorSet>; model : GltfVkModel; raw_set : VkDescriptorSet) {
    var w = WriteDescriptorSet(
        dstSet = vk_value_to_boost(raw_set),
        dstBinding = 7u,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER,
        descriptorCount = 1u)
    let view = model.env_tex.width > 0 ? weak_copy(model.env_tex.view) : weak_copy(model.white_tex.view)
    w.pImageInfo |> push(DescriptorImageInfo(
        sampler = weak_copy(model.default_sampler),
        imageView = view,
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL))
    writes |> emplace(w)
}

//! Attach an equirect (2:1 lat-long) HDR environment — e.g. vulkan_assets' load_texture_hdr, which builds
//! the mip chain the roughness lookup needs. Takes ownership of `env`; call between frames (device idle).
//! `GltfPbrLight.environment` scales it per frame; without an environment the flat ambient is used.
def public gltf_vk_set_environment(var model : GltfVkModel; device : Device; var env : Texture) {
    unsafe {
        delete model.env_tex
    }
    model.env_tex <- env
    model.env_max_lod = (model.env_tex.width > 0
        ? floor(log2(float(max(model.env_tex.width, model.env_tex.height)))) : 0.0)
    for (raw in values(model.desc_sets)) {
        var writes : array<WriteDescriptorSet>
        writes |> emplace_env_write(model, unsafe(reinterpret<VkDescriptorSet>(raw)))
        let no_copies : array<CopyDescriptorSet>
        update_descriptor_sets(device, writes, no_copies)
        unsafe {
            delete writes
        }
    }
}

// resolve a glTF texture slot to (view, sampler), falling back to the white / flat-normal defaults
def private write_tex_binding(var writes : array<WriteDescriptorSet>; model : GltfVkModel; scene : GltfScene;
                              raw_set : VkDescriptorSet; binding : uint; tex_idx : int; is_normal : bool) {
    var view_src = is_normal ? weak_copy(model.flat_normal_tex.view) : weak_copy(model.white_tex.view)
    var sampler_src = weak_copy(model.default_sampler)
    if (tex_idx >= 0 && tex_idx < length(model.textures) && model.textures[tex_idx].width > 0) {
        view_src = weak_copy(model.textures[tex_idx].view)
        let smp = scene.textures[tex_idx].sampler
        if (smp >= 0 && smp < length(model.samplers)) {
            sampler_src = weak_copy(model.samplers[smp])
        }
    }
    var w = WriteDescriptorSet(
        dstSet = vk_value_to_boost(raw_set),
        dstBinding = binding,
        descriptorType = VkDescriptorType.COMBINED_IMAGE_SAMPLER,
        descriptorCount = 1u)
    w.pImageInfo |> push(DescriptorImageInfo(
        sampler = sampler_src,
        imageView = view_src,
        imageLayout = VkImageLayout.SHADER_READ_ONLY_OPTIMAL))
    writes |> emplace(w)
}

// ===== per-frame update + record =====

def private fill_mat_globals(mat : GltfMaterial; cam_pos : float3; light : GltfPbrLight; env_amount, env_max_lod : float) {
    vg_mat.cam_pos = float4(cam_pos, 0.0)
    vg_mat.light_dir = float4(normalize(light.direction), 0.0)
    vg_mat.light_color = float4(light.color * light.intensity, 0.0)
    vg_mat.ambient = float4(light.ambient, 0.0)
    vg_mat.base_color_factor = mat.baseColorFactor
    vg_mat.mr_factors = float4(mat.metallicFactor, mat.roughnessFactor, mat.occlusionStrength, mat.normalScale)
    vg_mat.emissive_alpha = float4(mat.emissiveFactor, mat.alphaCutoff)
    let mode = mat.alphaMode == GltfAlphaMode.mask ? 1.0 : (mat.alphaMode == GltfAlphaMode.blend ? 2.0 : 0.0)
    vg_mat.mode_flags = float4(mode, env_amount, env_max_lod, 0.0)
}

//! Write the frame's UBOs: rigid view_proj, every skin's (root-baked) joint palette, and every
//! material's light+factors block. Host-visible memory — call BEFORE submitting the command buffer
//! that gltf_vk_render recorded (with run_cmd_sync / draw_frame that means: call this first).
def public gltf_vk_update(device : Device; model : GltfVkModel; scene : GltfScene; root : float4x4;
                          view, proj : float4x4; cam_pos : float3; light : GltfPbrLight) {
    let vp = proj * view
    vg_cam.view_proj = vp
    gltf_vk_vs_rigid_bind_uniform(device, model.cam_ubo.memory)
    for (s in range(length(model.skin_ubos))) {
        vg_skin.view_proj = vp
        let njoint = min(length(scene.skins[s].joints), GLTF_VK_MAX_JOINTS)
        for (j in range(njoint)) {
            let jm = root * gltf_joint_matrix(scene, s, j)
            for (c in range(4)) {
                vg_skin.joints[j * 4 + c] = jm[c]
            }
        }
        // clear unused slots so a clamped joint index reads identity, not a stale palette entry
        let idm = identity4x4()
        for (j in range(njoint, GLTF_VK_MAX_JOINTS)) {
            for (c in range(4)) {
                vg_skin.joints[j * 4 + c] = idm[c]
            }
        }
        gltf_vk_vs_skinned_bind_uniform(device, model.skin_ubos[s].memory)
    }
    let env_amount = model.env_tex.width > 0 ? light.environment : 0.0
    for (m in range(length(model.mat_ubos))) {
        fill_mat_globals(material_at(scene, m), cam_pos, light, env_amount, model.env_max_lod)
        gltf_vk_fs_bind_uniform(device, model.mat_ubos[m].memory)
    }
}

//! Record every mesh instance in `scene` into `cmd` — call inside a color+depth render pass on a UNORM
//! color target. Skinned nodes draw through the skinned pipeline (palette carries `root`); rigid nodes
//! push `root * node.world` + its normal matrix. Dynamic viewport/scissor: (width, height) sets both.
def public gltf_vk_render(cmd : CommandBuffer; model : GltfVkModel; scene : GltfScene; root : float4x4;
                          width, height : int) {
    var vps : array<Viewport>
    vps |> push(Viewport(width = float(width), height = float(height), maxDepth = 1.0f))
    cmd_set_viewport(cmd, 0u, vps)
    delete vps
    var sci : Rect2D
    sci.extent.width = uint(width)
    sci.extent.height = uint(height)
    var scis : array<Rect2D>
    scis |> push(sci)
    cmd_set_scissor(cmd, 0u, scis)
    delete scis

    var sets : array<DescriptorSet>
    var vbufs : array<Buffer>
    var voffs : array<uint64>
    voffs |> push(0ul)
    for (node in scene.nodes) {
        continue if (node.mesh < 0 || node.mesh >= length(model.meshes))
        let skin_slot = (node.skin >= 0 && node.skin < length(model.skin_ubos)) ? node.skin : -1
        let skinned = skin_slot >= 0
        if (!skinned) {
            let mdl = root * node.world
            vg_pc.model = mdl
            vg_pc.normal_mat = transpose(inverse(mdl))
        }
        for (prim in model.meshes[node.mesh].prims) {
            let mat = material_at(scene, prim.material)
            let vkey = variant_key(skinned, mat.alphaMode == GltfAlphaMode.blend, mat.doubleSided)
            let pidx = model.pipeline_index?[vkey] ?? -1
            let raw_set = model.desc_sets?[pair_key(prim.material, skin_slot)] ?? 0ul
            continue if (pidx < 0 || raw_set == 0ul)
            cmd_bind_pipeline(cmd, model.pipelines[pidx])
            sets |> clear()
            sets |> push(vk_value_to_boost(unsafe(reinterpret<VkDescriptorSet>(raw_set))))
            var no_dyn : array<uint>
            cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.GRAPHICS,
                skinned ? model.pipe_layout_skinned : model.pipe_layout_rigid, 0u, sets, no_dyn)
            delete no_dyn
            if (!skinned) {
                gltf_vk_vs_rigid_push_constants(cmd, model.pipe_layout_rigid)
            }
            vbufs |> clear()
            vbufs |> push(weak_copy(prim.vb.buffer))
            cmd_bind_vertex_buffers(cmd, 0u, vbufs, voffs)
            cmd_bind_index_buffer(cmd, weak_copy(prim.ib.buffer), 0ul, VkIndexType.UINT32)
            cmd_draw_indexed(cmd, uint(prim.index_count), 1u, 0u, 0, 0u)
        }
    }
    delete sets
    delete vbufs
    delete voffs
}
