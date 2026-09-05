options gen2
options persistent_heap

require ../river_run/rr_shaders.das public
require ../river_run/rr_postfx.das public
require lp_world public
require lp_lighting public
require lp_atmosphere public
require lp_geology

var @uniform lp_surface : float4
var @uniform lp_object_scale : float3
var @uniform lp_debug_material : int
var @uniform @stage = 5 lp_depth_map : sampler2D
var @inout lp_material_position : float3
var @live render_material_mode = 0
var @live render_bloom = true
var @live render_ao = true
var @live render_time_override = -1.0
var trace_render_errors = false
var render_errors : array<int2>

def trace_render_stage(stage : int) {
    return if (!trace_render_errors)
    for (_i in range(8)) {
        let error = glGetError()
        if (error == 0u) { break }
        render_errors |> push(int2(stage, int(error)))
    }
}
var lamp_shadow_fbo = 0u
var lamp_shadow_tex = 0u
var probe_fbo = 0u
var probe_color = 0u
var probe_depth = 0u
var probe_cube = 0u
var probe_state = -1
var @uniform @stage = 7 lp_environment : samplerCube
var @uniform lp_environment_ready : float = 0.0
let PROBE_SIZE = 128
let PROBE_CENTER = float3(-1.0, 3.0, 2.3)
var atmosphere_program = 0u
var dust_program = 0u
var sparks_program = 0u
var atmosphere_fbo = 0u
var atmosphere_color = 0u
var atmosphere_width = 0
var atmosphere_height = 0

def camera_ray(ndc, scale : float2; forward, right, up : float3) : float3 {
    return forward + right * (ndc.x * scale.x) + up * (ndc.y * scale.y)
}

def transformed_normal(normal, scale : float3; model : float4x4) : float3 {
    return normalize(float3x3(model) * (normal / (scale * scale)))
}

def hdr_pixel_valid(pixel : float4) : bool {
    return abs(pixel.x) <= 65504.0 && abs(pixel.y) <= 65504.0 && abs(pixel.z) <= 65504.0
}

[vertex_program]
def vs_latch_sky {
    let id = gl_VertexIndex
    let uv = float2(float((id << 1) & 2), float(id & 2))
    let ndc = uv * 2.0 - float2(1.0)
    f_ray = camera_ray(ndc, f_CamScale, f_CamFwd, f_CamRight, f_CamUp)
    gl_Position = float4(ndc, 1.0, 1.0)
}

[vertex_program]
def vs_latch_surface {
    let world_position = v_model * float4(v_position, 1.0)
    f_world = world_position.xyz
    f_normal = transformed_normal(v_normal, lp_object_scale, v_model)
    lp_material_position = v_position * lp_object_scale
    let view_position = v_view * world_position
    f_view_pos = view_position.xyz
    gl_Position = v_projection * view_position
}

def lp_hash(p : float3) : float {
    let value = sin(dot(p, float3(127.1, 311.7, 74.7))) * 43758.5453
    return value - floor(value)
}

def lp_noise(p : float3) : float {
    let cell = floor(p)
    let f = p - cell
    let blend = f * f * (float3(3.0) - 2.0 * f)
    let bottom = lerp(lerp(lp_hash(cell), lp_hash(cell + float3(1.0, 0.0, 0.0)), blend.x),
        lerp(lp_hash(cell + float3(0.0, 1.0, 0.0)), lp_hash(cell + float3(1.0, 1.0, 0.0)), blend.x), blend.y)
    let top = lerp(lerp(lp_hash(cell + float3(0.0, 0.0, 1.0)), lp_hash(cell + float3(1.0, 0.0, 1.0)), blend.x),
        lerp(lp_hash(cell + float3(0.0, 1.0, 1.0)), lp_hash(cell + float3(1.0, 1.0, 1.0)), blend.x), blend.y)
    return lerp(bottom, top, blend.z)
}

[fragment_program]
def fs_latch_sky {
    let ray = normalize(f_ray)
    let altitude = max(ray.z, 0.0)
    let horizon = exp(-altitude * 5.0)
    let sunset = pow(saturate(dot(normalize(float3(ray.xy, 0.08)), normalize(float3(-0.8, -0.6, 0.08)))), 6.0)
    var color = lerp(float3(0.018, 0.029, 0.075), float3(0.15, 0.16, 0.23), horizon)
    color += float3(0.19, 0.060, 0.021) * sunset * horizon
    let wind = float3(lp_clock * 0.008, lp_clock * 0.003, 0.0)
    let cloud_position = ray / max(ray.z + 0.12, 0.08) * 1.5 + wind
    let noise = lp_noise(cloud_position) * 0.65 + lp_noise(cloud_position * 2.7) * 0.35
    let cloud = smoothstep(0.34, 0.66, noise) * smoothstep(-0.02, 0.12, ray.z)
    let storm = pow(saturate(dot(ray, normalize(float3(0.25, 0.85, 0.25)))), 14.0)
    color = lerp(color, float3(0.025, 0.035, 0.061) + float3(0.020, 0.016, 0.012) * horizon, cloud * 0.88)
    color += float3(0.30, 0.39, 0.62) * lp_flash * storm * (0.30 + cloud)
    let bolt_height = ray.z / max(ray.y, 0.2)
    let bolt_x = 0.29 + sin(bolt_height * 119.0) * 0.014 + sin(bolt_height * 313.0) * 0.006
    let bolt_distance = abs(ray.x / max(ray.y, 0.2) - bolt_x)
    let bolt_width = max(fwidth(bolt_distance), 0.0010)
    let bolt = ((1.0 - smoothstep(0.0006, 0.0006 + bolt_width, bolt_distance)) *
        smoothstep(0.04, 0.055, bolt_height) * (1.0 - smoothstep(0.22, 0.25, bolt_height)) * step(0.8, ray.y))
    color += float3(1.6, 1.9, 2.6) * bolt * lp_flash
    f_FragColor = float4(color, 0.0)
    f_FragNormalDepth = float4(0.5, 0.5, 1.0, 1.0)
}

def surface_albedo(base : float3; wear : float; mapped, normal : float3) : float3 {
    var color = lerp(base, float3(0.22, 0.065, 0.022), wear)
    if (lp_surface.y < 0.01 && lp_surface.x > 0.8) {
        let strata = sin(mapped.z * 4.0 + lp_noise(mapped * 0.65) * 1.4) * 0.5 + 0.5
        color *= 0.72 + strata * 0.48
        color = lerp(color, float3(0.25, 0.21, 0.15), smoothstep(0.25, 0.85, normal.z) * 0.35)
    }
    return color
}

def plate_height(p : float2) : float {
    let diagonal = sin((p.x + p.y) * 28.0) * sin((p.x - p.y) * 28.0)
    return smoothstep(0.25, 0.72, diagonal)
}

def yard_reflection(position, reflected : float3; rough : float) : float3 {
    let horizon = exp(-abs(reflected.z) * 5.0)
    if (lp_environment_ready < 0.5) {
        return lerp(float3(0.045, 0.065, 0.13), float3(0.25, 0.21, 0.22), horizon)
    }
    let safe_ray = sign(reflected) * max(abs(reflected), float3(0.0001)) + float3(0.000001)
    let bounds_min = float3(-14.0, -13.0, 0.0)
    let bounds_max = float3(14.0, 15.0, 8.0)
    let exits = max((bounds_min - position) / safe_ray, (bounds_max - position) / safe_ray)
    let nearest_exit = min(exits.x, min(exits.y, exits.z))
    let distance = max(nearest_exit, 0.0)
    let projected = position + reflected * distance - PROBE_CENTER
    let direction = lerp(reflected, projected, step(0.001, distance))
    return textureLod(lp_environment, direction, rough * 6.0).xyz
}

[fragment_program]
def fs_latch_surface {
    let geometric_normal = normalize(f_normal)
    let view = normalize(f_CameraPos - f_world)
    let light = normalize(f_SunDir)
    var mapped = lp_material_position
    let footprint = max(length(dFdx(mapped)), length(dFdy(mapped)))
    let plate = geometric_normal.z > 0.9 && f_world.z > -0.005 && f_world.z < 0.04
    if (plate) {
        let parallax_fade = 1.0 - smoothstep(0.015, 0.055, footprint)
        let direction = view.xy / max(view.z, 0.25) * (0.012 * parallax_fade)
        var layer = 1.0
        for (_step in range(8)) {
            if (layer <= plate_height(mapped.xy)) { break }
            mapped.x -= direction.x / 8.0
            mapped.y -= direction.y / 8.0
            layer -= 0.125
        }
    }
    let macro_map = lp_noise(mapped * 3.3)
    let fine_map = lerp(lp_noise(mapped * 18.0), 0.5, smoothstep(0.02, 0.10, footprint))
    let brushed_map = lerp(lp_noise(mapped * float3(65.0, 4.0, 4.0)), 0.5,
        smoothstep(0.008, 0.035, footprint))
    let noise = macro_map * 0.65 + fine_map * 0.35
    let wear = smoothstep(0.42, 0.70, noise) * lp_surface.z
    let grain = fine_map * 0.07
    let albedo = surface_albedo(f_Color.xyz * (0.91 + grain), wear, mapped, geometric_normal)
    let rough_map = macro_map * 0.18 + brushed_map * 0.12 + fine_map * 0.10 - 0.15
    let rough = clamp(lp_surface.x + rough_map + wear * 0.45 + footprint * 0.2, 0.19, 0.95)
    let metal = lp_surface.y * (1.0 - wear) * (0.76 + macro_map * 0.24)
    let tread = plate ? plate_height(mapped.xy) * (1.0 - smoothstep(0.015, 0.055, footprint)) : 0.0
    let height = (fine_map * 0.010 + brushed_map * 0.005 + tread * 0.030) * (1.0 - step(0.1, lp_surface.w))
    let dpdx = dFdx(f_world)
    let dpdy = dFdy(f_world)
    let r1 = cross(dpdy, geometric_normal)
    let r2 = cross(geometric_normal, dpdx)
    let determinant = dot(dpdx, r1)
    let gradient = (r1 * dFdx(height) + r2 * dFdy(height)) * sign(determinant) * 0.18 / max(abs(determinant), 0.00001)
    let n = normalize(geometric_normal - gradient)
    let nv = max(dot(n, view), 0.001)
    let nl = max(dot(n, light), 0.0)
    let fresnel_base = lerp(float3(0.045), albedo, metal)
    let light_pos = v_light_vp * float4(f_world, 1.0)
    let shadow_uv = light_pos.xyz / light_pos.w * 0.5 + float3(0.5)
    var visibility = 1.0
    if (shadow_uv.x > 0.0 && shadow_uv.x < 1.0 && shadow_uv.y > 0.0 && shadow_uv.y < 1.0 &&
            shadow_uv.z > 0.0 && shadow_uv.z < 1.0) {
        visibility = 0.0
        let bias = 0.00015 + 0.0005 * (1.0 - nl)
        for (y in range(-2, 3)) {
            for (x in range(-2, 3)) {
                let weight = float((3 - abs(x)) * (3 - abs(y))) / 81.0
                visibility += textureCompare(f_ShadowMap,
                    shadow_uv.xy + float2(float(x), float(y)) * f_ShadowTexel, shadow_uv.z - bias) * weight
            }
        }
    }
    let sky = lerp(float3(0.028, 0.031, 0.044), float3(0.11, 0.16, 0.27), n.z * 0.5 + 0.5)
    let reflected = reflect(-view, n)
    let environment = yard_reflection(f_world, reflected, rough)
    let rim = pow(1.0 - nv, 5.0)
    var color = (yard_brdf(albedo, metal, rough, n, view, light) * f_SunColor * visibility +
        albedo * sky * (1.0 - metal * 0.7) * 1.5 + environment * (fresnel_base + float3(rim * 0.3)) * (1.0 - rough * 0.65) +
        f_Color.xyz * lp_surface.w)
    for (i in range(YARD_LIGHTS)) {
        let delta = lp_light_pos[i].xyz - f_world
        let direction = delta / sqrt(max(dot(delta, delta), 0.0001))
        let shadow = i == 3 ? lamp_visibility(f_world) : 1.0
        color += yard_brdf(albedo, metal, rough, n, view, direction) * yard_light_radiance(i, f_world) * shadow
    }
    color += yard_brdf(albedo, metal, rough, n, view, normalize(float3(0.25, 0.85, 0.35))) * float3(1.1, 1.4, 2.2) * lp_flash
    let fog = 1.0 - exp(-max(length(f_CameraPos - f_world) - 10.0, 0.0) * 0.021)
    color = lerp(color, f_FogColor, fog)
    if (lp_debug_material == 1) { color = float3(rough) }
    elif (lp_debug_material == 2) { color = float3(metal) }
    elif (lp_debug_material == 3) { color = n * 0.5 + float3(0.5) }
    f_FragColor = float4(clamp(color, float3(0.0), float3(100.0)), metal * (1.0 - rough))
    let vn = normalize(float3x3(v_view) * n)
    f_FragNormalDepth = float4(vn * 0.5 + float3(0.5), saturate(-f_view_pos.z / f_ZFar))
}

[fragment_program]
def fs_latch_ao_blur {
    let center = texture(lp_depth_map, p_uv)
    if (center.w >= 0.999) {
        p_FragColor = float4(1.0)
        return
    }
    var sum = 0.0
    var weights = 0.0
    for (y in range(-2, 3)) {
        for (x in range(-2, 3)) {
            let uv = p_uv + float2(float(x), float(y)) * p_texel
            let neighbor = texture(lp_depth_map, uv)
            let depth_weight = exp(-abs(neighbor.w - center.w) * 130.0 * 8.0)
            let normal_weight = pow(saturate(dot(neighbor.xyz * 2.0 - float3(1.0),
                center.xyz * 2.0 - float3(1.0))), 8.0)
            let weight = depth_weight * normal_weight * float((3 - abs(x)) * (3 - abs(y)))
            sum += texture(p_tex0, uv).x * weight
            weights += weight
        }
    }
    p_FragColor = float4(float3(sum / max(weights, 0.0001)), 1.0)
}

enum Shape {
    box
    sphere
    cylinder
    rock
    rock_alternate
    beveled
}

struct Part {
    position : float3
    size : float3
    color : float3
    surface : float4 = float4(0.45, 0.8, 0.0, 0.0)
    rotation : float4 = float4(0.0, 0.0, 0.0, 1.0)
    shape : Shape = Shape.box
}

var scene_parts : array<Part>
var frame_parts : array<Part>
var cube : OpenGLGeometryFragment
var sphere : OpenGLGeometryFragment
var cylinder : OpenGLGeometryFragment
var rock : OpenGLGeometryFragment
var rock_alternate : OpenGLGeometryFragment
var beveled : OpenGLGeometryFragment
var surface_program = 0u

let IRON = float3(0.18, 0.23, 0.25)
let RUST = float3(0.49, 0.24, 0.085)
let CHROME = float3(0.65, 0.72, 0.74)
let BLACK = float3(0.035, 0.045, 0.05)
let TEAL = float3(0.15, 0.8, 0.67)
let AMBER = float3(1.0, 0.47, 0.10)
let PAINT = float4(0.43, 0.05, 0.6, 0.0)
let POLISHED = float4(0.18, 0.96, 0.08, 0.0)
let DARK_METAL = float4(0.56, 0.75, 0.3, 0.0)
let STONE = float4(0.90, 0.0, 0.0, 0.0)
let GLOW = float4(0.35, 0.4, 0.0, 3.0)

def robot_glow(resident : int; time : float) : float4 {
    return GLOW * float4(1.0, 1.0, 1.0, robot_indicator(resident, time))
}

def z_rotation(a : float) : float4 {
    return float4(0.0, 0.0, sin(a * 0.5), cos(a * 0.5))
}

def part(pos, size, color : float3; surface : float4 = DARK_METAL;
         shape : Shape = Shape.box; rot : float4 = float4(0.0, 0.0, 0.0, 1.0)) {
    let rounded = shape == Shape.box && min(size.x, min(size.y, size.z)) > 0.12 && max(size.x, max(size.y, size.z)) < 2.0
    frame_parts |> push(Part(position = pos, size = size, color = color,
        surface = surface, shape = rounded ? Shape.beveled : shape, rotation = rot))
}

def rod(a, b : float3; radius : float; color : float3 = CHROME) {
    let delta = b - a
    let direction = normalize(delta)
    let rotation = (direction.z < -0.999 ? float4(1.0, 0.0, 0.0, 0.0) :
        normalize(float4(-direction.y, direction.x, 0.0, 1.0 + direction.z)))
    part((a + b) * 0.5, float3(radius, radius, length(delta) * 0.5), color,
        POLISHED, Shape.cylinder, rotation)
}

def yard_details() {
    for (y in fixed_array(-0.95, 1.55)) {
        part(float3(4.0, y, 3.1), float3(0.18, 0.12, 3.1), IRON)
    }
    part(float3(4.0, 0.3, 6.2), float3(0.24, 1.48, 0.16), IRON)
    for (x in fixed_array(-5.3, -2.7)) {
        part(float3(x, 10.3, 3.1), float3(0.12, 0.18, 3.1), IRON)
    }
    part(float3(-4.0, 10.3, 6.2), float3(1.54, 0.24, 0.16), IRON)
    rod(float3(-10.4, 2.6, 0.0), float3(-10.4, 2.6, 4.8), 0.065, IRON)
    rod(float3(-10.4, 2.6, 4.8), float3(-7.7, 2.6, 4.8), 0.065, IRON)
    for (index in fixed_array(4, 5, 6)) {
        let location = lp_light_pos[index].xyz
        part(location + float3(0.0, 0.0, 0.12), float3(0.34, 0.18, 0.08), BLACK)
        part(location, float3(0.27, 0.12, 0.035), index == 6 ? TEAL : AMBER, GLOW)
        let foot = float3(location.xy + float2(0.0, 0.16), index == 4 ? 0.87 : 0.08)
        rod(location + float3(0.0, 0.16, 0.16), foot, 0.035, IRON)
        part(foot, float3(0.14, 0.14, 0.035), IRON)
    }
    for (side in fixed_array(-1.0, 1.0)) {
        part(actuator_center() + float3(side * 0.26, 0.0, -0.10), float3(0.07, 0.20, 0.06), BLACK)
        part(COIL_POSITION + float3(side * 0.39, 0.0, 0.9), float3(0.06, 0.30, 0.12), IRON)
    }
    for (row in range(4)) {
        let z = 0.6 + float(row) * 0.6
        rod(float3(4.4, -3.40, z), float3(11.4, -3.40, z), 0.065, row == 2 ? RUST : IRON)
        for (x in fixed_array(5.0, 8.0, 11.0)) {
            part(float3(x, -3.38, z), float3(0.05, 0.12, 0.12), CHROME)
        }
    }
    for (i in range(7)) {
        let x = float(i) * 0.36 - 1.10
        part(GENERATOR_POSITION + float3(x, 1.27, 0.79), float3(0.075, 0.045, 0.075), CHROME, POLISHED, Shape.cylinder,
            float4(0.7071068, 0.0, 0.0, 0.7071068))
    }
    for (x in fixed_array(-0.68, 0.68)) {
        for (z in fixed_array(1.0, 1.24)) {
            part(BRACK_POSITION + float3(x, -0.71, z), float3(0.055), CHROME, POLISHED, Shape.sphere)
        }
    }
    for (i in range(6)) {
        part(float3(-6.65, 11.0 + float(i) * 0.60, 1.35), float3(0.035, 0.22, 0.7), BLACK)
        part(float3(-6.59, 11.0 + float(i) * 0.60, 1.40), float3(0.02, 0.17, 0.04), TEAL, GLOW)
    }
}

def build_yard() {
    frame_parts |> clear()
    part(float3(0.0, 0.0, -0.25), float3(24.0, 24.0, 0.25), float3(0.17, 0.20, 0.19), STONE)
    for (x in range(-6, 7)) {
        for (y in range(-5, 7)) {
            part(float3(float(x) * 2.0, float(y) * 2.0, -0.015), float3(0.988, 0.988, 0.025),
                float3(0.24, 0.29, 0.28), float4(0.53, 0.6, 0.3, 0.0))
        }
    }
    for (wall_index in range(length(barriers))) {
        let wall = barriers[wall_index]
        continue if (!wall.render_wall)
        let border = abs(wall.center.x) > 13.0 || abs(wall.center.y - 1.0) > 13.0
        var visual_center = wall.center
        var visual_half = wall.half_size
        if (wall_index == 10 || wall_index == 11) {
            visual_center.y += 0.31
            visual_half.y -= 0.31
        }
        part(float3(visual_center, border ? 1.0 : 1.45), float3(visual_half, border ? 1.0 : 1.45), IRON, PAINT)
        part(float3(visual_center, border ? 2.03 : 2.94), float3(visual_half, 0.055), CHROME, POLISHED)
    }
    for (i in range(11)) {
        let x = float(i) * 2.2 - 11.0
        part(float3(x, 14.1, 3.0), float3(0.10, 0.10, 3.0), RUST, PAINT)
        rod(float3(x, 14.1, 5.9), float3(x + 1.0, 14.1, 6.5), 0.075, IRON)
    }
    for (i in range(7)) {
        let y = float(i) * 3.6 - 10.0
        part(float3(-12.8, y, 2.3), float3(0.1, 0.1, 2.3), IRON)
        part(float3(-12.8, y, 4.55), float3(0.10, 0.55, 0.055), TEAL, GLOW)
    }
    for (i in range(16)) {
        let a = float(i) * 0.3927
        let r = 32.0 + sin(float(i) * 7.0) * 3.0
        let location = float3(cos(a) * r, sin(a) * r, 2.0)
        let stone_color = i % 3 == 0 ? float3(0.25, 0.20, 0.15) : float3(0.14, 0.19, 0.21)
        part(location, float3(5.0, 4.0, 3.0 + sin(a * 3.0) * 1.5), stone_color, STONE,
            i % 2 == 0 ? Shape.rock : Shape.rock_alternate, z_rotation(a * 0.7))
        for (fragment in range(3)) {
            part(location + float3(cos(a + float(fragment)) * 2.5, sin(a + float(fragment)) * 2.5, -1.6),
                float3(1.0 + float(fragment) * 0.25, 0.85, 0.65), stone_color * 0.8, STONE, Shape.rock_alternate, z_rotation(float(fragment)))
        }
    }
    part(float3(9.0, 11.5, 5.0), float3(1.2, 1.2, 5.0), IRON, PAINT, Shape.cylinder)
    for (z in range(5)) { part(float3(9.0, 11.5, float(z) * 2.0 + 0.2), float3(1.25, 1.25, 0.055), CHROME, POLISHED, Shape.cylinder) }
    rod(float3(9.0, 11.5, 8.0), GENERATOR_POSITION + float3(0.0, 0.0, 2.15), 0.15, RUST)
    part(GENERATOR_POSITION + float3(0.0, 0.0, 0.325), float3(1.5, 1.2, 0.325), IRON, DARK_METAL)
    part(GENERATOR_POSITION + float3(0.0, 0.0, 1.45), float3(1.4, 1.1, 0.8), RUST, PAINT)
    for (i in range(9)) { part(GENERATOR_POSITION + float3(float(i) * 0.27 - 1.1, 1.15, 1.45),
        float3(0.055, 0.04, 0.50), BLACK) }
    part(float3(10.0, 1.0, 0.75), float3(1.0, 0.55, 0.12), CHROME, POLISHED)
    for (x in fixed_array(9.2, 10.8)) { part(float3(x, 1.0, 0.34), float3(0.08, 0.44, 0.34), IRON) }
    for (i in range(4)) { part(ACTUATOR_CRATE_CENTER + float3(float(i % 2) * SALVAGE_CRATE_SPACING, float(i / 2) * SALVAGE_CRATE_SPACING, 0.0),
        ACTUATOR_CRATE_HALF_SIZE, RUST, PAINT, Shape.box, z_rotation(float(i) * 0.21)) }
    part(float3(-4.0, 14.0, 1.1), float3(1.1, 0.6, 1.1), IRON)
    part(float3(-4.0, 13.38, 1.5), float3(0.85, 0.03, 0.33), TEAL, GLOW * float4(1.0, 1.0, 1.0, 0.22))
    part(ARCHIVE_RECORD_POSITION, float3(0.36, 0.09, 0.24), float3(0.42, 0.29, 0.12), DARK_METAL)
    for (line in range(4)) {
        part(ARCHIVE_RECORD_POSITION + float3(-0.04, -0.10, 0.13 - float(line) * 0.075), float3(0.23 - float(line % 2) * 0.05, 0.006, 0.008), BLACK)
    }
    yard_details()
    scene_parts := frame_parts
    frame_parts |> clear()
}

def brack_model(time : float) {
    let p = BRACK_POSITION
    for (s in fixed_array(-1.0, 1.0)) {
        part(p + float3(s * 0.78, 0.0, 0.38), float3(0.31, 0.93, 0.35), BLACK)
        for (i in range(7)) {
            part(p + float3(s * 0.78, float(i) * 0.28 - 0.84, 0.72), float3(0.33, 0.055, 0.06), IRON)
        }
        for (i in range(4)) {
            part(p + float3(s * 1.10, float(i) * 0.44 - 0.66, 0.38), float3(0.055, 0.22, 0.22), CHROME, DARK_METAL, Shape.sphere)
        }
    }
    part(p + float3(0.0, 0.0, 0.99), float3(0.72, 0.68, 0.36), RUST, PAINT)
    part(p + float3(0.0, 0.04, 1.5), float3(0.50, 0.43, 0.23), CHROME, POLISHED)
    let head = p + float3(0.0, -0.10, 1.98 + sin(time * 0.7) * 0.025)
    let first_head = length(frame_parts)
    part(head, float3(0.48, 0.37, 0.30), RUST, PAINT)
    part(head + float3(0.0, -0.38, 0.03), float3(0.38, 0.025, 0.11), BLACK)
    for (x in fixed_array(-0.19, 0.19)) { part(head + float3(x, -0.41, 0.03), float3(0.095, 0.03, 0.045), AMBER, robot_glow(0, time)) }
    rotate_model_parts(first_head, head, attachment_rotation(robot_attention(0, time)))
    rod(p + float3(-0.72, 0.0, 1.38), p + float3(-1.05, -0.50, 1.0), 0.14)
    let handover = sin(saturate((handover_until - get_uptime()) / 2.0) * PI)
    let inspection = robot_work_cycle(0, time)
    let hand = p + float3(-0.84, -1.05 - handover * 0.45, 0.8 + handover * 0.50 + inspection * 0.18)
    rod(p + float3(-1.05, -0.50, 1.0), hand, 0.10)
    part(hand, float3(0.23, 0.24, 0.20), IRON, POLISHED, Shape.sphere)
    let drill_head = drill_head_position(time)
    let elbow = p + float3(1.20, -0.85, 1.40)
    rod(p + float3(0.72, 0.0, 1.38), elbow, 0.17, RUST)
    part(elbow, float3(0.19), CHROME, DARK_METAL, Shape.sphere)
    rod(elbow, drill_head + float3(0.0, 0.0, 0.16), 0.13, RUST)
    rod(drill_head + float3(0.0, 0.0, 0.16), drill_head, 0.09, CHROME)
    for (i in range(5)) { part(drill_head - float3(0.0, 0.0, float(i) * 0.10),
        float3(0.24 - float(i) * 0.035, 0.24 - float(i) * 0.035, 0.07), CHROME, POLISHED, Shape.cylinder) }
    for (blade in range(3)) {
        for (level in range(4)) {
            let a = float(blade) * PI * 2.0 / 3.0 + drill_rotation(time) + float(level) * 0.65
            let b = a + 0.65
            let radius = 0.245 - float(level) * 0.035
            let center = drill_head - float3(0.0, 0.0, float(level) * 0.10)
            rod(center + float3(cos(a) * radius, sin(a) * radius, 0.0),
                center + float3(cos(b) * (radius - 0.035), sin(b) * (radius - 0.035), -0.10), 0.018, BLACK)
        }
    }
    part(drill_contact_position() - float3(0.0, 0.0, 0.06), float3(0.40, 0.36, 0.06), IRON, DARK_METAL)
}

def patch_model(time : float) {
    let first_part = length(frame_parts)
    let p = patch_position
    let bob = sin(time * 1.5) * 0.025
    part(p + float3(0.0, 0.0, 0.83 + bob), float3(0.47, 0.61, 0.26), float3(0.40, 0.61, 0.56), PAINT, Shape.sphere)
    for (side in fixed_array(-1.0, 1.0)) {
        for (leg in range(3)) {
            let y = float(leg) * 0.48 - 0.48
            let limp = !world.patch_repaired && leg == 0 && side < 0.0
            let motion = (sin(time * 6.0 + float(leg) * 2.0 + side) * 0.12 * patch_walk_blend +
                (leg == 0 ? sin(time * 1.8) * robot_work_cycle(1, time) * 0.055 : 0.0))
            let joint = p + float3(side * 0.76, y * 1.3, 0.65 + motion)
            rod(p + float3(side * 0.36, y, 0.88 + bob), joint, 0.055)
            rod(joint, p + float3(side * 1.0, y * 1.8, limp ? 0.4 : 0.10), 0.038, RUST)
            part(joint, float3(0.095), CHROME, POLISHED, Shape.sphere)
        }
    }
    rod(p + float3(0.0, -0.24, 0.97), p + float3(0.0, -0.35, 1.41 + bob), 0.07)
    let first_head = length(frame_parts)
    part(p + float3(0.0, -0.37, 1.46 + bob), float3(0.28, 0.24, 0.19), CHROME, POLISHED, Shape.sphere)
    for (x in fixed_array(-0.115, 0.115)) { part(p + float3(x, -0.58, 1.48 + bob), float3(0.055, 0.05, 0.055), TEAL, robot_glow(1, time), Shape.sphere) }
    rod(p + float3(0.18, -0.25, 1.53), p + float3(0.24, -0.18, 1.96), 0.018)
    part(p + float3(0.24, -0.18, 1.96), float3(0.045), AMBER, GLOW, Shape.sphere)
    rotate_model_parts(first_head, p + float3(0.0, -0.37, 1.46 + bob), attachment_rotation(robot_attention(1, time)))
    let orientation = z_rotation(patch_heading)
    for (i in range(first_part, length(frame_parts))) {
        frame_parts[i].position = p + quat_mul_vec(orientation, frame_parts[i].position - p)
        frame_parts[i].rotation = quat_mul(orientation, frame_parts[i].rotation)
    }
}

def vesper_model(time : float) {
    let first_part = length(frame_parts)
    let p = VESPER_POSITION
    let ceramic = float3(0.72, 0.77, 0.69)
    for (s in fixed_array(-1.0, 1.0)) {
        part(p + float3(s * 0.26, -0.12, 0.10), float3(0.16, 0.36, 0.10), BLACK)
        rod(p + float3(s * 0.26, 0.0, 0.23), p + float3(s * 0.19, 0.12, 1.12), 0.07)
        part(p + float3(s * 0.22, 0.05, 0.63), float3(0.12, 0.12, 0.35), ceramic, PAINT)
    }
    part(p + float3(0.0, 0.0, 1.42), float3(0.27, 0.20, 0.43), ceramic, float4(0.30, 0.1, 0.08, 0.0))
    part(p + float3(0.0, -0.21, 1.43), float3(0.025, 0.018, 0.30), TEAL, robot_glow(2, time))
    rod(p + float3(0.0, 0.0, 1.80), p + float3(0.0, 0.0, 2.12), 0.065)
    let head = p + float3(0.0, 0.0, 2.35 + sin(time) * 0.025)
    for (i in range(20)) {
        let a = float(i) * PI * 0.1 + time * 0.12
        let b = a + PI * 0.1
        rod(head + float3(cos(a) * 0.36, 0.0, sin(a) * 0.36),
            head + float3(cos(b) * 0.36, 0.0, sin(b) * 0.36), 0.04)
    }
    part(head, float3(0.16), BLACK, POLISHED, Shape.sphere)
    part(head + float3(0.0, -0.15, 0.0), float3(0.06, 0.035, 0.06), TEAL, robot_glow(2, time), Shape.sphere)
    for (s in fixed_array(-1.0, 1.0)) {
        rod(p + float3(s * 0.28, 0.0, 1.73), p + float3(s * 0.50, 0.04, 1.28), 0.045)
        let reading = robot_work_cycle(2, time)
        let hand = p + float3(s * (0.35 - reading * 0.09), -0.26 - reading * 0.16, 1.04 + reading * 0.28)
        rod(p + float3(s * 0.50, 0.04, 1.28), hand, 0.04)
        part(hand, float3(0.065, 0.07, 0.13), ceramic, POLISHED)
    }
    let reading = robot_work_cycle(2, time)
    part(p + float3(0.0, -0.30 - reading * 0.16, 1.10 + reading * 0.28), float3(0.30 - reading * 0.07, 0.045, 0.14), BLACK)
    part(p + float3(0.0, -0.35 - reading * 0.16, 1.10 + reading * 0.28), float3(0.25 - reading * 0.07, 0.008, 0.095), TEAL, GLOW)
    rotate_model_parts(first_part, p, attachment_rotation(robot_attention(2, time)))
}

def build_dynamic(time : float) {
    frame_parts |> clear()
    brack_model(time)
    patch_model(time)
    vesper_model(time)
    let clutch = GENERATOR_POSITION + float3(0.0, 1.29, 1.10)
    let lever = clutch + float3(sin(starter_progress * 16.0) * 0.25, 0.08, cos(starter_progress * 16.0) * 0.25)
    rod(clutch, lever, 0.035, CHROME)
    part(lever, float3(0.065), AMBER, DARK_METAL, Shape.sphere)
    if (starter_engaged && patch_can_assist()) {
        rod(patch_position + float3(0.0, -0.35, 1.0), clutch + float3(0.32, 0.0, 0.0), 0.045, CHROME)
    }
    let lamp = hanging_lamp(time)
    rod(float3(-7.7, 2.6, 4.8), lamp + float3(0.0, 0.0, 0.13), 0.015, BLACK)
    part(lamp + float3(0.0, 0.0, 0.15), float3(0.24, 0.24, 0.08), IRON, DARK_METAL, Shape.cylinder)
    part(lamp, float3(0.15, 0.15, 0.05), AMBER, GLOW, Shape.sphere)
    let brack_lamp = robot_attachment(0, time)
    part(brack_lamp.position - brack_lamp.direction * 0.08, float3(0.18, 0.13, 0.16), BLACK, DARK_METAL, Shape.box, brack_lamp.rotation)
    part(brack_lamp.position, float3(0.12, 0.03, 0.10), AMBER * robot_indicator(0, time), GLOW, Shape.box, brack_lamp.rotation)
    part(lp_light_pos[1].xyz, float3(0.075), float3(0.65, 0.86, 1.0) * robot_indicator(1, time),
        GLOW * float4(1.0, 1.0, 1.0, inspection_lamp(time)), Shape.sphere)
    let workshop_opening = max(workshop_aperture, 0.0)
    let archive_opening = max(archive_aperture, 0.0)
    part(float3(4.0, 0.3, 1.45 + workshop_opening * 3.05), float3(0.28, 1.1, 1.45), RUST, PAINT)
    part(float3(-4.0, 10.3, 1.45 + archive_opening * 3.05), float3(1.18, 0.25, 1.45), IRON, PAINT)
    part(float3(3.65, 1.55, 1.6), float3(0.04, 0.16, 0.10), world.workshop_open ? TEAL : AMBER, GLOW)
    part(float3(-2.5, 9.96, 1.6), float3(0.16, 0.04, 0.10), world.archive_open ? TEAL : AMBER, GLOW)
    part(GENERATOR_POSITION + float3(0.7, 1.24, 1.7), float3(0.25, 0.07, 0.11), BLACK)
    part(GENERATOR_POSITION + float3(0.7, 1.32, 1.7), float3(0.2, 0.02, 0.06), world.power ? TEAL : AMBER, GLOW)
    if (find_index(world.salvaged, "coil") < 0) {
        for (i in range(5)) { part(COIL_POSITION + float3(float(i) * 0.12 - 0.24, 0.0, 1.01),
            float3(0.045, 0.23, 0.23), RUST, POLISHED, Shape.sphere) }
    }
    if (actuator_available()) { rod(actuator_center() - float3(0.3, 0.0, 0.0),
        actuator_center() + float3(0.3, 0.0, 0.0), ACTUATOR_RADIUS) }
}

def rotate_model_parts(first : int; center : float3; rotation : float4) {
    for (i in range(first, length(frame_parts))) {
        frame_parts[i].position = center + quat_mul_vec(rotation, frame_parts[i].position - center)
        frame_parts[i].rotation = quat_mul(rotation, frame_parts[i].rotation)
    }
}

def draw_part(item : Part; shadow : bool) {
    v_model = compose(item.position, item.rotation, item.size)
    if (shadow) {
        vs_shadow_bind_uniform(shadow_program)
    } else {
        f_Color = float4(item.color, 1.0)
        lp_surface = item.surface
        lp_object_scale = item.size
        vs_latch_surface_bind_uniform(surface_program)
        fs_latch_surface_bind_uniform(surface_program)
    }
    if (item.shape == Shape.box) { cube |> draw_geometry_fragment() }
    elif (item.shape == Shape.sphere) { sphere |> draw_geometry_fragment() }
    elif (item.shape == Shape.cylinder) { cylinder |> draw_geometry_fragment() }
    elif (item.shape == Shape.beveled) { beveled |> draw_geometry_fragment() }
    elif (item.shape == Shape.rock_alternate) { rock_alternate |> draw_geometry_fragment() }
    else { rock |> draw_geometry_fragment() }
}

def draw_yard(shadow : bool) {
    glUseProgram(shadow ? shadow_program : surface_program)
    for (item in scene_parts) { draw_part(item, shadow) }
    for (item in frame_parts) { draw_part(item, shadow) }
}

def latch_texture(width, height : int; internal_format, format, data_type, filter : uint) : uint {
    var handle = 0u
    glGenTextures(1, safe_addr(handle))
    glBindTexture(GL_TEXTURE_2D, handle)
    glTexImage2D(GL_TEXTURE_2D, 0, int(internal_format), width, height, 0, format, data_type, null)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, int(filter))
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, int(filter))
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    return handle
}

def init_renderer() {
    update_yard_lights(0.0)
    create_scene_programs()
    create_postfx_programs()
    sky_program = cache_shader_program(vs_latch_sky`shader_text, fs_latch_sky`shader_text)
    surface_program = cache_shader_program(vs_latch_surface`shader_text, fs_latch_surface`shader_text)
    ao_blur_program = cache_shader_program(vs_post`shader_text, fs_latch_ao_blur`shader_text)
    atmosphere_program = cache_shader_program(vs_post`shader_text, fs_latch_atmosphere`shader_text)
    dust_program = cache_shader_program(vs_latch_dust`shader_text, fs_latch_dust`shader_text)
    sparks_program = cache_shader_program(vs_latch_sparks`shader_text, fs_latch_sparks`shader_text)
    cube <- create_geometry_fragment <| gen_cube()
    beveled <- create_geometry_fragment <| gen_beveled_box(float3(1.0), 0.09, 3)
    sphere <- create_geometry_fragment <| gen_sphere(28, 20, false)
    cylinder <- create_geometry_fragment <| gen_cylinder(GenDirection.xy, 28)
    rock <- create_geometry_fragment <| fractured_outcrop(1.0)
    rock_alternate <- create_geometry_fragment <| fractured_outcrop(3.7)
    build_yard()
    lamp_shadow_tex = latch_texture(1024, 1024, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL)
    glGenFramebuffers(1, safe_addr(lamp_shadow_fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, lamp_shadow_fbo)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, lamp_shadow_tex, 0)
    var targets = fixed_array(uint(GL_NONE))
    glDrawBuffers(1, unsafe(addr(targets[0])))
    glReadBuffer(uint(GL_NONE))
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { panic("Lamp shadow framebuffer incomplete") }
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
}

def draw_twilight_sky() {
    glDisable(GL_CULL_FACE)
    glDisable(GL_DEPTH_TEST)
    glDepthMask(false)
    glUseProgram(sky_program)
    vs_latch_sky_bind_uniform(sky_program)
    fs_latch_sky_bind_uniform(sky_program)
    fullscreen_pass()
    glEnable(GL_DEPTH_TEST)
    glDepthMask(true)
    glEnable(GL_CULL_FACE)
}

def capture_yard_probe() {
    let state = (world.power ? 1 : 0) + (world.workshop_open ? 2 : 0) + (world.archive_open ? 4 : 0)
    return if (state == probe_state)
    if (probe_fbo == 0u) {
        let format = fx_hdr ? GL_RGBA16F : GL_RGBA8
        probe_color = latch_texture(PROBE_SIZE, PROBE_SIZE, format, GL_RGBA, fx_hdr ? GL_HALF_FLOAT : GL_UNSIGNED_BYTE, GL_LINEAR)
        glGenRenderbuffers(1, safe_addr(probe_depth))
        glBindRenderbuffer(GL_RENDERBUFFER, probe_depth)
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, PROBE_SIZE, PROBE_SIZE)
        glGenFramebuffers(1, safe_addr(probe_fbo))
        glBindFramebuffer(GL_FRAMEBUFFER, probe_fbo)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, probe_color, 0)
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, probe_depth)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { panic("Reflection probe framebuffer incomplete") }
        glGenTextures(1, safe_addr(probe_cube))
        glBindTexture(GL_TEXTURE_CUBE_MAP, probe_cube)
        for (face in range(6)) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + uint(face), 0, int(format), PROBE_SIZE, PROBE_SIZE, 0, GL_RGBA, fx_hdr ? GL_HALF_FLOAT : GL_UNSIGNED_BYTE, null)
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP)
    }
    let saved = (view = v_view, projection = v_projection, position = f_CameraPos, forward = f_CamFwd,
        right = f_CamRight, up = f_CamUp, scale = f_CamScale, flash = lp_flash)
    lp_environment := probe_cube
    lp_environment_ready = 0.0
    lp_flash = 0.0
    f_CameraPos = PROBE_CENTER
    f_CamScale = float2(1.0)
    v_projection = perspective_rh_minus1_to_1(PI * 0.5, 1.0, 0.08, 130.0)
    let directions = fixed_array(float3(1.0, 0.0, 0.0), float3(-1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0),
        float3(0.0, -1.0, 0.0), float3(0.0, 0.0, 1.0), float3(0.0, 0.0, -1.0))
    let ups = fixed_array(float3(0.0, -1.0, 0.0), float3(0.0, -1.0, 0.0), float3(0.0, 0.0, 1.0),
        float3(0.0, 0.0, -1.0), float3(0.0, -1.0, 0.0), float3(0.0, -1.0, 0.0))
    glBindFramebuffer(GL_FRAMEBUFFER, probe_fbo)
    glViewport(0, 0, PROBE_SIZE, PROBE_SIZE)
    for (face in range(6)) {
        f_CamFwd = directions[face]
        f_CamUp = ups[face]
        f_CamRight = normalize(cross(f_CamFwd, f_CamUp))
        v_view = look_at_rh(PROBE_CENTER, PROBE_CENTER + f_CamFwd, f_CamUp)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        draw_twilight_sky()
        glUseProgram(surface_program)
        for (item in scene_parts) { draw_part(item, false) }
        glActiveTexture(GL_TEXTURE0 + 7u)
        glBindTexture(GL_TEXTURE_CUBE_MAP, probe_cube)
        glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + uint(face), 0, 0, 0, 0, 0, PROBE_SIZE, PROBE_SIZE)
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP)
    v_view = saved.view
    v_projection = saved.projection
    f_CameraPos = saved.position
    f_CamFwd = saved.forward
    f_CamRight = saved.right
    f_CamUp = saved.up
    f_CamScale = saved.scale
    lp_flash = saved.flash
    lp_environment_ready = 1.0
    probe_state = state
}

def run_yard_atmosphere(width, height : int) {
    if (atmosphere_width != width || atmosphere_height != height) {
        glDeleteFramebuffers(1, safe_addr(atmosphere_fbo))
        glDeleteTextures(1, safe_addr(atmosphere_color))
        atmosphere_color = latch_texture(width, height, fx_hdr ? GL_RGBA16F : GL_RGBA8, GL_RGBA,
            fx_hdr ? GL_HALF_FLOAT : GL_UNSIGNED_BYTE, GL_LINEAR)
        glGenFramebuffers(1, safe_addr(atmosphere_fbo))
        glBindFramebuffer(GL_FRAMEBUFFER, atmosphere_fbo)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, atmosphere_color, 0)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { panic("Atmosphere framebuffer incomplete") }
        atmosphere_width = width
        atmosphere_height = height
    }
    glBindFramebuffer(GL_FRAMEBUFFER, atmosphere_fbo)
    glViewport(0, 0, width, height)
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
    glDisable(GL_BLEND)
    glUseProgram(atmosphere_program)
    p_tex0 := scene_color
    p_tex1 := scene_nd
    lp_effect_switches = float2(render_ssr ? 1.0 : 0.0, render_dust ? 1.0 : 0.0)
    lp_effect_texel = float2(1.0 / float(width), 1.0 / float(height))
    vs_post_bind_uniform(atmosphere_program)
    fs_latch_atmosphere_bind_uniform(atmosphere_program)
    trace_render_stage(61)
    fullscreen_pass()
    trace_render_stage(62)
    glBindFramebuffer(GL_FRAMEBUFFER, atmosphere_fbo)
    glReadBuffer(GL_COLOR_ATTACHMENT0)
    glActiveTexture(GL_TEXTURE0)
    glBindTexture(GL_TEXTURE_2D, scene_color)
    trace_render_stage(63)
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height)
    trace_render_stage(64)
    glBindFramebuffer(GL_FRAMEBUFFER, scene_fbo)
}

def draw_drill_sparks(time : float) {
    lp_spark_strength = drill_contact(time)
    return if (lp_spark_strength <= 0.0)
    lp_spark_origin = drill_contact_position()
    begin_transparent_span()
    glBlendFunc(GL_SRC_ALPHA, uint(GL_ONE))
    glEnable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
    glUseProgram(sparks_program)
    vs_latch_sparks_bind_uniform(sparks_program)
    fs_latch_sparks_bind_uniform(sparks_program)
    glBindVertexArray(empty_vao)
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 48)
    end_transparent_span()
}

def draw_yard_dust() {
    return if (!render_dust)
    begin_transparent_span()
    glDisable(GL_CULL_FACE)
    glEnable(GL_DEPTH_TEST)
    glUseProgram(dust_program)
    vs_latch_dust_bind_uniform(dust_program)
    fs_latch_dust_bind_uniform(dust_program)
    glBindVertexArray(empty_vao)
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1400)
    end_transparent_span()
}

def render_yard(width, height : int; time : float) {
    if (trace_render_errors) { render_errors |> clear() }
    let render_time = render_time_override >= 0.0 ? render_time_override : time
    update_yard_lights(render_time)
    let aspect = float(width) / float(height)
    let forward = float3(sin(heading) * cos(pitch), cos(heading) * cos(pitch), sin(pitch))
    v_view = look_at_rh(avatar, avatar + forward, float3(0.0, 0.0, 1.0))
    v_projection = perspective_rh_minus1_to_1(PI * 0.37, aspect, 0.07, 130.0)
    f_CameraPos = avatar
    f_CamFwd = forward
    f_CamRight = normalize(cross(forward, float3(0.0, 0.0, 1.0)))
    f_CamUp = normalize(cross(f_CamRight, forward))
    f_CamScale = float2(tan(PI * 0.185) * aspect, tan(PI * 0.185))
    f_SunDir = normalize(float3(-0.75, -0.6, 0.18))
    f_SunColor = float3(0.72, 0.42, 0.27)
    f_SkyColor = float3(0.035, 0.065, 0.14)
    f_HorizonColor = float3(0.08, 0.10, 0.16)
    f_FogColor = f_HorizonColor
    f_NightAmount = 0.0
    f_GameTime = (render_time_override >= 0.0 ? render_time_override : time) * 2.5
    lp_debug_material = render_material_mode
    f_ZFar = 130.0
    v_light_vp = (ortho_rh(-23.0, 23.0, -23.0, 23.0, 0.1, 100.0) *
        look_at_rh(float3(0.0, 1.0, 0.0) + f_SunDir * 45.0, float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0)))
    ensure_render_targets(width, height)
    f_ShadowMap := shadow_tex
    f_ShadowTexel = float2(1.0 / float(SHADOW_SIZE))
    lp_depth_map := scene_nd
    glUseProgram(ao_blur_program)
    fs_latch_ao_blur_bind_uniform(ao_blur_program)
    build_dynamic(render_time)
    trace_render_stage(1)
    begin_shadow_pass()
    draw_yard(true)
    end_shadow_pass()
    trace_render_stage(2)
    let sun_vp = v_light_vp
    v_light_vp = lp_lamp_vp
    glBindFramebuffer(GL_FRAMEBUFFER, lamp_shadow_fbo)
    glViewport(0, 0, 1024, 1024)
    glClear(GL_DEPTH_BUFFER_BIT)
    glCullFace(GL_FRONT)
    draw_yard(true)
    glCullFace(GL_BACK)
    v_light_vp = sun_vp
    lp_lamp_shadow := lamp_shadow_tex
    trace_render_stage(3)
    capture_yard_probe()
    trace_render_stage(4)
    begin_scene_pass(f_HorizonColor)
    draw_twilight_sky()
    draw_yard(false)
    trace_render_stage(5)
    run_yard_atmosphere(width, height)
    trace_render_stage(6)
    draw_yard_dust()
    draw_drill_sparks(render_time)
    trace_render_stage(7)
    run_postfx(PostSettings(exposure = 1.35, bloom_strength = render_bloom ? 0.24 : 0.0, ao_strength = 1.0,
        ao_apply = render_ao ? 0.8 : 0.0,
        grade_tint = float3(0.87, 1.02, 0.94), grade_amount = 0.15, vignette_strength = 0.25,
        grain = 0.012, tan_half_fov = f_CamScale, z_far = 130.0, time = time))
    trace_render_stage(8)
}
