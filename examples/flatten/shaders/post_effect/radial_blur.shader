require engine.render.shader_dsl

// Radial blur — zooms/blurs outward from screen center.
// Used for impact hits, explosions, speed boosts, flash effects.

var {
    strength   = 0.25   // blur radius as fraction of screen
    samples    = 8.0    // number of blur taps (unrolled at compile time)
    center_x   = 0.5
    center_y   = 0.5
}

def radial_tap(uv : float2; cx : float; cy : float; t : float) : float3 {
    let uvx    = uv.x
    let uvy    = uv.y
    let offset = float2(uvx - cx, uvy - cy) * t
    return sample_scene_color(uv - offset)
}

[pixel_shader]
def radial_blur(inp : PostEffectInput) {
    let uv  = inp.screenPos
    let cx  = center_x
    let cy  = center_y
    let inv = 1.0 / samples
    let s   = strength * inv

    var acc = float3(0.0, 0.0, 0.0)
    for (i in range(8)) {
        let t = float(i) * s
        acc += radial_tap(uv, cx, cy, t)
    }
    return PostEffectOutput(out_color = acc * inv)
}
