options gen2
options persistent_heap

require ../river_run/rr_shaders.das public
require lp_world public
require lp_behavior public

let YARD_LIGHTS = 12
var @uniform lp_light_pos : float4[YARD_LIGHTS]
var @uniform lp_light_color : float4[YARD_LIGHTS]
var @uniform lp_light_direction : float4[YARD_LIGHTS]
var @uniform lp_lamp_vp : float4x4
var @uniform @stage = 6 lp_lamp_shadow : sampler2DShadow
var @uniform lp_flash : float
var @uniform lp_clock : float
var @live reduced_flashes = false

def storm_flash(time : float; reduced : bool) : float {
    if (reduced) { return 0.0 }
    let cycle = floor(time / 47.0)
    let phase = time - cycle * 47.0
    let onset = 12.0 + sin(cycle * 12.73) * 7.0
    let age = phase - onset
    return (smoothstep(0.0, 0.10, age) * (1.0 - smoothstep(0.18, 0.80, age)) * 0.65 +
        smoothstep(1.1, 1.16, age) * (1.0 - smoothstep(1.25, 1.85, age)) * 0.38)
}

def yard_wind(time : float) : float2 {
    return float2(0.55 + sin(time * 0.19) * 0.18, 0.16 + sin(time * 0.31 + 1.4) * 0.12)
}

def hanging_lamp(time : float) : float3 {
    let wind = yard_wind(time)
    let swing = float2(sin(time * 1.43) * (0.18 + wind.x * 0.12), sin(time * 1.09 + 0.7) * 0.15)
    return float3(-7.7 + swing.x, 2.6 + swing.y, 3.25 + dot(swing, swing) * 0.22)
}

def set_yard_light(index : int; position, color, direction : float3; radius, cone : float) {
    lp_light_pos[index] = float4(position, radius)
    lp_light_color[index] = float4(color, 1.0)
    lp_light_direction[index] = float4(normalize(direction), cone)
}

def update_yard_lights(time : float) {
    lp_clock = time
    lp_flash = storm_flash(time, reduced_flashes)
    let brack = robot_attachment(0, time)
    let patch = robot_attachment(1, time)
    let vesper = robot_attachment(2, time)
    set_yard_light(0, brack.position, float3(13.0, 6.8, 2.8) * robot_indicator(0, time), brack.direction, 7.0, 0.62)
    set_yard_light(1, patch.position, float3(5.4, 9.6, 11.0) * robot_indicator(1, time) * inspection_lamp(time), patch.direction, 6.5, 0.70)
    set_yard_light(2, vesper.position, float3(2.4, 6.2, 5.6) * robot_indicator(2, time), vesper.direction, 4.5, -1.0)
    let lamp = hanging_lamp(time)
    set_yard_light(3, lamp, float3(27.0, 16.0, 6.0), normalize(lamp - float3(-7.7, 2.6, 4.8)), 9.0, 0.40)
    set_yard_light(4, COIL_POSITION + float3(0.0, 0.0, 2.65), float3(11.0, 8.0, 4.5), float3(0.0, 0.0, -1.0), 5.0, 0.25)
    set_yard_light(5, float3(7.6, 3.9, 2.75), float3(12.0, 6.3, 2.7), float3(0.0, -0.6, -1.0), 7.5, 0.12)
    set_yard_light(6, float3(-4.8, 12.2, 2.4), float3(7.0, 8.0, 6.6), float3(0.8, 0.9, -1.0), 6.0, 0.05)
    set_yard_light(7, GENERATOR_POSITION + float3(0.70, 1.36, 1.70), world.power ? float3(1.5, 8.0, 5.0) : float3(3.6, 1.1, 0.2), float3(0.0, 1.0, 0.0), 3.5, -1.0)
    set_yard_light(8, float3(-12.8, -2.8, 4.40), float3(1.8, 4.5, 4.0), float3(0.3, 0.0, -1.0), 6.0, 0.1)
    set_yard_light(9, float3(-12.8, 8.0, 4.40), float3(1.8, 4.5, 4.0), float3(0.3, 0.0, -1.0), 6.0, 0.1)
    set_yard_light(10, float3(3.55, 1.55, 1.6), world.workshop_open ? float3(0.5, 2.0, 1.2) : float3(2.0, 0.8, 0.15), float3(-1.0, 0.0, 0.0), 2.2, -1.0)
    set_yard_light(11, float3(-2.5, 9.85, 1.6), world.archive_open ? float3(0.5, 2.0, 1.2) : float3(2.0, 0.8, 0.15), float3(0.0, -1.0, 0.0), 2.2, -1.0)
    lp_lamp_vp = (perspective_rh_minus1_to_1(2.30, 1.0, 0.15, 14.0) *
        look_at_rh(lamp, lamp + lp_light_direction[3].xyz, float3(0.0, 1.0, 0.0)))
}

def lamp_visibility(position : float3) : float {
    let clip = lp_lamp_vp * float4(position, 1.0)
    let uv = clip.xyz / max(clip.w, 0.0001) * 0.5 + float3(0.5)
    if (clip.w <= 0.0 || uv.x <= 0.0 || uv.x >= 1.0 || uv.y <= 0.0 || uv.y >= 1.0 || uv.z >= 1.0) { return 1.0 }
    var visibility = 0.0
    for (y in range(-1, 2)) {
        for (x in range(-1, 2)) {
            visibility += textureCompare(lp_lamp_shadow, uv.xy + float2(float(x), float(y)) / 1024.0, uv.z - 0.0012)
        }
    }
    return visibility / 9.0
}

def yard_light_radiance(index : int; position : float3) : float3 {
    let delta = lp_light_pos[index].xyz - position
    let distance2 = dot(delta, delta)
    let radius = lp_light_pos[index].w
    let falloff = saturate(1.0 - distance2 / (radius * radius))
    let cone = lp_light_direction[index].w
    let cosine = dot(-delta / sqrt(max(distance2, 0.001)), lp_light_direction[index].xyz)
    let beam = cone < 0.0 ? 1.0 : smoothstep(cone, min(cone + 0.22, 0.99), cosine)
    return lp_light_color[index].xyz * (falloff * falloff * beam / (1.0 + distance2))
}

def yard_brdf(albedo : float3; metal, rough : float; n, view, light : float3) : float3 {
    let delta = view + light
    let h = delta / sqrt(max(dot(delta, delta), 0.00001))
    let nv = max(dot(n, view), 0.001)
    let nl = max(dot(n, light), 0.0)
    let nh = max(dot(n, h), 0.0)
    let vh = max(dot(view, h), 0.0)
    let a2 = rough * rough * rough * rough
    let denom = nh * nh * (a2 - 1.0) + 1.0
    let distribution = a2 / max(PI * denom * denom, 0.000001)
    let k = (rough + 1.0) * (rough + 1.0) / 8.0
    let geometry = nv / (nv * (1.0 - k) + k) * nl / (nl * (1.0 - k) + k)
    let f0 = lerp(float3(0.04), albedo, metal)
    let fresnel = f0 + (float3(1.0) - f0) * pow(1.0 - vh, 5.0)
    let specular = distribution * geometry * fresnel / max(4.0 * nv * nl, 0.001)
    return ((float3(1.0) - fresnel) * albedo * ((1.0 - metal) / PI) + specular) * nl
}
