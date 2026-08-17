options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 12 -- GPU-driven rendering: Hi-Z occlusion culling + indirect draw + bindless materials.
// The GPU itself decides what to draw: a compute shader tests every instance against a hierarchical-Z
// depth pyramid, compacts the survivors into an indirect-draw buffer + a GPU-written count, and the
// draw reads that count -- the CPU never learns how many objects survive. Every shader is daslang,
// lowered to SPIR-V at compile time by dasSpirv.
//
// This file builds up in the order the frame runs: (1) the Hi-Z depth pyramid (below), then the cull
// compute, then the two draw views. All authored against the R32f storage-image @format qualifier
// (8-bit Rgba8 is unusable for occlusion depth).

module gpu_driven_tut_shaders public

require vulkan/vulkan_boost public
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// ===== Hi-Z depth pyramid build =====
// Standard depth (0 = near, 1 = far). Each HZB texel holds the MAX depth of the region it covers (the
// FARTHEST occluder), so an object is occluded iff its nearest depth exceeds the HZB value over its
// screen footprint. mip 0 copies the occluder depth 1:1; each finer level is the max of the 2x2 below.

var @set = 0 @binding = 0 hzb_src : sampler2D                    // read: depth buffer (mip0) or HZB level L-1
var @set = 0 @binding = 1 @format = "r32f" hzb_dst : image2D     // write: HZB level L (R32f storage)

struct HzbPush {
    src_lod : int       // mip level to read via texelFetch (0 when reading the depth buffer for mip 0)
}
var @push_constant hzb_pc : HzbPush

// mip 0: copy the occluder depth buffer into HZB level 0 (1:1, lod 0 of the depth sampler).
[vulkan_compute_shader(local_size_x=8, local_size_y=8, name="hzb_mip0_spv")]
def hzb_mip0 {
    let gid = gl_GlobalInvocationID
    let c = int2(int(gid.x), int(gid.y))
    let d = texelFetch(hzb_src, c, 0)
    imageStore(hzb_dst, c, float4(d.x, 0.0f, 0.0f, 1.0f))
}

// downsample: HZB[L][c] = max over the 2x2 footprint in level L-1 (conservative farthest occluder).
// Every level is a power-of-two, exact 2x reduction and the host dispatches exactly (dim/8) groups, so
// the 2x2 reads (s .. s+(1,1)) are always in bounds -- no edge-clamp and no out-of-bounds fetch.
[vulkan_compute_shader(local_size_x=8, local_size_y=8, name="hzb_down_spv")]
def hzb_down {
    let gid = gl_GlobalInvocationID
    let c = int2(int(gid.x), int(gid.y))
    let s = c * 2
    let lod = hzb_pc.src_lod
    let a = texelFetch(hzb_src, s, lod).x
    let b = texelFetch(hzb_src, s + int2(1, 0), lod).x
    let e = texelFetch(hzb_src, s + int2(0, 1), lod).x
    let f = texelFetch(hzb_src, s + int2(1, 1), lod).x
    let m = max(max(a, b), max(e, f))
    imageStore(hzb_dst, c, float4(m, 0.0f, 0.0f, 1.0f))
}

// ===== instance scene + cull compute =====

// One axis-aligned cube per instance (no rotation): the world AABB is pos +/- 0.5*scale. Counts are
// carried as floats in float4s (cast in-shader) to stay on the codegen paths the UBO emitter handles
// cleanly -- trailing scalar UBO fields are a known rough edge.

struct CubeInstance {
    pos_scale : float4    // xyz = world center, w = uniform scale
    mat_idx   : int       // bindless material index
    pad0      : int
    pad1      : int
    pad2      : int       // std430 pad to 32 bytes
}

// VkDrawIndexedIndirectCommand layout (5 x 32-bit), appended by the cull shader, consumed by
// vkCmdDrawIndexedIndirectCount.
struct DrawCmd {
    index_count    : uint
    instance_count : uint
    first_index    : uint
    vertex_offset  : int
    first_instance : uint
}

struct Scene {
    view_proj     : float4x4    // main camera (the culling + main-draw viewpoint)
    god_view_proj : float4x4    // god camera (the reveal viewpoint, sees the occluder from the side)
    hzb_dims      : float4      // x,y = HZB base size (px); z = mip count; w = instance count
    params        : float4      // x = index count per cube (36); yzw unused
}

var @uniform @set = 0 @binding = 0 scene : Scene
var @ssbo @set = 0 @binding = 1 instances : array<CubeInstance>
var @ssbo @set = 0 @binding = 2 draw_cmds : array<DrawCmd>
var @ssbo @set = 0 @binding = 3 draw_count : array<uint>
var @ssbo @set = 0 @binding = 4 draw_instance : array<uint>
var @ssbo @set = 0 @binding = 5 cull_reason : array<uint>
var @set = 0 @binding = 6 hzb_tex : sampler2D

// One invocation per instance: project the world AABB, frustum-test, then Hi-Z occlusion-test against
// the depth pyramid. Survivors append a draw command + their instance id (atomic compaction) and get
// cull_reason 0; the god view colours the rest by reason (1 = frustum, 2 = occluded).
[vulkan_compute_shader(local_size_x=64, name="cull_spv")]
def cull_cs {
    let i = int(gl_GlobalInvocationID.x)
    if (i >= int(scene.hzb_dims.w)) return
    let inst = instances[i]
    let cen = inst.pos_scale.xyz
    let hf = inst.pos_scale.w * 0.5f
    // project the 8 world-AABB corners (unrolled -- the emitter keeps screen-AABB accumulation simplest
    // as straight-line lets rather than loop-carried vector state).
    let lo3 = cen - float3(hf, hf, hf)
    let hi3 = cen + float3(hf, hf, hf)
    let c0 = scene.view_proj * float4(lo3.x, lo3.y, lo3.z, 1.0f)
    let c1 = scene.view_proj * float4(hi3.x, lo3.y, lo3.z, 1.0f)
    let c2 = scene.view_proj * float4(lo3.x, hi3.y, lo3.z, 1.0f)
    let c3 = scene.view_proj * float4(hi3.x, hi3.y, lo3.z, 1.0f)
    let c4 = scene.view_proj * float4(lo3.x, lo3.y, hi3.z, 1.0f)
    let c5 = scene.view_proj * float4(hi3.x, lo3.y, hi3.z, 1.0f)
    let c6 = scene.view_proj * float4(lo3.x, hi3.y, hi3.z, 1.0f)
    let c7 = scene.view_proj * float4(hi3.x, hi3.y, hi3.z, 1.0f)
    let min_w = min(min(min(c0.w, c1.w), min(c2.w, c3.w)), min(min(c4.w, c5.w), min(c6.w, c7.w)))
    // Divide each corner by its own w to get NDC, unconditionally. If the AABB crosses the near plane
    // (min_w ~ 0) the projection is garbage, but `near_cross` wins the decision below and the instance
    // is kept, so the garbage is never acted on -- this keeps the shader flat (no deep nesting).
    let n0 = c0.xyz * (1.0f / c0.w)
    let n1 = c1.xyz * (1.0f / c1.w)
    let n2 = c2.xyz * (1.0f / c2.w)
    let n3 = c3.xyz * (1.0f / c3.w)
    let n4 = c4.xyz * (1.0f / c4.w)
    let n5 = c5.xyz * (1.0f / c5.w)
    let n6 = c6.xyz * (1.0f / c6.w)
    let n7 = c7.xyz * (1.0f / c7.w)
    let lo = min(min(min(n0, n1), min(n2, n3)), min(min(n4, n5), min(n6, n7)))
    let hi = max(max(max(n0, n1), max(n2, n3)), max(max(n4, n5), max(n6, n7)))
    let near_z = lo.z
    // Hi-Z occlusion sample: pick the mip where the AABB spans ~2x2 texels, sample its footprint, take
    // the farthest occluder. Computed unconditionally; the result only matters when the AABB is on-screen.
    let mip_count = int(scene.hzb_dims.z)
    let base = scene.hzb_dims.xy
    let uv_min = clamp(lo.xy * 0.5f + float2(0.5f, 0.5f), float2(0.0f, 0.0f), float2(1.0f, 1.0f))
    let uv_max = clamp(hi.xy * 0.5f + float2(0.5f, 0.5f), float2(0.0f, 0.0f), float2(1.0f, 1.0f))
    let span = (uv_max - uv_min) * base
    let lod = float(clamp(int(ceil(log2(max(max(span.x, span.y), 1.0f)))), 0, mip_count - 1))
    let o0 = textureLod(hzb_tex, uv_min, lod).x
    let o1 = textureLod(hzb_tex, float2(uv_max.x, uv_min.y), lod).x
    let o2 = textureLod(hzb_tex, float2(uv_min.x, uv_max.y), lod).x
    let o3 = textureLod(hzb_tex, uv_max, lod).x
    let occ = max(max(o0, o1), max(o2, o3))
    // flat decision (nested ternary -> OpSelect; 0 = visible, 1 = frustum-culled, 2 = occlusion-culled).
    // near-cross overrides everything -> keep (0).
    let near_cross = min_w <= 0.0001f
    let frustum = hi.x < -1.0f || lo.x > 1.0f || hi.y < -1.0f || lo.y > 1.0f || near_z > 1.0f
    let occluded = near_z > occ
    let reason = near_cross ? 0 : (frustum ? 1 : (occluded ? 2 : 0))
    cull_reason[i] = uint(reason)
    if (reason == 0) {
        let kk = int(atomicAdd(draw_count[0], 1u))
        draw_cmds[kk].index_count = uint(scene.params.x)
        draw_cmds[kk].instance_count = 1u
        draw_cmds[kk].first_index = 0u
        draw_cmds[kk].vertex_offset = 0
        draw_cmds[kk].first_instance = 0u
        draw_instance[kk] = uint(i)
    }
}

// ===== shared cube geometry + bindless materials =====
// The 36-index unit cube (tutorial 04/05 geometry) plus a UV for texturing. N_MAT distinct textures live
// in a bindless sampler2D[] array; each instance picks one with nonuniformEXT (the index varies per draw).

var @in @location = 0 a_pos    : float3
var @in @location = 1 a_normal : float3
var @in @location = 2 a_uv     : float2

let N_MAT = 6
var @set = 0 @binding = 7 samplers : sampler2D[6]

// ===== main camera view: survivors only, via vkCmdDrawIndexedIndirectCount =====

// gl_DrawID is the compacted draw ordinal [0, drawCount); draw_instance[gl_DrawID] maps it back to the
// original instance the cull kept. The wall in front means the culled instances were hidden anyway, so
// this view looks correct -- that is the whole point of occlusion culling.

var @out @location = 0 v_normal : float3
var @out @location = 1 v_uv     : float2
var @out @flat @location = 2 v_mat : int

[vulkan_vertex_shader(name="main_vert_spv")]
def main_vs {
    let idx = int(draw_instance[gl_DrawID])
    let inst = instances[idx]
    let world = a_pos * inst.pos_scale.w + inst.pos_scale.xyz
    gl_Position = scene.view_proj * float4(world, 1.0f)
    v_normal = a_normal
    v_uv = a_uv
    v_mat = inst.mat_idx
}

var @in @location = 0 f_normal : float3
var @in @location = 1 f_uv     : float2
var @in @flat @location = 2 f_mat : int
var @out @location = 0 main_color : float4

[vulkan_fragment_shader(name="main_frag_spv")]
def main_fs {
    let n = normalize(f_normal)
    let l = normalize(float3(0.4f, 1.0f, 0.5f))
    let lambert = max(dot(n, l), 0.0f) * 0.7f + 0.3f
    let tex = texture(samplers[nonuniformEXT(f_mat)], f_uv)
    main_color = float4(tex.xyz * lambert, 1.0f)
}

// ===== god view: ALL N instances, the reveal =====

// A regular instanced draw (cmd_draw_indexed, instanceCount = N) from an offset camera that sees the
// occluder side-on. gl_InstanceIndex IS the instance id, so cull_reason[gl_InstanceIndex] colours each
// cube by the GPU's verdict: full material if visible, red ghost if occlusion-, blue if frustum-culled.

var @out @location = 0 g_normal : float3
var @out @location = 1 g_uv     : float2
var @out @flat @location = 2 g_mat    : int
var @out @flat @location = 3 g_reason : int

[vulkan_vertex_shader(name="god_vert_spv")]
def god_vs {
    let idx = gl_InstanceIndex
    let inst = instances[idx]
    let world = a_pos * inst.pos_scale.w + inst.pos_scale.xyz
    gl_Position = scene.god_view_proj * float4(world, 1.0f)
    g_normal = a_normal
    g_uv = a_uv
    g_mat = inst.mat_idx
    g_reason = int(cull_reason[idx])
}

var @in @location = 0 gf_normal : float3
var @in @location = 1 gf_uv     : float2
var @in @flat @location = 2 gf_mat    : int
var @in @flat @location = 3 gf_reason : int
var @out @location = 0 god_color : float4

[vulkan_fragment_shader(name="god_frag_spv")]
def god_fs {
    let n = normalize(gf_normal)
    let l = normalize(float3(0.4f, 1.0f, 0.5f))
    let lambert = max(dot(n, l), 0.0f) * 0.7f + 0.3f
    let tex = texture(samplers[nonuniformEXT(gf_mat)], gf_uv)
    let lit = tex.xyz * lambert
    let red = float3(0.9f, 0.15f, 0.1f)         // occlusion-culled (reason 2)
    let blue = float3(0.15f, 0.3f, 0.9f)        // frustum-culled (reason 1)
    let ghost = gf_reason == 2 ? red : blue
    let culled = gf_reason != 0
    let rgb = culled ? ghost : lit
    let a = culled ? 0.7f : 1.0f                 // translucent ghosts read over the wall + survivors
    god_color = float4(rgb, a)
}

// ===== occluder wall =====

// One opaque slab, rendered three times: into the depth pre-pass (so it populates the HZB), opaque into
// the main view, and translucent into the god view. The host bakes the wall's animated position into the
// mvp push constant (a lone matrix -> no trailing-scalar push-constant codegen pitfall).

struct OccPush {
    mvp : float4x4
}
var @push_constant occ_pc : OccPush

[vulkan_vertex_shader(name="occluder_vert_spv")]
def occluder_vs {
    gl_Position = occ_pc.mvp * float4(a_pos, 1.0f)
}

var @out @location = 0 occ_solid : float4
[vulkan_fragment_shader(name="occluder_solid_frag_spv")]
def occluder_solid_fs {
    occ_solid = float4(0.45f, 0.45f, 0.5f, 1.0f)        // opaque gray (main view)
}

var @out @location = 0 occ_ghost : float4
[vulkan_fragment_shader(name="occluder_ghost_frag_spv")]
def occluder_ghost_fs {
    occ_ghost = float4(0.5f, 0.5f, 0.55f, 0.16f)        // faint (god view: see the ghosts behind it)
}
