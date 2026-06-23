require engine.render.shader_dsl

var {
    @color vignetteColor  = float3(0.05, 0.9, 0.15)
    vignetteStrength      = 0.7
    pulseSpeed            = 1.2
}

[pixel_shader]
def heal(inp : PostEffectInput) {
    let uv    = inp.screenPos
    let pulse = sin(g_Time * pulseSpeed) * 0.3 + 0.7

    let scene = sample_scene_color(uv)

    let dist     = length(uv - float2(0.5)) * 1.41421
    let vignette = smooth_step(0.4, 1.0, dist) * vignetteStrength * pulse

    return PostEffectOutput(out_color = lerp(scene, vignetteColor, vignette))
}
