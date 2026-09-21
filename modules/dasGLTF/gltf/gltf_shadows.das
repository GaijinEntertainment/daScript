options gen2
module gltf_shadows shared public
require math
require opengl/opengl_boost public
require opengl/gl_binding_cache
require gltf_pbr_common

//! Regular depth sampling permits a color fallback while shadows are disabled.
var @uniform @stage = 6 u_shadow_tex : sampler2D
var @uniform u_shadow_vp : float4x4
var @uniform u_shadow_texel : float2
var @uniform u_shadow_bias : float
var @uniform u_shadow_strength : float
let GLTF_MAX_POINT_LIGHTS = 8
var @uniform @stage = 9 u_local_shadow_tex : sampler2D
var @uniform u_local_shadow_size : float2
var @uniform u_point_shadow : float4[GLTF_MAX_POINT_LIGHTS]
var @uniform u_point_count : int
var @uniform u_point_position : float4[GLTF_MAX_POINT_LIGHTS]
var @uniform u_point_color : float4[GLTF_MAX_POINT_LIGHTS]

var @uniform @stage = 10 u_shadow_compare : sampler2DShadow
var @uniform @stage = 11 u_local_shadow_compare : sampler2DShadow
var @uniform u_shadow_filter : int

def private gltf_shadow_compare(p : float3; uv, gradient, texel, tile, atlas_size : float2; bias : float; local : bool) : float {
    let sample_uv = clamp(uv, texel * .5, float2(1.0) - texel * .5)
    let depth = gltf_shadow_receiver_depth(p, sample_uv, gradient, bias)
    if (local){return textureCompare(u_local_shadow_compare, (sample_uv + tile) / atlas_size, depth)}
    return textureCompare(u_shadow_compare, sample_uv, depth)
}

def private gltf_shadow_edge_filter(p : float3; gradient : float2; bias : float; texel, tile, atlas_size : float2; local : bool) : float {
    let half_texel = texel * .5
    let nw = gltf_shadow_compare(p, p.xy - half_texel, gradient, texel, tile, atlas_size, bias, local)
    let sw = gltf_shadow_compare(p, p.xy + float2(-half_texel.x, half_texel.y), gradient, texel, tile, atlas_size, bias, local)
    let ne = gltf_shadow_compare(p, p.xy + float2(half_texel.x, -half_texel.y), gradient, texel, tile, atlas_size, bias, local)
    let se = gltf_shadow_compare(p, p.xy + half_texel, gradient, texel, tile, atlas_size, bias, local)
    let center = gltf_shadow_compare(p, p.xy, gradient, texel, tile, atlas_size, bias, local)
    let direction = float2(sw - ne + se - nw, sw - ne - se + nw) * texel
    let negative = gltf_shadow_compare(p, p.xy - direction, gradient, texel, tile, atlas_size, bias, local)
    let positive = gltf_shadow_compare(p, p.xy + direction, gradient, texel, tile, atlas_size, bias, local)
    return gltf_shadow_edge_blend(center, negative, positive)
}

struct GltfShadowComparison {
    //! Renderer-owned comparison sampler and lit depth fallback; delete before destroying the GL context.
    sampler, fallback : uint
}
def gltf_shadow_comparison_create() : GltfShadowComparison {
    //! Create comparison resources without changing the active texture unit or its binding.
    var result = GltfShadowComparison()
    glGenSamplers(1, unsafe(addr(result.sampler)))
    glSamplerParameteri(result.sampler, GL_TEXTURE_COMPARE_MODE, int(GL_COMPARE_REF_TO_TEXTURE)); glSamplerParameteri(result.sampler, GL_TEXTURE_COMPARE_FUNC, int(GL_LEQUAL))
    glSamplerParameteri(result.sampler, GL_TEXTURE_MIN_FILTER, int(GL_LINEAR)); glSamplerParameteri(result.sampler, GL_TEXTURE_MAG_FILTER, int(GL_LINEAR))
    glSamplerParameteri(result.sampler, GL_TEXTURE_WRAP_S, int(GL_CLAMP_TO_EDGE)); glSamplerParameteri(result.sampler, GL_TEXTURE_WRAP_T, int(GL_CLAMP_TO_EDGE))
    var previous_binding : int
    glGetIntegerv(GL_TEXTURE_BINDING_2D, unsafe(addr(previous_binding)))
    glGenTextures(1, unsafe(addr(result.fallback))); glBindTexture(GL_TEXTURE_2D, result.fallback)
    let depth = 0xffffffffu
    glTexImage2D(GL_TEXTURE_2D, 0, int(GL_DEPTH_COMPONENT24), 1, 1, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, unsafe(addr(depth)))
    glBindTexture(GL_TEXTURE_2D, uint(previous_binding))
    return result
}
def finalize(var comparison : GltfShadowComparison){
    if (comparison.sampler != 0u){glDeleteSamplers(1, unsafe(addr(comparison.sampler)))}
    if (comparison.fallback != 0u){gl_binding_invalidate(); glDeleteTextures(1, unsafe(addr(comparison.fallback)))}
}
def gltf_shadow_comparison_bind(comparison : GltfShadowComparison; directional, local : uint){
    //! Bind comparison samplers on units 10 and 11; zero texture handles use the lit fallback.
    u_shadow_compare := directional != 0u ? directional : comparison.fallback
    u_local_shadow_compare := local != 0u ? local : comparison.fallback
    glBindSampler(10u, comparison.sampler); glBindSampler(11u, comparison.sampler)
}
def gltf_shadow_comparison_unbind(){
    //! Release the comparison sampler bindings on units 10 and 11.
    glBindSampler(10u, 0u); glBindSampler(11u, 0u)}

[arch(at="../ARCHITECTURE.md#directional-shadow-filtering")]
def gltf_shadow_visibility(world : float3; normal, light_direction : float3) : float {
    //! Evaluate directional shadow visibility using the configured filter and receiver correction.
    if (u_shadow_strength <= 0.0) { return 1.0 }
    let light_pos = u_shadow_vp * float4(world, 1.0)
    let p = light_pos.xyz / light_pos.w * 0.5 + float3(0.5)
    let gradient = gltf_shadow_receiver_gradient(dFdx(p), dFdy(p))
    if (p.x <= 0.0 || p.x >= 1.0 || p.y <= 0.0 || p.y >= 1.0 || p.z <= 0.0 || p.z >= 1.0) { return 1.0 }
    let bias = u_shadow_bias * (1.0 + 3.0 * (1.0 - max(dot(normal, normalize(light_direction)), 0.0)))
    if (u_shadow_filter == 1){return 1.0 - u_shadow_strength * (1.0 - gltf_shadow_edge_filter(p, gradient, bias, u_shadow_texel, float2(0.0), float2(1.0), false))}
    let texel_pos = p.xy / u_shadow_texel - float2(0.5)
    let base = floor(texel_pos)
    let fraction = texel_pos - base
    let wx = gltf_shadow_pcf_weights(fraction.x)
    let wy = gltf_shadow_pcf_weights(fraction.y)
    var visible = 0.0
    for (y in range(4)) {
        for (x in range(4)) {
            let uv = (base + float2(float(x), float(y)) - float2(0.5)) * u_shadow_texel
            let depth = texture(u_shadow_tex, uv).x
            visible += (gltf_shadow_receiver_depth(p, uv, gradient, bias) <= depth ? 1.0 : 0.0) * wx[x] * wy[y]
        }
    }
    return 1.0 - u_shadow_strength * (1.0 - visible)
}

def gltf_point_shadow_visibility(index : int; world, normal : float3) : float {
    //! Evaluate one point light shadow in its six-face atlas region.
    let settings = u_point_shadow[index]
    if (settings.x < 0.5 || settings.y <= 0.0){return 1.0}
    let delta = world + normal * 0.008 - u_point_position[index].xyz
    let a = abs(delta); var face = 0; var xy = float2(0.0); var distance = a.x
    if (a.x >= a.y && a.x >= a.z){
        face = delta.x >= 0.0 ? 0 : 1; xy = float2(delta.x >= 0.0 ? -delta.z : delta.z, -delta.y)
    }elif (a.y >= a.z){
        face = delta.y >= 0.0 ? 2 : 3; distance = a.y; xy = float2(delta.x, delta.y >= 0.0 ? delta.z : -delta.z)
    }else{
        face = delta.z >= 0.0 ? 4 : 5; distance = a.z; xy = float2(delta.z >= 0.0 ? delta.x : -delta.x, -delta.y)
    }
    let near = settings.z; let far = settings.w
    let safe_distance = max(distance, 0.000001)
    let p = float3(xy / safe_distance * 0.5 + float2(0.5), far / (far - near) - far * near / ((far - near) * safe_distance))
    let gradient = gltf_shadow_receiver_gradient(dFdx(p), dFdy(p))
    if (distance <= near || distance >= far){return 1.0}
    let size = u_local_shadow_size.x; let base = floor(p.xy * size - float2(0.5)); let f = p.xy * size - float2(0.5) - base
    if (u_shadow_filter == 1){return 1.0 - settings.y * (1.0 - gltf_shadow_edge_filter(p, gradient, 0.00015, float2(1.0 / size), float2(float(face), settings.x - 1.0), float2(6.0, u_local_shadow_size.y), true))}
    let wx = gltf_shadow_pcf_weights(f.x); let wy = gltf_shadow_pcf_weights(f.y)
    var visible = 0.0
    for (y in range(4)){for (x in range(4)){
        let uv = clamp((base + float2(float(x), float(y)) - float2(0.5)) / size, float2(0.5 / size), float2(1.0 - 0.5 / size))
        let atlas = (uv + float2(float(face), settings.x - 1.0)) / float2(6.0, u_local_shadow_size.y)
        let depth = texture(u_local_shadow_tex, atlas).x
        visible += (gltf_shadow_receiver_depth(p, uv, gradient, 0.00015) <= depth ? 1.0 : 0.0) * wx[x] * wy[y]
    }}
    return 1.0 - settings.y * (1.0 - visible)
}

