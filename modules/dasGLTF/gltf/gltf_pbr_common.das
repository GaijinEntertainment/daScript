options gen2
options indenting = 4

module gltf_pbr_common shared

require math

// Backend-neutral glTF PBR shader math, shared by the OpenGL renderer (gltf/gltf_pbr) and the
// dasVulkan one (vulkan_gltf). Pure scalar/vector/matrix functions only — no samplers, no shader
// globals — so both shader emitters (dasGlsl -> GLSL, dasSpirv -> SPIR-V) lower them as user
// shader functions; callers sample textures and pass the values in.

def gltf_shadow_pcf_weights(fraction : float) : float4 {
    //! Combined weights of three adjacent bilinear comparison taps. Four nearest
    //! depth comparisons reproduce them without interpolating raw depth values.
    //! The weights shift continuously as the receiver crosses a shadow texel.
    return float4(1.0 - fraction, 1.0, 1.0, fraction) / 3.0
}

def gltf_shadow_receiver_gradient(dx, dy : float3) : float2 {
    //! Depth slope in shadow UV coordinates, independent of the camera-screen basis.
    let determinant = dx.x * dy.y - dx.y * dy.x
    if (abs(determinant) < 0.000000000001) { return float2(0.0) }
    return float2(dy.y * dx.z - dx.y * dy.z, dx.x * dy.z - dy.x * dx.z) / determinant
}

def gltf_shadow_receiver_depth(receiver : float3; uv, gradient : float2; bias : float) : float {
    //! Compare at each PCF texel's position on the receiver plane, not the center depth.
    return receiver.z + dot(gradient, uv - receiver.xy) - bias
}

def gltf_ggx_distribution(n, h : float3; roughness : float) : float {
    //! Trowbridge-Reitz GGX normal distribution.
    let a = roughness * roughness
    let a2 = a * a
    let ndoth = max(dot(n, h), 0.0)
    let d = ndoth * ndoth * (a2 - 1.0) + 1.0
    return a2 / max(3.14159265 * d * d, 0.0000001)
}

def gltf_ggx_geometry_smith(n, v, l : float3; roughness : float) : float {
    //! Smith geometry term with Schlick-GGX, direct-lighting k = (r+1)^2 / 8.
    let r = roughness + 1.0
    let k = r * r * 0.125
    let ndotv = max(dot(n, v), 0.0)
    let ndotl = max(dot(n, l), 0.0)
    let gv = ndotv / (ndotv * (1.0 - k) + k)
    let gl = ndotl / (ndotl * (1.0 - k) + k)
    return gv * gl
}

def gltf_fresnel_schlick(cos_theta : float; f0 : float3) : float3 {
    //! Schlick Fresnel approximation.
    let m = clamp(1.0 - cos_theta, 0.0, 1.0)
    let m5 = m * m * m * m * m
    return f0 + (float3(1.0, 1.0, 1.0) - f0) * m5
}

def gltf_pbr_direct(albedo : float3; metallic, roughness : float; n, v, l, light_color : float3) : float3 {
    //! Cook-Torrance direct-lighting term for one light: outgoing radiance given the surface
    //! normal (n), view (v) and light (l) directions — all normalized — and the light's
    //! radiance (color * intensity).
    let h = normalize(v + l)
    let ndotl = max(dot(n, l), 0.0)
    let ndotv = max(dot(n, v), 0.0)
    let f0 = lerp(float3(0.04, 0.04, 0.04), albedo, float3(metallic, metallic, metallic))
    let d = gltf_ggx_distribution(n, h, roughness)
    let g = gltf_ggx_geometry_smith(n, v, l, roughness)
    let f = gltf_fresnel_schlick(max(dot(h, v), 0.0), f0)
    let spec_denom = 4.0 * ndotv * ndotl + 0.0001
    let specular = f * (d * g * (1.0 / spec_denom))
    let kd = (float3(1.0, 1.0, 1.0) - f) * (1.0 - metallic)
    let diffuse = kd * albedo * (1.0 / 3.14159265)
    return (diffuse + specular) * light_color * ndotl
}

def gltf_occlusion_factor(occ, occlusion_strength : float) : float {
    //! The glTF occlusion strength blend (occlusion darkens indirect light only).
    return 1.0 + occlusion_strength * (occ - 1.0)
}

def gltf_pbr_ambient(albedo, ambient : float3; occ, occlusion_strength : float) : float3 {
    //! Flat ambient irradiance with the glTF occlusion strength blend.
    return ambient * albedo * gltf_occlusion_factor(occ, occlusion_strength)
}

def gltf_equirect_uv(d : float3) : float2 {
    //! Direction -> equirectangular UV for a 2:1 lat-long environment map.
    let u = atan2(d.z, d.x) * 0.15915494 + 0.5
    let v = acos(clamp(d.y, -1.0, 1.0)) * 0.31830989
    return float2(u, v)
}

def gltf_fresnel_roughness(cos_theta : float; f0 : float3; roughness : float) : float3 {
    //! Roughness-aware Schlick Fresnel for environment specular — a rough surface cannot
    //! reach the full grazing reflection.
    let m = clamp(1.0 - cos_theta, 0.0, 1.0)
    let m5 = m * m * m * m * m
    let fr = max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), f0)
    return f0 + (fr - f0) * m5
}

def gltf_reflect_dir(n, v : float3) : float3 {
    //! Mirror reflection of the view direction about the normal (v points TOWARD the camera).
    return n * (2.0 * dot(n, v)) - v
}

def gltf_env_ambient(albedo : float3; metallic, roughness : float; n, v : float3;
                     irradiance, reflection : float3; occ_factor : float) : float3 {
    //! Image-based ambient: `irradiance` is the environment sampled along n (high mip),
    //! `reflection` the environment sampled along the reflection direction (roughness-scaled
    //! mip). Diffuse is energy-balanced against the roughness-aware Fresnel; occlusion darkens
    //! both (indirect light only, per glTF).
    let f0 = lerp(float3(0.04, 0.04, 0.04), albedo, float3(metallic, metallic, metallic))
    let ndotv = max(dot(n, v), 0.0)
    let fres = gltf_fresnel_roughness(ndotv, f0, roughness)
    let kd = (float3(1.0, 1.0, 1.0) - fres) * (1.0 - metallic)
    return (kd * irradiance * albedo + reflection * fres) * occ_factor
}

def gltf_perturb_normal(n_geom, tangent : float3; tangent_w : float; nsample : float3; normal_scale : float) : float3 {
    //! Perturb the normalized geometric normal by a tangent-space normal-map sample (raw 0..1 rgb)
    //! when a tangent frame exists; falls back to the geometric normal otherwise (a flat-normal
    //! default texture also yields no perturbation). `tangent_w` is the glTF handedness sign.
    let tlen = length(tangent)
    var n = n_geom
    if (tlen > 0.0001) {
        let t = tangent * (1.0 / tlen)
        let b = cross(n_geom, t) * tangent_w
        let s = nsample * 2.0 - float3(1.0, 1.0, 1.0)
        let sx = s.x * normal_scale
        let sy = s.y * normal_scale
        n = normalize(t * sx + b * sy + n_geom * s.z)
    }
    return n
}

def gltf_skin_point(m0, m1, m2, m3 : float4x4; w : float4; p : float4) : float4 {
    //! Linear-blend skinning in VECTOR space (shader math has mat*vec / vec*scalar / vec+vec but
    //! no mat+mat / scalar*mat): blends the four transformed points rather than the matrices.
    //! Use p.w = 1 for positions, 0 for normals/tangents.
    return ((m0 * p) * w.x
          + (m1 * p) * w.y
          + (m2 * p) * w.z
          + (m3 * p) * w.w)
}

def gltf_srgb_encode(linear_rgb : float3) : float3 {
    //! Approximate linear -> sRGB (2.2 gamma) encode for output to a non-sRGB framebuffer.
    let g22 = 1.0 / 2.2
    return pow(linear_rgb, float3(g22, g22, g22))
}

//! A single directional light plus a flat ambient term. `direction` points TOWARD the light.
//! Shared host-side light description for the GL and Vulkan renderers. `environment` scales
//! the equirect environment map's image-based ambient when one is set on the renderer
//! (0 disables it, falling back to the flat `ambient`).
struct GltfPbrLight {
    direction   : float3 = float3(0.3, 0.7, 0.5)
    color       : float3 = float3(1.0, 1.0, 1.0)
    intensity   : float  = 3.0
    ambient     : float3 = float3(0.15, 0.15, 0.18)
    environment : float  = 1.0
}

def gltf_shadow_edge_blend(center, negative, positive : float) : float {return center * .5 + (negative + positive) * .25}
