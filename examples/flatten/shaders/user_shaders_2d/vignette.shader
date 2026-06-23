require engine.render.shader_dsl

var {
    @color vignetteColor = float3(0, 0, 0)
    strength = 1.5
    radius   = 0.6
    softness = 0.4
}

[pixel_shader]
def vignette(inp : PostEffectInput) {
    let uv = inp.screenPos
    let col = sample_scene_color(uv)

    // Distance from center [0,1] -> vignette mask
    let d = length(uv - float2(0.5, 0.5)) * strength
    let mask = smooth_step(radius - softness, radius, d)

    return PostEffectOutput(out_color = lerp(col, vignetteColor, mask))
}
