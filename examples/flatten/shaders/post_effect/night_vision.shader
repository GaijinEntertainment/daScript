require engine.render.shader_dsl

// Night vision — green phosphor amplification with noise grain,
// scan lines, vignette, and depth-based fog for the far field.

var {
    @color phosphorColor = float3(0.1, 1.0, 0.25)
    noiseStrength        = 0.35
    scanlineCount        = 200.0
    scanlineDark         = 0.6
    vignetteRadius       = 0.55
    vignetteSoftness     = 0.35
    brightness           = 1.8
    fogStart             = 20.0
    fogEnd               = 60.0
}

def hash_noise(uv : float2; t : float) : float {
    return frac(sin(dot(uv + float2(t, t * 0.7), float2(127.1, 311.7))) * 43758.5)
}

[pixel_shader]
def night_vision(inp : PostEffectInput) {
    let uv    = inp.screenPos
    let col   = sample_scene_color(uv)
    let depth = sample_scene_depth(uv)

    // Convert to luminance and amplify
    let luma  = dot(col, float3(0.299, 0.587, 0.114)) * brightness

    // Noise grain
    let grain = hash_noise(uv, g_Time) * noiseStrength
    let lit   = saturate(luma + grain)

    // Scan lines
    let scan  = lerp(scanlineDark, 1.0, step(0.5, frac(uv.y * scanlineCount)))

    // Depth fog (far objects lose detail)
    let fogT  = saturate((depth - fogStart) / (fogEnd - fogStart))
    let foggedLit = lerp(lit, lit * 0.3, fogT)

    // Vignette
    let d     = length(uv - float2(0.5, 0.5))
    let vig   = smooth_step(vignetteRadius + vignetteSoftness, vignetteRadius, d)

    let final = phosphorColor * foggedLit * scan * vig
    return PostEffectOutput(out_color = final)
}
