require engine.render.shader_dsl

// Sharpen — unsharp mask. Samples a 3x3 neighbourhood, subtracts
// the blurred version from the original to enhance edges.
// Useful as a final pass to counteract TAA softness.

var {
    strength   = 0.8    // sharpening amount (0 = off, 1 = strong)
    sampleStep = 0.001  // neighbourhood offset in UV space
}

[pixel_shader]
def sharpen(inp : PostEffectInput) {
    let uv  = inp.screenPos
    let s   = sampleStep

    let c   = sample_scene_color(uv)

    // 4-tap cross blur — offsets as separate float pairs to avoid swizzle-on-temp
    var blur = float3(0.0, 0.0, 0.0)
    for (ox, oy in [-s, s, 0.0, 0.0],
                   [0.0, 0.0, -s,  s]) {
        blur += sample_scene_color(uv + float2(ox, oy))
    }
    blur = blur * 0.25

    // Unsharp mask: result = c + strength * (c - blur)
    let sharpened = c + (c - blur) * strength

    return PostEffectOutput(out_color = max(sharpened, float3(0.0, 0.0, 0.0)))
}
