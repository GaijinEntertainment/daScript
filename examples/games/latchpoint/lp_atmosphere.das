options gen2
options persistent_heap

require lp_lighting public
require ../river_run/rr_postfx.das public

var @live render_ssr = true
var @live render_dust = true
var @uniform lp_effect_switches : float2
var @uniform lp_effect_texel : float2
var @inout dust_uv : float2
var @inout dust_color : float3
var @inout dust_alpha : float
var @uniform lp_spark_strength : float
var @uniform lp_spark_origin : float3

[vertex_program]
def vs_latch_sparks {
    let id = float(gl_InstanceIndex)
    let seed = float3(dust_hash(id + 17.0), dust_hash(id + 74.0), dust_hash(id + 194.0))
    let phase = lp_clock * 2.5 + seed.x
    let age = (phase - floor(phase)) * 0.4
    let velocity = float3((seed.x - 0.5) * 3.0, (seed.y - 0.5) * 3.0, 0.8 + seed.z * 1.6)
    let position = lp_spark_origin + velocity * age - float3(0.0, 0.0, 4.0 * age * age)
    let vertex = gl_VertexIndex
    let corner = float2(vertex == 1 || vertex == 2 || vertex == 4 ? 1.0 : -1.0,
        vertex == 2 || vertex == 4 || vertex == 5 ? 1.0 : -1.0)
    let offset = f_CamRight * corner.x * 0.007 + f_CamUp * corner.y * (0.012 + seed.y * 0.02)
    gl_Position = v_projection * (v_view * float4(position + offset, 1.0))
    dust_uv = corner
    dust_alpha = (1.0 - age / 0.4) * lp_spark_strength
}

[fragment_program]
def fs_latch_sparks {
    f_FragColor = float4(float3(7.0, 2.5, 0.25), exp(-dot(dust_uv, dust_uv) * 4.0) * dust_alpha)
}

def dust_hash(value : float) : float {
    let h = sin(value * 127.1 + 31.7) * 43758.54
    return h - floor(h)
}

[vertex_program]
def vs_latch_dust {
    let id = float(gl_InstanceIndex)
    let wind = yard_wind(lp_clock)
    let seed = float3(dust_hash(id + 1.0), dust_hash(id + 53.0), dust_hash(id + 157.0))
    let travel = seed + float3(lp_clock * 0.011, lp_clock * 0.003 + sin(lp_clock * 0.31) * 0.007, 0.0)
    let wrapped = travel - floor(travel)
    let position = wrapped * float3(28.0, 28.0, 4.5) + float3(-14.0, -13.0, 0.1)
    let vertex = gl_VertexIndex
    let corner = float2(vertex == 1 || vertex == 2 || vertex == 4 ? 1.0 : -1.0,
        vertex == 2 || vertex == 4 || vertex == 5 ? 1.0 : -1.0)
    let size = 0.006 + seed.z * 0.014
    let offset = f_CamRight * corner.x * size * (1.0 + wind.x) + f_CamUp * corner.y * size
    gl_Position = v_projection * (v_view * float4(position + offset, 1.0))
    dust_uv = corner
    var illumination = float3(0.08, 0.10, 0.16)
    for (light in range(7)) { illumination += yard_light_radiance(light, position) * 0.32 }
    dust_color = illumination * float3(0.8, 0.73, 0.63)
    dust_alpha = smoothstep(0.3, 1.1, length(position - f_CameraPos)) * 0.45
}

[fragment_program]
def fs_latch_dust {
    let alpha = exp(-dot(dust_uv, dust_uv) * 3.8) * dust_alpha
    f_FragColor = float4(dust_color, alpha)
}

def effect_view_position(uv : float2; depth : float) : float3 {
    return float3((uv * 2.0 - float2(1.0)) * f_CamScale * depth, -depth)
}

[fragment_program]
def fs_latch_atmosphere {
    let source = texture(p_tex0, p_uv)
    let nd = texture(p_tex1, p_uv)
    let depth = nd.w * 130.0
    let origin = effect_view_position(p_uv, depth)
    var color = source.xyz
    if (lp_effect_switches.x > 0.5 && nd.w < 0.999 && source.w > 0.14) {
        let normal = normalize(nd.xyz * 2.0 - float3(1.0))
        let ray = reflect(normalize(origin), normal)
        let start = origin + normal * 0.075
        var previous = 0.0
        for (step_index in range(32)) {
            let distance = 0.18 + float(step_index) * 0.24
            let point = start + ray * distance
            if (point.z > -0.08) { break }
            let uv = (point.xy / (f_CamScale * -point.z)) * 0.5 + float2(0.5)
            if (uv.x <= 0.015 || uv.y <= 0.015 || uv.x >= 0.985 || uv.y >= 0.985) { break }
            let hit_nd = texture(p_tex1, uv)
            let delta = -point.z - hit_nd.w * 130.0
            if (hit_nd.w < 0.999 && delta > 0.0 && delta < 0.18 + distance * 0.015 && previous <= 0.0) {
                let edge = min(min(uv.x, uv.y), min(1.0 - uv.x, 1.0 - uv.y))
                let confidence = smoothstep(0.02, 0.15, edge) * (1.0 - smoothstep(5.0, 7.6, distance))
                let reflected = (texture(p_tex0, uv).xyz * 0.5 +
                    texture(p_tex0, uv + lp_effect_texel * 2.0).xyz * 0.25 +
                    texture(p_tex0, uv - lp_effect_texel * 2.0).xyz * 0.25)
                color = lerp(color, color * 0.85 + reflected * 0.65, source.w * confidence * 0.6)
                break
            }
            previous = delta
        }
    }
    if (lp_effect_switches.y > 0.5) {
        let ray = normalize(f_CamFwd + f_CamRight * (p_uv.x * 2.0 - 1.0) * f_CamScale.x +
            f_CamUp * (p_uv.y * 2.0 - 1.0) * f_CamScale.y)
        let distance = min(length(origin), 32.0)
        var scattering = float3(0.0)
        let stride = distance / 20.0
        for (i in range(20)) {
            let point = f_CameraPos + ray * ((float(i) + 0.5) * stride)
            let density = (0.65 + sin(point.x * 1.7 + point.y * 0.8 - lp_clock * 0.4) * 0.20) * exp(-max(point.z, 0.0) * 0.35)
            let lamp = yard_light_radiance(3, point) * lamp_visibility(point)
            let robots = yard_light_radiance(0, point) + yard_light_radiance(1, point)
            scattering += (lamp + robots * 0.6) * density * stride * 0.010
        }
        color += scattering
    }
    p_FragColor = float4(clamp(color, float3(0.0), float3(100.0)), source.w)
}
