options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module gltf_pbr shared

require gltf/gltf_types
require gltf/gltf_scene
require gltf/gltf_motion public
require gltf/gltf_shadows public
require gltf/gltf_pbr_common public
require gltf/gltf_gl public
require opengl/opengl_boost public
require glsl/glsl_opengl public
require daslib/safe_addr
require math
require gltf/gltf_atmosphere public
require daslib/math_boost

// Max joints in the palette uniform. Desktop GL guarantees >= 1024 vec4 of vertex uniforms; 64 mat4 is
// 256 vec4, leaving ample room for the rest. Skins with more joints are clamped (rare for game assets).
let GLTF_MAX_JOINTS = 64

// OPT-IN metallic-roughness PBR renderer for a GltfGlModel. The shader is authored once in the
// portable shader DSL (Cook-Torrance GGX) and lowered to GLSL by the [*_program] macros — the same
// body ports to Vulkan by swapping the annotation name. The shader globals below are module-private:
// the app never touches them, it drives everything through gltf_pbr_create / gltf_pbr_render.

// ===== vertex stage =====
// @location order is the contract with gltf_gl's GltfGlVertex (0..7); we consume 0..3 + 6..7 (skinning).
var @in @location = 0 a_position : float3
var @in @location = 1 a_normal   : float3
var @in @location = 2 a_tangent  : float4
var @in @location = 3 a_uv0      : float2
var @in @location = 6 a_joints0  : float4   // joint indices (decoded to float at parse time)
var @in @location = 7 a_weights0 : float4   // per-joint blend weights (sum to 1)

var @uniform u_model : float4x4
var @uniform u_viewProj : float4x4
var @uniform u_skinned  : float                    // 1 = blend the joint palette, 0 = rigid (use u_model)
var @uniform u_joints   : float4x4[GLTF_MAX_JOINTS] // world-space skinning palette (joint.world * inverseBind)
var @uniform u_normal_matrix : float3x3            // inverse-transpose of u_model's 3x3 (rigid-path normals)

var @inout v_world_pos : float3
var @inout v_world_normal : float3
var @inout v_world_tangent : float3
var @inout v_tangent_w : float
var @inout v_uv0 : float2
var @uniform u_previous_model : float4x4
var @uniform u_previous_vp : float4x4
var @uniform u_previous_joints : float4x4[GLTF_MAX_JOINTS]
var @uniform u_motion_jitter : float2
var @uniform u_motion_kind : int
var @inout v_motion_current : float4
var @inout v_motion_previous : float4
def private gltf_pbr_vertex {
    var world = float4(0.0, 0.0, 0.0, 1.0)
    var wnormal = float3(0.0, 0.0, 0.0)
    var wtangent = float3(0.0, 0.0, 0.0)
    if (u_skinned > 0.5) {
        // The palette already maps into world space (joint.world * inverseBind), so u_model is not
        // applied. clamp to the palette size: a skin with > GLTF_MAX_JOINTS joints (or malformed
        // JOINTS_0) would otherwise index u_joints out of range -> undefined shader behavior.
        let weights = float4(1.0 - a_weights0.y - a_weights0.z - a_weights0.w, a_weights0.yzw)
        let p = float4(a_position, 1.0); let n = float4(a_normal, 0.0); let t = float4(a_tangent.xyz, 0.0)
        let jx = clamp(int(a_joints0.x), 0, GLTF_MAX_JOINTS - 1)
        world = (u_joints[jx] * p) * weights.x
        wnormal = (u_joints[jx] * n).xyz * weights.x
        wtangent = (u_joints[jx] * t).xyz * weights.x
        if (weights.y > 0.0) {
            let jy = clamp(int(a_joints0.y), 0, GLTF_MAX_JOINTS - 1)
            world += (u_joints[jy] * p) * weights.y
            wnormal += (u_joints[jy] * n).xyz * weights.y
            wtangent += (u_joints[jy] * t).xyz * weights.y
            if (weights.z > 0.0) {
                let jz = clamp(int(a_joints0.z), 0, GLTF_MAX_JOINTS - 1)
                world += (u_joints[jz] * p) * weights.z
                wnormal += (u_joints[jz] * n).xyz * weights.z
                wtangent += (u_joints[jz] * t).xyz * weights.z
                if (weights.w > 0.0) {
                    let jw = clamp(int(a_joints0.w), 0, GLTF_MAX_JOINTS - 1)
                    world += (u_joints[jw] * p) * weights.w
                    wnormal += (u_joints[jw] * n).xyz * weights.w
                    wtangent += (u_joints[jw] * t).xyz * weights.w
                }
            }
        }
    } else {
        world = u_model * float4(a_position, 1.0)
        // u_normal_matrix is inverse-transpose(u_model 3x3), so normals stay correct under
        // non-uniform scale / shear (float3x3(u_model) would skew them).
        wnormal = u_normal_matrix * a_normal
        wtangent = u_normal_matrix * a_tangent.xyz
    }
    v_world_pos = world.xyz
    v_world_normal = normalize(wnormal)
    v_world_tangent = wtangent
    v_tangent_w = a_tangent.w
    v_uv0 = a_uv0
    gl_Position = u_viewProj * world
}
[vertex_program]
def gltf_pbr_vs {gltf_pbr_vertex()}
[vertex_program]
def gltf_pbr_motion_vs {
    gltf_pbr_vertex()
    v_motion_current = gl_Position; v_motion_current.xy -= u_motion_jitter * gl_Position.w
    var old_world = u_previous_model * float4(a_position, 1.0)
    if (u_skinned > .5 && u_motion_kind == 1){
        let w = float4(1.0 - a_weights0.y - a_weights0.z - a_weights0.w, a_weights0.yzw)
        let p = float4(a_position, 1.0)
        old_world = u_previous_joints[clamp(int(a_joints0.x), 0, GLTF_MAX_JOINTS - 1)] * p * w.x
        if (w.y > 0.0){old_world += u_previous_joints[clamp(int(a_joints0.y), 0, GLTF_MAX_JOINTS - 1)] * p * w.y
            if (w.z > 0.0){old_world += u_previous_joints[clamp(int(a_joints0.z), 0, GLTF_MAX_JOINTS - 1)] * p * w.z
                if (w.w > 0.0){old_world += u_previous_joints[clamp(int(a_joints0.w), 0, GLTF_MAX_JOINTS - 1)] * p * w.w}
            }
        }
    }
    v_motion_previous = u_previous_vp * old_world
}

// ===== fragment stage =====
var @uniform u_fog_color : float3
var @uniform u_fog_params : float4
var @uniform u_fog_falloff : float
var @uniform u_background_tint : float4
var @uniform u_cam_pos : float3
var @uniform u_light_dir   : float3   // direction TOWARD the light, normalized
var @uniform u_light_color : float3   // radiance (color * intensity)
var @uniform u_ambient     : float3   // constant ambient irradiance

var @uniform u_base_color_factor : float4
var @uniform u_metallic_factor : float
var @uniform u_material_scale : float2
var @uniform u_material_wetness : float
var @uniform u_normal_roughness : float
var @uniform u_reflection_amount : float
var @uniform u_roughness_factor : float
var @uniform u_emissive_factor : float3
var @uniform u_normal_scale : float
var @uniform u_occlusion_strength : float
var @uniform u_alpha_cutoff : float
var @uniform u_alpha_mode         : float   // 0 OPAQUE, 1 MASK, 2 BLEND

// base-color / emissive are sRGB textures (hardware decode -> linear); the rest are linear.
var @uniform @stage = 0 u_base_color_tex  : sampler2D
var @uniform @stage = 1 u_metal_rough_tex : sampler2D
var @uniform @stage = 2 u_normal_tex      : sampler2D
var @uniform @stage = 3 u_occlusion_tex   : sampler2D
var @uniform @stage = 4 u_emissive_tex    : sampler2D
// equirect HDR environment (linear radiance, mipmapped); u_env_amount = 0 -> flat ambient
var @uniform @stage = 5 u_env_tex         : sampler2D
var @uniform u_env_amount : float
var @uniform u_env_max_lod : float
var @out @location = 0 o_color : float4
var @out @location = 1 o_surface_material : float4

[fragment_program]
def gltf_pbr_depth_fs {
    if (u_alpha_mode > 0.5 && u_alpha_mode < 1.5 &&
        texture(u_base_color_tex, v_uv0).w * u_base_color_factor.w < u_alpha_cutoff) { discard() }
}


// The BRDF / normal-perturbation / skinning math lives in gltf/gltf_pbr_common — shared with the
// dasVulkan renderer; this module only samples textures and feeds the values in.
def private shade_normal(uv : float2) : float3 {
    return gltf_perturb_normal(normalize(v_world_normal), v_world_tangent, v_tangent_w,
        texture(u_normal_tex, uv).xyz, u_normal_scale)
}

var @uniform @stage = 8 u_ssao_tex : sampler2D
var @uniform u_ssao_enabled : float
var @uniform u_ssao_inverse_size : float2
def gltf_surface_roughness(mr : float4) : float {
    return clamp(lerp(mr.y * u_roughness_factor * u_material_scale.x, .16, clamp(u_material_wetness * mr.x, 0.0, 1.0)), .04, 1.0)
}
[fragment_program]
def gltf_pbr_normal_fs {
    let base = texture(u_base_color_tex, v_uv0) * u_base_color_factor
    if (u_alpha_mode > 0.5 && u_alpha_mode < 1.5 && base.w < u_alpha_cutoff){discard()}
    let roughness = gltf_surface_roughness(texture(u_metal_rough_tex, v_uv0))
    o_color = float4(shade_normal(v_uv0) * 0.5 + float3(0.5), u_normal_roughness > .5 ? roughness : 1.0)
}

//! Combined opaque surface outputs: normals/roughness and albedo/metallic.
[fragment_program]
def gltf_pbr_surface_fs {
    let base = texture(u_base_color_tex, v_uv0) * u_base_color_factor
    if (u_alpha_mode > .5 && u_alpha_mode < 1.5 && base.w < u_alpha_cutoff){discard()}
    let mr = texture(u_metal_rough_tex, v_uv0)
    o_color = float4(shade_normal(v_uv0) * .5 + float3(.5), gltf_surface_roughness(mr))
    o_surface_material = float4(base.xyz, clamp(mr.z * u_metallic_factor * u_material_scale.y, 0.0, 1.0))
}
var @out @location = 2 o_surface_motion : float2
//! Surface MRT with previous-minus-current, unjittered UV motion (RG16F).
[fragment_program]
def gltf_pbr_surface_motion_fs {
    let base = texture(u_base_color_tex, v_uv0) * u_base_color_factor
    if (u_alpha_mode > .5 && u_alpha_mode < 1.5 && base.w < u_alpha_cutoff){discard()}
    let mr = texture(u_metal_rough_tex, v_uv0)
    o_color = float4(shade_normal(v_uv0) * .5 + float3(.5), u_normal_roughness > .5 ? gltf_surface_roughness(mr) : 1.0)
    o_surface_material = float4(base.xyz, clamp(mr.z * u_metallic_factor * u_material_scale.y, 0.0, 1.0))
    o_surface_motion = u_motion_kind == 0 ? float2(4.0) : (u_motion_kind == 1 && v_motion_previous.w > 0.0 ? (v_motion_previous.xy / v_motion_previous.w - v_motion_current.xy / v_motion_current.w) * .5 : float2(2.0))
}
[fragment_program]
def gltf_pbr_material_fs {
    let base = texture(u_base_color_tex, v_uv0) * u_base_color_factor
    if (u_alpha_mode > .5 && u_alpha_mode < 1.5 && base.w < u_alpha_cutoff){discard()}
    o_color = float4(base.xyz, clamp(texture(u_metal_rough_tex, v_uv0).z * u_metallic_factor * u_material_scale.y, 0.0, 1.0))
}
[fragment_program]
def gltf_pbr_fs {
    let uv = v_uv0
    let base = texture(u_base_color_tex, uv) * u_base_color_factor
    // MASK: hard cutoff; BLEND leaves alpha for host-side blending; OPAQUE forces alpha 1.
    if (u_alpha_mode > 0.5 && u_alpha_mode < 1.5 && base.w < u_alpha_cutoff) {
        discard()
    }
    let mr = texture(u_metal_rough_tex, uv)
    let metallic = clamp(mr.z * u_metallic_factor * u_material_scale.y, 0.0, 1.0)
    let roughness = gltf_surface_roughness(mr)
    let occ = texture(u_occlusion_tex, uv).x
    let emissive = texture(u_emissive_tex, uv).xyz * u_emissive_factor

    let albedo = base.xyz
    let n = shade_normal(uv)
    let v = normalize(u_cam_pos - v_world_pos)
    let l = normalize(u_light_dir)
    let direct = gltf_pbr_direct(albedo, metallic, roughness, n, v, l, u_light_color)
    var ambient = gltf_pbr_ambient(albedo, u_ambient, occ, u_occlusion_strength)
    if (u_env_amount > 0.0) {
        // image-based ambient: irradiance from a deep mip along n, reflection from a
        // roughness-scaled mip along the mirror direction (mips as prefiltered environment);
        // clamp so a tiny environment (max lod < 2) can't produce a negative LOD
        let irr = textureLod(u_env_tex, gltf_equirect_uv(n), max(u_env_max_lod - 2.0, 0.0)).xyz
        let refl = textureLod(u_env_tex, gltf_equirect_uv(gltf_reflect_dir(n, v)), roughness * u_env_max_lod).xyz
        ambient = gltf_env_ambient(albedo, metallic, roughness, n, v, irr, refl,
            gltf_occlusion_factor(occ, u_occlusion_strength)) * u_env_amount
    }

    if (u_reflection_amount > 0.0){
        let reflected = textureLod(u_env_tex, gltf_equirect_uv(gltf_reflect_dir(n, v)), roughness * u_env_max_lod).xyz
        let f0 = lerp(float3(.04), albedo, float3(metallic))
        ambient += reflected * gltf_fresnel_roughness(max(dot(n, v), 0.0), f0, roughness) * u_reflection_amount * gltf_occlusion_factor(occ, u_occlusion_strength)
    }
    var local_light = float3(0.0)
    for (i in range(GLTF_MAX_POINT_LIGHTS)) {
        if (i < u_point_count) {
            let delta = u_point_position[i].xyz - v_world_pos
            let distance = max(length(delta), 0.001)
            let visibility = gltf_point_shadow_visibility(i, v_world_pos, n)
            if (distance < u_point_position[i].w){
            let falloff = max(0.0, 1.0 - distance / max(u_point_position[i].w, 0.001))
            let radiance = u_point_color[i].xyz * (falloff * falloff / (1.0 + distance * distance)) * visibility
            local_light += gltf_pbr_direct(albedo, metallic, roughness, n, v, delta / distance, radiance)
            }
        }
    }
    if (u_ssao_enabled > 0.5){ambient *= texture(u_ssao_tex, gl_FragCoord.xy * u_ssao_inverse_size).x}
    let linear = ambient + direct * gltf_shadow_visibility(v_world_pos, n, u_light_dir) + local_light + emissive
    // no sRGB default framebuffer -> encode linear -> sRGB (approximate 2.2 gamma)
    var out_rgb = gltf_srgb_encode(linear)
    let grey = dot(out_rgb, float3(0.299, 0.587, 0.114))
    out_rgb = lerp(out_rgb, float3(grey) * u_background_tint.xyz, u_background_tint.w)
    let transmission = atmosphere_transmittance(u_cam_pos, v_world_pos, u_fog_params, max(u_fog_falloff, 0.01))
    out_rgb = lerp(u_fog_color, out_rgb, transmission)
    let out_a = u_alpha_mode > 1.5 ? base.w : 1.0
    o_color = float4(out_rgb, out_a)
}

var @uniform u_silhouette_color : float4
[fragment_program]
def gltf_pbr_silhouette_fs {
    let base = texture(u_base_color_tex, v_uv0) * u_base_color_factor
    if (u_alpha_mode > 0.5 && base.w < u_alpha_cutoff){discard()}
    o_color = u_silhouette_color
}

// ===== host renderer =====

[safe_when_uninitialized]
struct GltfPbrRenderer {
    shadowFilter : int
    shadowComparison : GltfShadowComparison
    materialWetness : float
    materialScale : float2 = float2(1.0)
    reflectionAmount : float
    materialProgram, surfaceProgram : uint
    atmosphere : Atmosphere
    backgroundTint : float4
    program   : uint
    envTex    : uint     // mipmapped equirect HDR environment, 0 = none (flat ambient)
    envMaxLod : float
    silhouetteProgram : uint
    motionProgram : uint
    normalProgram : uint
    aoTex : uint
    aoInverseSize : float2
    depthProgram : uint
    shadowTex : uint
    shadowViewProj : float4x4
    shadowTexel : float2
    shadowBias : float
    shadowStrength : float
    localShadowTex : uint
    localShadowSize : float2
    pointShadow : float4[GLTF_MAX_POINT_LIGHTS]
    pointCount : int
    pointPosition : float4[GLTF_MAX_POINT_LIGHTS]
    pointColor : float4[GLTF_MAX_POINT_LIGHTS]
}

struct GltfPbrPointLight {
    castsShadow : bool
    position : float3
    color : float3
    radius : float
}

//! Small local-light set, useful for torches/spells. Colors are linear radiance.
//! Unshadowed unless the caller supplies the optional per-light depth atlas settings.
//! Passing an empty array clears the set. Excess lights are ignored after the first eight.
def gltf_pbr_set_point_lights(var r : GltfPbrRenderer; lights : array<GltfPbrPointLight>) {
    r.pointCount = min(length(lights), GLTF_MAX_POINT_LIGHTS)
    for (i in range(r.pointCount)) {
        r.pointPosition[i] = float4(lights[i].position, max(lights[i].radius, 0.001))
        r.pointColor[i] = float4(max(lights[i].color, float3(0.0)), 0.0)
    }
}

//! Configure a caller-owned depth texture (compare mode NONE, nearest filtering).
//! Passing texture 0 disables shadows; the renderer never owns the map/FBO.
//! Shadowing affects direct illumination only, preserving ambient and emissive light.
def gltf_pbr_set_shadow(var r : GltfPbrRenderer; tex : uint; light_vp : float4x4;
                        resolution : int; bias : float = 0.0005; strength : float = 1.0) {
    r.shadowTex = tex
    r.shadowViewProj = light_vp
    r.shadowTexel = float2(1.0 / float(max(resolution, 1)))
    r.shadowBias = max(bias, 0.0)
    r.shadowStrength = tex != 0u ? clamp(strength, 0.0, 1.0) : 0.0
}

//! Attach a mipmapped equirect HDR environment (see gltf_gl_load_hdr_env); pass 0 to remove it.
//! The renderer does not take ownership of the texture. `GltfPbrLight.environment` scales its
//! contribution per frame.
def gltf_pbr_set_environment(var r : GltfPbrRenderer; tex : uint; max_lod : float) {
    r.envTex = tex
    r.envMaxLod = max_lod
}

//! Compile + link the PBR program. Call with a current GL context; `delete` the renderer to release it.
def gltf_pbr_create : GltfPbrRenderer {
    return GltfPbrRenderer(program = create_shader_program(@@gltf_pbr_vs, @@gltf_pbr_fs), shadowComparison = gltf_shadow_comparison_create())
}

def finalize(var r : GltfPbrRenderer) {
    delete r.shadowComparison
    for (program in [r.program, r.depthProgram, r.silhouetteProgram, r.materialProgram, r.surfaceProgram, r.normalProgram, r.motionProgram]){gl_binding_forget_program(program)}
    if (r.program != 0u) {
        glDeleteProgram(r.program)
    }
    if (r.depthProgram != 0u) { glDeleteProgram(r.depthProgram) }
    if (r.silhouetteProgram != 0u) { glDeleteProgram(r.silhouetteProgram) }
    if (r.motionProgram != 0u){glDeleteProgram(r.motionProgram)}
    if (r.surfaceProgram != 0u){glDeleteProgram(r.surfaceProgram)}
    if (r.materialProgram != 0u){glDeleteProgram(r.materialProgram)}
    if (r.normalProgram != 0u) { glDeleteProgram(r.normalProgram) }
}

def private bind_material(model : GltfGlModel; mat : GltfMaterial) {
    u_base_color_factor = mat.baseColorFactor
    u_metallic_factor = mat.metallicFactor
    u_roughness_factor = mat.roughnessFactor
    u_emissive_factor = mat.emissiveFactor
    u_normal_scale = mat.normalScale
    u_occlusion_strength = mat.occlusionStrength
    u_alpha_cutoff = mat.alphaCutoff
    u_alpha_mode = (mat.alphaMode == GltfAlphaMode.mask ? 1.0
        : (mat.alphaMode == GltfAlphaMode.blend ? 2.0 : 0.0))
    u_base_color_tex := gltf_gl_resolve_tex(model, mat.baseColorTex, model.whiteTex)
    u_metal_rough_tex := gltf_gl_resolve_tex(model, mat.metalRoughTex, model.whiteTex)
    u_normal_tex := gltf_gl_resolve_tex(model, mat.normalTex, model.flatNormalTex)
    u_occlusion_tex := gltf_gl_resolve_tex(model, mat.occlusionTex, model.whiteTex)
    u_emissive_tex := gltf_gl_resolve_tex(model, mat.emissiveTex, model.whiteTex)
    gl_material_state_filtered(mat.doubleSided, mat.alphaMode == GltfAlphaMode.blend)
}

// Fill the joint-palette uniform for a skin (clamped to GLTF_MAX_JOINTS), pre-multiplied by the world-
// space `root` transform. The skinned vertex path blends the palette directly (it never reads u_model),
// so baking root here is what makes a viewer turntable transform reach skinned geometry too.
def private bind_skin_palette(scene : GltfScene; skinIndex : int; root : float4x4) {
    let njoint = min(length(scene.skins[skinIndex].joints), GLTF_MAX_JOINTS)
    for (j in range(njoint)) {
        u_joints[j] = root * gltf_joint_matrix(scene, skinIndex, j)
    }
    // clear the unused slots so a clamped / out-of-range joint index reads identity rather than a
    // stale matrix left over from a previously-bound skin
    for (j in range(njoint, GLTF_MAX_JOINTS)) {
        u_joints[j] = identity4x4()
    }
}

def private bind_node(scene : GltfScene; node : GltfNode; root : float4x4; var palette : int&) {
    if (node.skin >= 0 && node.skin < length(scene.skins)) {
        if (!gl_binding_filter_active || palette != node.skin){bind_skin_palette(scene, node.skin, root); palette = node.skin}
        u_skinned = 1.0
        u_model = identity4x4()   // skinned path blends the (root-baked) palette; u_model is unused
        u_normal_matrix = identity3x3()   // unused in the skinned branch
    } else {
        u_skinned = 0.0
        u_model = root * node.world
        u_normal_matrix = float3x3(transpose(inverse(u_model)))
    }
}

//! Render opaque/masked geometry into the caller's bound depth framebuffer.
//! Lazily creates the depth program; caller owns viewport, clear, depth and FBO state.
//! Blended surfaces do not cast solid shadows.
def gltf_pbr_render_depth(var r : GltfPbrRenderer; model : GltfGlModel; scene : GltfScene;
                          root, light_vp : float4x4) {
    if (r.depthProgram == 0u) { r.depthProgram = create_shader_program(@@gltf_pbr_vs, @@gltf_pbr_depth_fs) }
    gl_use_program_filtered(r.depthProgram)
    u_viewProj = light_vp
    var palette = -1
    for (node in scene.nodes) {
        if (node.mesh < 0 || node.mesh >= length(model.meshes)) { continue }
        bind_node(scene, node, root, palette)
        for (glp in model.meshes[node.mesh].primitives) {
            if (glp.material >= 0 && glp.material < length(scene.materials)) {
                let mat = scene.materials[glp.material]
                if (mat.alphaMode == GltfAlphaMode.blend) { continue }
                bind_material(model, mat)
            } else { bind_material(model, GltfMaterial()) }
            gltf_pbr_vs_bind_uniform(r.depthProgram)
            gltf_pbr_depth_fs_bind_uniform(r.depthProgram)
            gltf_gl_draw_primitive(glp)
        }
    }
    gl_release_program()
}

def private bind_previous_pose(scene : GltfScene; node : GltfNode; node_index : int; previous : GltfMotionPose){
    u_previous_model = node_index < length(previous.nodes) ? previous.nodes[node_index] : u_model
    if (node.skin < 0 || node.skin >= length(previous.offsets) || node.skin >= length(scene.skins)){return }
    let count = min(length(scene.skins[node.skin].joints), GLTF_MAX_JOINTS)
    for (j in range(GLTF_MAX_JOINTS)){let index = previous.offsets[node.skin] + j; u_previous_joints[j] = j < count && index < length(previous.joints) ? previous.joints[index] : identity4x4()}
}

def private gltf_surface_program(var r : GltfPbrRenderer; material, surface, motion : bool) : uint {
    if (motion){
        if (r.motionProgram == 0u){r.motionProgram = create_shader_program(@@gltf_pbr_motion_vs, @@gltf_pbr_surface_motion_fs)}
        return r.motionProgram
    }
    if (surface){
        if (r.surfaceProgram == 0u){r.surfaceProgram = create_shader_program(@@gltf_pbr_vs, @@gltf_pbr_surface_fs)}
        return r.surfaceProgram
    }
    if (material){
        if (r.materialProgram == 0u){r.materialProgram = create_shader_program(@@gltf_pbr_vs, @@gltf_pbr_material_fs)}
        return r.materialProgram
    }
    if (r.normalProgram == 0u){r.normalProgram = create_shader_program(@@gltf_pbr_vs, @@gltf_pbr_normal_fs)}
    return r.normalProgram
}

//! Opaque depth/normal prepass using the same skinning, material normals and alpha masks.
def gltf_pbr_render_normals(var r : GltfPbrRenderer; model : GltfGlModel; scene : GltfScene; root, vp : float4x4; pack_roughness : bool = false; material_buffer : bool = false; surface_buffer : bool = false; motion_buffer : bool = false; previous : GltfMotionPose const? = null; previous_vp : float4x4 = identity4x4(); jitter : float2 = float2(0.0); motion_kind : int = 0) {
    let program = gltf_surface_program(r, material_buffer, surface_buffer, motion_buffer)
    gl_use_program_filtered(program); u_viewProj = vp; u_material_scale = r.materialScale; u_material_wetness = r.materialWetness; u_normal_roughness = pack_roughness ? 1.0 : 0.0
    var palette = -1
    if (motion_buffer){u_motion_kind = motion_kind; u_previous_vp = previous_vp; u_motion_jitter = jitter}
    for (node, node_index in scene.nodes, count()){
        if (node.mesh < 0 || node.mesh >= length(model.meshes)){continue}
        bind_node(scene, node, root, palette)
        if (motion_buffer){
            let valid = previous != null && gltf_motion_has_node(*previous, scene, node_index)
            u_motion_kind = motion_kind == 1 && !valid ? 2 : motion_kind
            if (u_motion_kind == 1){bind_previous_pose(scene, node, node_index, *previous)}
        }
        for (glp in model.meshes[node.mesh].primitives){
            if (glp.material >= 0 && glp.material < length(scene.materials)){
                let mat = scene.materials[glp.material]
                if (mat.alphaMode == GltfAlphaMode.blend){continue}
                bind_material(model, mat)
            }else{bind_material(model, GltfMaterial())}
            if (motion_buffer){gltf_pbr_motion_vs_bind_uniform(program); gltf_pbr_surface_motion_fs_bind_uniform(program)}else{gltf_pbr_vs_bind_uniform(program); if (surface_buffer){gltf_pbr_surface_fs_bind_uniform(program)}elif (material_buffer){gltf_pbr_material_fs_bind_uniform(program)}else{gltf_pbr_normal_fs_bind_uniform(program)}}
            gltf_gl_draw_primitive(glp)
        }
    }
    gl_release_program()
}

//! Draw opaque/masked geometry with a flat color and the ordinary skinning path.
//! Caller owns depth test/write state. Draw hidden before visible geometry to avoid self-overlays.
def gltf_pbr_render_silhouette(var r : GltfPbrRenderer; model : GltfGlModel; scene : GltfScene;
                               root, vp : float4x4; color : float4) {
    if (r.silhouetteProgram == 0u){r.silhouetteProgram = create_shader_program(@@gltf_pbr_vs, @@gltf_pbr_silhouette_fs)}
    gl_use_program_filtered(r.silhouetteProgram); u_viewProj = vp; u_silhouette_color = color
    var palette = -1
    for (node in scene.nodes){
        if (node.mesh < 0 || node.mesh >= length(model.meshes)){continue}
        bind_node(scene, node, root, palette)
        for (glp in model.meshes[node.mesh].primitives){
            if (glp.material >= 0 && glp.material < length(scene.materials)){
                let mat = scene.materials[glp.material]
                if (mat.alphaMode == GltfAlphaMode.blend){continue}
                bind_material(model, mat)
            }else{bind_material(model, GltfMaterial())}
            gltf_pbr_vs_bind_uniform(r.silhouetteProgram); gltf_pbr_silhouette_fs_bind_uniform(r.silhouetteProgram)
            gltf_gl_draw_primitive(glp)
        }
    }
    gl_release_program()
}

//! Render every mesh instance in `scene` with the PBR shader, using the GL objects in `model`. A node
//! with a `skin` is drawn skinned (its joint palette is uploaded and the mesh-node transform ignored);
//! otherwise the node's precomputed `world` matrix is the model transform. Per primitive the material
//! factors + textures are bound and doubleSided/alpha GL state is set. Caller owns clear + depth state.
def gltf_pbr_render(r : GltfPbrRenderer; model : GltfGlModel; scene : GltfScene;
                    view, proj : float4x4; camPos : float3; light : GltfPbrLight) {
    gltf_pbr_render(r, model, scene, identity4x4(), view, proj, camPos, light)
}

//! As above, but pre-multiplies a world-space `root` transform onto the whole scene (a viewer turntable
//! rotation/scale, say). Rigid nodes render at `root * node.world`; for skinned nodes `root` is baked into
//! the joint palette, so it transforms rigid and skinned geometry alike — and lighting responds correctly
//! (root reaches the world-space normals). The plain overload passes `identity4x4()`, so it is unchanged.
def gltf_pbr_render(r : GltfPbrRenderer; model : GltfGlModel; scene : GltfScene; root : float4x4;
                    view, proj : float4x4; camPos : float3; light : GltfPbrLight) {
    gltf_shadow_comparison_bind(r.shadowComparison, r.shadowTex, r.localShadowTex); u_shadow_filter = r.shadowFilter
    gl_use_program_filtered(r.program)
    u_viewProj = proj * view
    u_cam_pos = camPos
    u_light_dir = normalize(light.direction)
    u_light_color = light.color * light.intensity
    u_ambient = light.ambient
    u_env_tex := (r.envTex != 0u ? r.envTex : model.whiteTex)
    u_env_amount = r.envTex != 0u ? light.environment : 0.0
    u_material_scale = r.materialScale; u_material_wetness = r.materialWetness; u_reflection_amount = r.envTex != 0u ? r.reflectionAmount : 0.0
    u_env_max_lod = r.envMaxLod
    u_shadow_tex := r.shadowTex != 0u ? r.shadowTex : model.whiteTex
    u_shadow_vp = r.shadowViewProj
    u_shadow_texel = r.shadowTexel
    u_shadow_bias = r.shadowBias
    u_shadow_strength = r.shadowStrength
    u_ssao_tex := r.aoTex != 0u ? r.aoTex : model.whiteTex
    u_ssao_enabled = r.aoTex != 0u ? 1.0 : 0.0
    u_ssao_inverse_size = r.aoInverseSize
    u_fog_color = r.atmosphere.color
    u_fog_params = float4(r.atmosphere.density, r.atmosphere.start, r.atmosphere.height_density, r.atmosphere.height_top)
    u_fog_falloff = r.atmosphere.height_falloff
    u_background_tint = r.backgroundTint
    u_local_shadow_tex := r.localShadowTex != 0u ? r.localShadowTex : model.whiteTex
    u_local_shadow_size = r.localShadowSize
    u_point_count = r.pointCount
    for (i in range(GLTF_MAX_POINT_LIGHTS)) {
        u_point_shadow[i] = r.pointShadow[i]
        u_point_position[i] = r.pointPosition[i]
        u_point_color[i] = r.pointColor[i]
    }
    var palette = -1
    for (node in scene.nodes) {
        if (node.mesh < 0 || node.mesh >= length(model.meshes)) {
            continue
        }
        bind_node(scene, node, root, palette)
        for (glp in model.meshes[node.mesh].primitives) {
            if (glp.material >= 0 && glp.material < length(scene.materials)) {
                bind_material(model, scene.materials[glp.material])
            } else {
                bind_material(model, GltfMaterial())
            }
            gltf_pbr_vs_bind_uniform(r.program)
            gltf_pbr_fs_bind_uniform(r.program)
            gltf_gl_draw_primitive(glp)
        }
    }
    gl_release_program(); gltf_shadow_comparison_unbind()
}
