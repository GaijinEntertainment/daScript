require engine.render.shader_dsl

var {
    @color deepColor = float3(0.0, 0.15, 0.35)
    @color shallowColor = float3(0.2, 0.6, 0.8)
    @color foamColor = float3(1.5, 1.6, 1.8)
    waveSpeed = 0.6
    waveScale = 5.0
    foamThreshold = 0.55
}

[pixel_shader]
def water(inp : PbrInput) {
    let t = g_Time * waveSpeed
    let uv1 = inp.worldPos * waveScale + float3(t, t * 0.3, t * 0.7)
    let uv2 = inp.worldPos * (waveScale * 2.1) + float3(t * 1.6, 0, t * 0.5)

    let n1 = noise(uv1)
    let n2 = noise(uv2)
    let n = n1 * 0.55 + n2 * 0.45

    let depthMix = lerp(deepColor, shallowColor, n)
    let foamMask = smooth_step(foamThreshold, foamThreshold + 0.15, n)
    let surface = lerp(depthMix, foamColor, foamMask)

    return PbrOutput(
        albedo = surface,
        emission = foamColor * foamMask,
        emissionStrength = foamMask * 0.8,
        metalness = 0.0,
        roughness = 0.2,
        ao = 1.0
    )
}
