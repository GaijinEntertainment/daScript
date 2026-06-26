options gen2
options indenting = 4

// Minimal lit cube shaders, authored in daslang and lowered to SPIR-V by dasSpirv. The whole
// transform travels in one push constant: `mvp` places the vertex in clip space, `model` rotates
// the normal into world space for a single-light lambert term. No UBO, no texture, no descriptor
// sets -- the cube is a self-contained push-constant-only pipeline so the example stays focused on
// the dasImgui-over-Vulkan integration rather than cube plumbing.

module cube_shaders public

require vulkan/vulkan_boost public
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

//! Per-frame transforms (128 bytes, vertex-only). `mvp` = proj*view*model; `model` carries the
//! cube's world rotation so the vertex shader can rotate the normal without an inverse-transpose
//! (rotation-only model => a vec4(normal,0) multiply is correct).
struct CubePC {
    mvp   : float4x4
    model : float4x4
}
var @push_constant cpc : CubePC

var @in @location = 0 c_pos    : float3
var @in @location = 1 c_normal : float3
var @out @location = 0 c_world_normal : float3

[vulkan_vertex_shader(name="cube_vert_spv")]
def cube_vs {
    gl_Position = cpc.mvp * float4(c_pos, 1.0)
    c_world_normal = (cpc.model * float4(c_normal, 0.0)).xyz
}

var @in @location = 0 cf_normal : float3
var @out @location = 0 cf_color : float4

[vulkan_fragment_shader(name="cube_frag_spv")]
def cube_fs {
    let n = normalize(cf_normal)
    let l = normalize(float3(0.4, 0.8, 0.5))       // warm key light, above-front
    let key = max(dot(n, l), 0.0) * 0.8 + 0.2      // ambient floor 0.2
    let base = float3(1.0, 0.45, 0.12)             // warm orange
    cf_color = float4(base * key, 1.0)
}
