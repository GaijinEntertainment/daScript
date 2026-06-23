require engine.render.shader_dsl

var {
    @color tintColor  = float3(1.05, 0.92, 0.75)
    hazeStrength      = 0.012
    hazeSpeed         = 1.2
    hazeScale         = 6.0
    heightFalloff     = 2.0
}

[pixel_shader]
def heat_haze(inp : PostEffectInput) {
    let uv = inp.screenPos
    let t  = g_Time * hazeSpeed

    let heatMask = pow(1.0 - uv.y, heightFalloff)
    let n1 = noise(float2(uv.x * hazeScale,       uv.y * hazeScale * 0.5 + t))
    let n2 = noise(float2(uv.x * hazeScale * 1.7, uv.y * hazeScale * 0.8 - t * 0.6))
    let offset = float2(n1 - 0.5, n2 - 0.5) * hazeStrength * heatMask

    let scene  = sample_scene_color(uv + offset)
    let tinted = lerp(scene, scene * tintColor, heatMask * 0.4)

    return PostEffectOutput(out_color = tinted)
}
