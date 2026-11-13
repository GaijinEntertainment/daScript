require engine.render.shader_dsl

var {
    @color vignetteColor = float3(1.0, 0.05, 0.05)
    shakeStrength        = 0.008
    vignetteStrength     = 0.7
    desaturation         = 0.5
    pulseSpeed           = 3.5
}

[pixel_shader]
def damage(inp : PostEffectInput) {
    let uv    = inp.screenPos
    let pulse = sin(g_Time * pulseSpeed) * 0.5 + 0.5

    let shakeX = (noise(float2(g_Time * 12.0, 0.0)) - 0.5) * shakeStrength * pulse
    let shakeY = (noise(float2(0.0, g_Time * 11.3)) - 0.5) * shakeStrength * pulse
    let scene  = sample_scene_color(uv + float2(shakeX, shakeY))

    let gray  = dot(scene, float3(0.299, 0.587, 0.114))
    let desat = lerp(scene, float3(gray), desaturation * pulse)

    let dist     = length(uv - float2(0.5)) * 1.41421
    let vignette = smooth_step(0.4, 1.0, dist) * vignetteStrength * pulse

    return PostEffectOutput(out_color = lerp(desat, vignetteColor, vignette))
}
