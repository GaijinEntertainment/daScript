require engine.render.shader_dsl

// Kaleidoscope — maps UV into polar coordinates, folds the angle into one
// of `segments` slices, then samples a texture through the mirrored UV.
// Uses branches to handle the mirroring and quadrant mapping.

var {
    albedo_texture = Sampler2D("%builtin_package/logo.png")
    segments       = 6.0
    zoom           = 0.4
    speed          = 0.2
    twist          = 0.5
}

def polar_fold(uv : float2; segs : float; t : float) : float2 {
    let c  = uv - float2(0.5, 0.5)
    let cx = c.x
    let cy = c.y
    let r  = length(c)
    let angle = atan2(cy, cx) + t

    let slice = 3.14159 * 2.0 / segs
    let a = frac(angle / slice) * slice
    let mirrored_a = a > slice * 0.5 ? slice - a : a

    return float2(cos(mirrored_a), sin(mirrored_a)) * r * zoom + float2(0.5, 0.5)
}

[pixel_shader]
def kaleidoscope(inp : PbrInput) {
    let t = g_Time * speed
    let folded = polar_fold(inp.uv, segments, t + length(inp.uv - float2(0.5, 0.5)) * twist)
    let c = tex2d(albedo_texture, folded)

    return PbrOutput(
        albedo = c.xyz,
        emission = c.xyz * 0.3,
        emissionStrength = 0.5,
        metalness = 0.0,
        roughness = 1.0,
        ao = 1.0
    )
}
